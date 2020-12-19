workspace "Valhalla"
	architecture "x64"
	targetdir "build"

	configurations
	{
		"Debug_Windows",
		"Debug_Other",
		"Release",
		"Dist"
	}

	startproject "Odin"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Valhalla/vendor/GLFW/include"
	IncludeDir["Glad"] = "Valhalla/vendor/Glad/include"
	IncludeDir["stb"] = "Valhalla/vendor/stb/include"
	IncludeDir["ImGui"] = "Valhalla/vendor/ImGui"
	IncludeDir["glm"] = "Valhalla/vendor/glm"

	include "Valhalla/vendor/GLFW"
	include "Valhalla/vendor/Glad"
	include "Valhalla/vendor/ImGui"

	project "Valhalla"
		location "Valhalla"
		kind "SharedLib"
		language "C++"
		characterset "MBCS"
		cppdialect "C++17"
		staticruntime "on"

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
			"%{prj.name}/src/**.inl"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.stb}",
			"%{prj.name}/vendor/assimp/include"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib"
		}

		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"

			defines
			{
				"VH_BUILD_DLL"
			}

			postbuildcommands
			{
				("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Odin")
			}



		filter "configurations:Debug_Windows"
			defines "VH_DEBUG"
			symbols "On"
			defines
			{
				"VH_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug_Other"
			defines "VH_DEBUG"
			symbols "On"
			defines
			{
				
				"VH_PLATFORM_OTHER"
			}

		filter "configurations:Release"
			defines "VH_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "VH_DIST"
			optimize "on"

	project "Odin"
		location "Odin"
		language "C++"
		kind "ConsoleApp"
		entrypoint "mainCRTStartup"
		cppdialect "C++17"
		staticruntime "on"

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
			"%{IncludeDir.glm}"
		}

		postbuildcommands
		{
			
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
						
			}

		filter "configurations:Debug_Windows"
			defines "VH_DEBUG"
			symbols "On"
			entrypoint "wWinMainCRTStartup"

			defines
			{
				"VH_PLATFORM_WINDOWS"
			}

			links
			{
				"Valhalla/vendor/assimp/bin/Debug/assimp-vc141-md.dll"

			}

			postbuildcommands
			{
				
			}


			filter "configurations:Debug_Other"
			defines "VH_DEBUG"
			symbols "On"

			defines
			{
				"VH_PLATFORM_OTHER"
			}

			links
			{
				"Valhalla/vendor/assimp/bin/Debug/assimp-vc141-md.dll"

			}

			postbuildcommands
			{
				
			}

		filter "configurations:Release"
			defines "VH_RELEASE"
			optimize "on"

			links
			{
				"Valhalla/vendor/assimp/bin/Release/assimp-vc141-mt.dll"
			}

			postbuildcommands
			{
				
			}

		filter "configurations:Dist"
			defines "VH_DIST"
			optimize "on"

		links
		{
			"Valhalla/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands 
		{
			
		}
