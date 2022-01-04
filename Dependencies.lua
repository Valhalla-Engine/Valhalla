IncludeDir = {}
IncludeDir["Assimp"] = "%{wks.location}/Valhalla/vendor/assimp/include"
IncludeDir["stb"] = "%{wks.location}/Valhalla/vendor/stb/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Valhalla/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Valhalla/vendor/glfw"
IncludeDir["Glad"] = "%{wks.location}/Valhalla/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Valhalla/vendor/Imgui"
IncludeDir["ImGuiNodeEditor"] = "%{wks.location}/Valhalla/vendor/imgui-node-editor"
IncludeDir["glm"] = "%{wks.location}/Valhalla/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Valhalla/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/Valhalla/vendor/mono/include"
IncludeDir["Optick"] = "%{wks.location}/Valhalla/vendor/Optick/src"


Library = {}
Library["Assimp_Debug"] = "%{wks.location}/Valhalla/vendor/assimp/bin/Debug/assimp-vc142-mtd.lib"
Library["Assimp_Release"] = "%{wks.location}/Valhalla/vendor/assimp/bin/Release/assimp-vc142-mt.lib"
Library["mono"] = "%{wks.location}/Valhalla/vendor/mono/lib/Release/mono-2.0-sgen.lib"

Binaries = {}
Binaries["Assimp_Debug"] = "%{wks.location}/Valhalla/vendor/assimp/bin/Debug/assimp-vc142-mtd.dll"
Binaries["Assimp_Release"] = "%{wks.location}/Valhalla/vendor/assimp/bin/Release/assimp-vc142-mt.dll"