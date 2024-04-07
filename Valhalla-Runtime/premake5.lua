project "Valhalla-Runtime"
	kind "ConsoleApp"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	links  { "Valhalla" }

	defines  { "GLM_FORCE_DEPTH_ZERO_TO_ONE", }

	files {
		"src/**.h", 
		"src/**.c", 
		"src/**.hpp", 
		"src/**.cpp" 
	}

	includedirs {
		"src/",
		"../Valhalla/src",
		"../Valhalla/vendor",
	}

	filter "system:windows"
		systemversion "latest"
		defines { "VH_PLATFORM_WINDOWS" }

	filter { "system:windows", "configurations:Debug" }
		postbuildcommands {
			'{COPY} "../Valhalla/vendor/assimp/bin/windows/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/NvidiaAftermath/lib/x64/windows/GFSDK_Aftermath_Lib.x64.dll" "%{cfg.targetdir}"'
		}

	filter { "system:windows", "configurations:Release" }
		postbuildcommands {
			'{COPY} "../Valhalla/vendor/assimp/bin/windows/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
			'{COPY} "../Valhalla/vendor/NvidiaAftermath/lib/x64/windows/GFSDK_Aftermath_Lib.x64.dll" "%{cfg.targetdir}"'
		}

	filter { "system:windows", "configurations:Dist" }
		postbuildcommands {
			'{COPY} "../Valhalla/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Debug"
		symbols "On"
		defines { "VH_DEBUG", }

		ProcessDependencies("Debug")

	filter "configurations:Release"
		optimize "On"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
		defines { "VH_RELEASE", "VH_TRACK_MEMORY", }

		ProcessDependencies("Release")

	filter "configurations:Debug or configurations:Release"
		defines {"VH_TRACK_MEMORY"}

	filter "configurations:Dist"
		kind "WindowedApp"
		optimize "On"
		symbols "Off"
        vectorextensions "AVX2"
        isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
		defines { "VH_DIST" }

		ProcessDependencies("Dist")

        