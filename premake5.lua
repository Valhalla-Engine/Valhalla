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
        "%{prj.name}/src/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.imgui}"

    }

    links
    {
        "GLFW",
        "ImGui",
        "opengl32.lib"
    }

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
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}"
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
    
    filter "configurations:Release"
        defines "VH_RELEASE"
        optimize "on"
    
    filter "configurations:Dist"
        defines "VH_DIST"
        optimize "on"
    
group ""