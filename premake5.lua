workspace "Valhalla"
	architecture "x64"
	targetdir "build"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Odin"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


	group "Core"
	project "Valhalla"
		location "Valhalla"
		kind "StaticLib"
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
		}

		links
		{
			--"ImGui"
		}

		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"

			defines
			{
				"VH_BUILD_DLL", "VH_PLATFORM_WINDOWS"
			}

			postbuildcommands
			{
				("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Odin")
			}



		filter "configurations:Debug"
			defines "VH_DEBUG"
			symbols "On"


		filter "configurations:Release"
			defines "VH_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "VH_DIST"
			optimize "on"

	group ""

	group"Tools"
	project "Odin"
		location "Odin"
		language "C++"
		kind "ConsoleApp"
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
		}

		postbuildcommands
		{
			
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
					"VH_PLATFORM_WINDOWS"	
			}

		filter "configurations:Debug"
			defines "VH_DEBUG"
			symbols "On"



		filter "configurations:Release"
			defines "VH_RELEASE"
			optimize "on"


		filter "configurations:Dist"
			defines "VH_DIST"
			optimize "on"

		group ""
