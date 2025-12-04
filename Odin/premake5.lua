project "Odin"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"   -- required for modules
    staticruntime "Off"  -- optional, your choice

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    -- Files
    files  { 
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp",
        -- Modules (interface units)
        "src/**.ixx",
        "src/**.cppm",

        -- Shaders
        "Resources/Shaders/**.glsl",
        "Resources/Shaders/**.glslh",
        "Resources/Shaders/**.hlsl",
        "Resources/Shaders/**.hlslh",
        "Resources/Shaders/**.slh",
    }

    -- Include directories
    includedirs  {
        "src/",
        "../Valhalla/src/",
        "../Valhalla/vendor/"
    }

    -- Link against engine
    links { "Valhalla" }

    -- Windows-specific settings
    filter "system:windows"
        systemversion "latest"
        defines { "VH_PLATFORM_WINDOWS" }

        -- Link DirectX and other Windows libs for modules that need them
        links { "d3d12", "dxgi", "d3dcompiler" }

        -- Post-build copies
        postbuildcommands {
            '{COPY} "../Valhalla/vendor/NvidiaAftermath/lib/x64/windows/GFSDK_Aftermath_Lib.x64.dll" "%{cfg.targetdir}"',
        }

    -- Debug/Release/Dist postbuild
    filter { "system:windows", "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../Valhalla/vendor/assimp/bin/windows/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
        }

    filter { "system:windows", "configurations:Release or configurations:Dist" }
        postbuildcommands {
            '{COPY} "../Valhalla/vendor/assimp/bin/windows/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }

    -- Configurations
    filter "configurations:Debug"
        defines { "VH_DEBUG" }
        symbols "On"
        ProcessDependencies("Debug")

    filter "configurations:Release"
        defines { "VH_RELEASE" }
        optimize "On"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
        ProcessDependencies("Release")

    filter "configurations:Debug or configurations:Release"
        defines { "VH_TRACK_MEMORY" }

    filter "configurations:Dist"
        defines { "VH_DIST" }
        optimize "On"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
        ProcessDependencies("Dist")

    -- Exclude shader files from compilation
    filter "files:**.hlsl"
        flags { "ExcludeFromBuild" }

    -- C++20 modules support
    filter "system:windows"
        buildoptions { "/experimental:module /std:c++20" }