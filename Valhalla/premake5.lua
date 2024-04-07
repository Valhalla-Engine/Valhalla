project "Valhalla"
	kind "StaticLib"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vhpch.h"
	pchsource "src/vhpch.cpp"

	files {
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp",

		"Platform/" .. firstToUpper(os.target()) .. "/**.hpp",
		"Platform/" .. firstToUpper(os.target()) .. "/**.cpp",

		"vendor/FastNoise/**.cpp",

		"vendor/yaml-cpp/src/**.cpp",
		"vendor/yaml-cpp/src/**.h",
		"vendor/yaml-cpp/include/**.h",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp",

		"vendor/imgui/misc/cpp/imgui_stdlib.cpp",
		"vendor/imgui/misc/cpp/imgui_stdlib.h"
	}

	includedirs { "src/", "vendor/", }

	IncludeDependencies()

	defines { "GLM_FORCE_DEPTH_ZERO_TO_ONE", }

	filter "files:vendor/FastNoise/**.cpp or files:vendor/yaml-cpp/src/**.cpp or files:vendor/imgui/misc/cpp/imgui_stdlib.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		defines { "VH_PLATFORM_WINDOWS", }


	filter "configurations:Debug"
		symbols "On"
		defines { "VH_DEBUG", "_DEBUG", }

	filter "configurations:Release"
		optimize "On"
		vectorextensions "AVX2"
		isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
		defines { "VH_RELEASE", "NDEBUG", }

	filter { "configurations:Debug or configurations:Release" }
		defines {
			"VH_TRACK_MEMORY"
		}

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
			"src/Valhalla/Asset/AssimpMeshImporter.cpp",
		}
	