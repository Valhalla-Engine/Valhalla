project "Valhalla"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    toolset "v145"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    -- All sources, including module interface (.ixx) and module implementation (.cppm)
    files {
        "src/**.ixx",
        "src/**.cppm",
        "src/**.cxx",
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        "Platform/" .. firstToUpper(os.target()) .. "/**.hpp",
        "Platform/" .. firstToUpper(os.target()) .. "/**.cpp",
        "Platform/" .. firstToUpper(os.target()) .. "/**.ixx",
        "Platform/" .. firstToUpper(os.target()) .. "/**.cppm",

        "vendor/FastNoise/**.cpp",
        "vendor/yaml-cpp/src/**.cpp",
        "vendor/yaml-cpp/src/**.h",
        "vendor/yaml-cpp/include/**.h",
        "vendor/VulkanMemoryAllocator/**.h",
        "vendor/VulkanMemoryAllocator/**.cpp",

        "vendor/imgui/misc/cpp/imgui_stdlib.cpp",
        "vendor/imgui/misc/cpp/imgui_stdlib.h"
    }

    includedirs {
        "src/",
        "vendor/",
        "vendor/DirectXMath/Inc"
    }

    IncludeDependencies()

    defines { "GLM_FORCE_DEPTH_ZERO_TO_ONE" }

    -- Disable PCH for external/vendor files
    filter { "files:vendor/FastNoise/**.cpp", "files:vendor/yaml-cpp/src/**.cpp", "files:vendor/imgui/misc/cpp/imgui_stdlib.cpp" }
        flags { "NoPCH" }

    -- Enable modules for your project files
    filter { "files:src/**.ixx", "files:src/**.cppm" }
        compileas "Module"

    filter "toolset:msc"
        buildoptions { 
            "/scanDependencies",
            "/utf-8",
            "/Zc:__cplusplus"
        }

    -- Platform-specific
    filter "system:windows"
        systemversion "latest"
        defines { "VH_PLATFORM_WINDOWS" }
        buildoptions { "/experimental:module", "/utf-8", "/Zc:__cplusplus" }

        files {
            "vendor/imgui/backends/imgui_impl_win32.cpp",
            "vendor/imgui/backends/imgui_impl_dx12.cpp"
        }

        links {
            "d3d12",
            "dxgi",
            "d3dcompiler"
        }

    -- Configurations
    filter "configurations:Debug"
        symbols "On"
        defines { "VH_DEBUG", "_DEBUG" }

    filter "configurations:Release"
        optimize "On"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
        defines { "VH_RELEASE", "NDEBUG" }

    filter { "configurations:Debug or configurations:Release" }
        defines { "VH_TRACK_MEMORY" }

    filter "configurations:Dist"
        optimize "On"
        symbols "Off"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
        defines { "VH_DIST" }

        removefiles {
            "src/Valhalla/Platform/Vulkan/ShaderCompiler/**.cpp",
            "src/Valhalla/Platform/Vulkan/Debug/**.cpp",
            "src/Valhalla/Asset/AssimpAnimationImporter.cpp",
            "src/Valhalla/Asset/AssimpMeshImporter.cpp"
        }
