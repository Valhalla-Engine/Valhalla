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
include "Valhalla/vendor/ImGui"
include "Valhalla/vendor/Box2D"
include "Valhalla/vendor/Optick"
group ""

group "Core"
project "Valhalla"
	location "Valhalla"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	characterset "MBCS"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vhpch.h"
	pchsource "Valhalla/src/vhpch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/FastNoise/**.cpp",

		"%{prj.name}/vendor/yaml-cpp/src/**.cpp",
		"%{prj.name}/vendor/yaml-cpp/src/**.h",
		"%{prj.name}/vendor/yaml-cpp/include/**.h",
		"%{prj.name}/vendor/VulkanMemoryAllocator/**.h",
		"%{prj.name}/vendor/VulkanMemoryAllocator/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",

		"%{IncludeDir.Assimp}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.VulkanSDK}",
				
		"%{IncludeDir.NvidiaAftermath}",
		"%{IncludeDir.miniaudio}"
	}
	
	links
	{ 
		"ImGui",
		"Box2D",
		"Optick",

		"%{Library.Vulkan}",
		"%{Library.VulkanUtils}",

		"%{Library.mono}",

		"%{Library.NvidiaAftermath}",
	}

	defines
	{
		
	}
	
	filter "files:Valhalla/vendor/FastNoise/**.cpp or files:Valhalla/vendor/yaml-cpp/src/**.cpp"
   	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		
		defines 
		{ 
			"VH_PLATFORM_WINDOWS",
			"VH_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "VH_DEBUG"
		symbols "On"
				
		links
		{
			
		}

	filter "configurations:Release"
		defines
		{
			"VH_RELEASE"
		}

		links
		{
		}

		optimize "On"

	filter "configurations:Dist"
		defines "VH_DIST"
		optimize "On"

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
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"Valhalla/src",
		"Valhalla/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.miniaudio}"
	}

	postbuildcommands 
	{
		'{COPY} "../Valhalla/vendor/NvidiaAftermath/lib/x64/GFSDK_Aftermath_Lib.x64.dll" "%{cfg.targetdir}"'
	}
	
	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"VH_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "VH_DEBUG"
		symbols "on"

		links
		{
			"Valhalla/vendor/assimp/bin/Debug/assimp-vc142-mtd.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../Valhalla/vendor/assimp/bin/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}
				
	filter "configurations:Release"
		defines "VH_RELEASE"
		optimize "on"

		links
		{
			"Valhalla/vendor/assimp/bin/Release/assimp-vc142-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../Valhalla/vendor/assimp/bin/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'

		}

	filter "configurations:Dist"
		defines "VH_DIST"
		optimize "on"

		links
		{
			"Valhalla/vendor/assimp/bin/Release/assimp-vc142-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../Valhalla/vendor/assimp/bin/Release/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

