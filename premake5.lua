workspace "Valhalla"
    architecture "x64"
    targetdir "build"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    startproject "Odin"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

group "Dependencies"
include "Valhalla/vendor/GLFW"
include "Valhalla/vendor/ImGui"
include "Valhalla/vendor/Glad"
include "Valhalla/vendor/Optick"
group ""

group "Core"
project "Valhalla"
    location "Valhalla"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "vhpch.h"
    pchsource "Valhalla/src/vhpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.inl",

        "%{prj.name}/vendor/yaml-cpp/src/**.cpp",
        "%{prj.name}/vendor/yaml-cpp/src/**.h",
        "%{prj.name}/vendor/yaml-cpp/include/**.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor",

        "%{IncludeDir.Assimp}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiNodeEditor}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.mono}",

        "%{IncludeDir.Optick}"


    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "Optick",
        "opengl32.lib",

        "%{Library.mono}"
    }

    filter "files:Valhalla/vendor/yaml-cpp/src/**.cpp"
    flags {"NoPCH"}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "VH_PLATFORM_WINDOWS"
        }

    filter "system:Unix"
        system "linux"

        defines
        {
            "VH_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines "VH_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "VH_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "VH_DIST"
        optimize "On"
group ""

group "Tools"
project "Odin"
    location "Odin"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    links
    {
        "Valhalla"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "Valhalla/src",
        "Valhalla/vendor",
        "%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiNodeEditor}",
		"%{IncludeDir.Optick}"

    }

    filter "system:windows"
        systemversion "latest"

        defines "VH_PLATFORM_WINDOWS"
    filter "system:Unix"
        system "linux"
        
        defines "VH_PLATFORM_LINUX"
    
    filter "configurations:Debug"
        defines "VH_DEBUG"
        symbols "on"

        links
        {
            "%{Library.Assimp_Debug}"
        }

        postbuildcommands
        {
            '{COPY} "%{Binaries.Assimp_Debug}" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }
    
    filter "configurations:Release"
        defines "VH_RELEASE"
        optimize "on"

        links
        {
            "%{Library.Assimp_Release}"
        }

        postbuildcommands
        {
            '{COPY} "%{Binaries.Assimp_Release}" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }
    
    filter "configurations:Dist"
        defines "VH_DIST"
        optimize "on"

        links
        {
            "%{Library.Assimp_Release}"
        }

        postbuildcommands
        {
            '{COPY} "%{Binaries.Assimp_Release}" "%{cfg.targetdir}"',
            '{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }
    
group ""