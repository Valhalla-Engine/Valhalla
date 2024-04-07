project "JoltViewer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    --debugdir "JoltPhysics/"

    targetdir "../../../Hazelnut/Tools/JoltViewer"
    objdir ("bin/" .. outputdir .. "/%{prj.name}")

    files
    {
        "JoltViewer/JoltViewer.cpp",
        "JoltViewer/JoltViewer.h",

        -- Build TestFramework directly into JoltViewer
        "JoltPhysics/TestFramework/**.cpp",
        "JoltPhysics/TestFramework/**.h"
    }

    includedirs { "JoltPhysics/TestFramework/", "JoltPhysics/", "JoltViewer/" }

    links { "JoltPhysics", "Shcore.lib" }

    defines
    {
        "JPH_USE_LZCNT",
        "JPH_USE_TZCNT",
        "JPH_USE_F16C",
        "JPH_USE_FMADD"
    }

    filter "system:windows"
        links { "D3D12.lib", "DXGI.lib", "d3dcompiler.lib", "dinput8.lib", "dxguid.lib" }

        systemversion "latest"
        postbuildcommands  { '{COPY} "JoltPhysics/Assets" "%{cfg.targetdir}/Assets"' }
        
    filter "configurations:Debug"
        symbols "on"
        optimize "off"

        files { "JoltViewer/DummyProfiler.cpp" }

        defines
        {
            "JPH_DEBUG_RENDERER",
            "JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
            "JPH_EXTERNAL_PROFILE"
        }

    filter "configurations:Release"
        optimize "speed"
        
        files { "JoltViewer/DummyProfiler.cpp" }

        defines
        {
            "JPH_DEBUG_RENDERER",
            "JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
            "JPH_EXTERNAL_PROFILE"
        }

    filter "configurations:Dist"
        optimize "speed"
        symbols "off"
