include "./vendor/premake_customization/solution_items.lua"
include "Dependencies.lua"
include "./Odin/Resources/LUA/Valhalla.lua"


workspace "Valhalla"
	configurations { "Debug", "Release", "Dist" }
	targetdir "build"
	startproject "Odin"
	conformancemode "On"

	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	solution_items { ".editorconfig" }

	flags { "MultiProcessorCompile" }

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
		"TRACY_ENABLE",
		"TRACY_ON_DEMAND",
		"TRACY_CALLSTACK=10",
	}

    filter "action:vs*"
        linkoptions { "/ignore:4099" } 

	filter "language:C++ or language:C"
		architecture "x86_64"

	filter "configurations:Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

	filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Valhalla/vendor/GLFW"
include "Valhalla/vendor/imgui"
include "Valhalla/vendor/Box2D"
group ""

group "Core"
include "Valhalla"
group ""

group "Tools"
include "Odin"
group ""

group "Runtime"
--include "Valhalla-Runtime"
group ""
