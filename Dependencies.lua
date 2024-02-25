VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Assimp"] = "%{wks.location}/Valhalla/vendor/assimp/include"
IncludeDir["stb"] = "%{wks.location}/Valhalla/vendor/stb/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Valhalla/vendor/yaml-cpp/include"
IncludeDir["ImGui"] = "%{wks.location}/Valhalla/vendor/ImGui"
IncludeDir["Box2D"] = "%{wks.location}/Valhalla/vendor/Box2D/include"
IncludeDir["entt"] = "%{wks.location}/Valhalla/vendor/entt/include"
IncludeDir["FastNoise"] = "%{wks.location}/Valhalla/vendor/FastNoise"
IncludeDir["mono"] = "%{wks.location}/Valhalla/vendor/mono/include"
IncludeDir["PhysX"] = "%{wks.location}/Valhalla/vendor/PhysX/include"
IncludeDir["shaderc"] = "%{wks.location}/Valhalla/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Valhalla/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["NvidiaAftermath"] = "%{wks.location}/Valhalla/vendor/NvidiaAftermath/include"
IncludeDir["miniaudio"] = "%{wks.location}/Valhalla/vendor/miniaudio/include"
IncludeDir["Optick"] = "%{wks.location}/Valhalla/vendor/Optick/src"
IncludeDir["DirectXTK"] = "%{wks.location}/Valhalla/vendor/DirectXTK/Inc"

LibraryDir = {}

LibraryDir["PhysX"] = "%{wks.location}/Valhalla/vendor/PhysX/lib/%{cfg.buildcfg}"
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Valhalla/vendor/VulkanSDK/Lib"
LibraryDir["NvidiaAftermath"] = "%{wks.location}/Valhalla/vendor/NvidiaAftermath/lib/x64"
LibraryDir["DirectXTK"] = "%{wks.location}/Valhalla/vendor/DirectXTK/Bin/"

Library = {}
Library["mono"] = "%{wks.location}/Valhalla/vendor/mono/lib/Debug/mono-2.0-sgen.lib"
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["NvidiaAftermath"] = "%{LibraryDir.NvidiaAftermath}/GFSDK_Aftermath_Lib.x64.lib"

Library["DirectXTK_Debug"] = "%{LibraryDir.DirectXTK}/Debug-windows-x86_64/DirectXTK/DirectXTK.lib"
Library["DirectXTK_Release"] = "%{LibraryDir.DirectXTK}/Release-windows-x86_64/DirectXTK/DirectXTK.lib"

