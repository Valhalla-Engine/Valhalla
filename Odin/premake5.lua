project "Odin"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    toolset "v145"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp",

        "src/**.ixx",
        "src/**.cppm",

        "Resources/Shaders/**.glsl",
        "Resources/Shaders/**.glslh",
        "Resources/Shaders/**.hlsl",
        "Resources/Shaders/**.hlslh",
        "Resources/Shaders/**.slh",
    }

    includedirs {
        "src/",
        "../Valhalla/src/",
        "../Valhalla/vendor/"
    }

    links {
        "Valhalla"
    }

    -- MSVC settings that should match Valhalla's module environment
    filter "toolset:msc"
        buildoptions {
            "/utf-8",
            "/Zc:__cplusplus"
        }

    -- Treat actual module files as module units
    filter { "files:**.ixx", "files:**.cppm" }
        compileas "Module"

    -- Windows
    filter "system:windows"
        systemversion "latest"

        buildoptions {
            "/utf-8"
        }

        defines {
            "VH_PLATFORM_WINDOWS"
        }

        links {
            "d3d12",
            "dxgi",
            "d3dcompiler"
        }

        postbuildcommands {
            '{COPY} "../Valhalla/vendor/NvidiaAftermath/lib/x64/windows/GFSDK_Aftermath_Lib.x64.dll" "%{cfg.targetdir}"',
        }

    -- Debug
    filter "configurations:Debug"
        symbols "On"

        defines {
            "VH_DEBUG",
            "_DEBUG"
        }

        ProcessDependencies("Debug")

    filter { "system:windows", "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../Valhalla/vendor/assimp/bin/windows/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
        }

    -- Release
    filter "configurations:Release"
        optimize "On"
        vectorextensions "AVX2"
        isaextensions {
            "BMI",
            "POPCNT",
            "LZCNT",
            "F16C"
        }

        defines {
            "VH_RELEASE",
            "NDEBUG"
        }

        ProcessDependencies("Release")

    -- Dist
    filter "configurations:Dist"
        optimize "On"
        symbols "Off"

        vectorextensions "AVX2"

        isaextensions {
            "BMI",
            "POPCNT",
            "LZCNT",
            "F16C"
        }

        defines {
            "VH_DIST",
            "NDEBUG"
        }

        ProcessDependencies("Dist")

    -- Shared Debug/Release tracking
    filter "configurations:Debug or configurations:Release"
        defines {
            "VH_TRACK_MEMORY"
        }

    -- Release / Dist DLL copies
    filter { "system:windows", "configurations:Release or configurations:Dist" }
        postbuildcommands {
            '{COPY} "../Valhalla/vendor/assimp/bin/windows/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }

    -- Shader files are resources, not C++ compilation units
    filter "files:**.hlsl"
        flags {
            "ExcludeFromBuild"
        }

    -- Always reset the filter at the end
    filter {}