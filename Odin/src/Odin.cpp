#include <Valhalla.h>
#include <Valhalla/Entrypoint.h>

#include <filesystem>

class OdinApplication : public Valhalla::Application
{
public:
	OdinApplication(const Valhalla::ApplicationSpecification& specifications, std::string_view projectPath) : Application(specifications)
	{
		if (projectPath.empty())
			m_ProjectPath = "SandboxProject/Sandbox.vhproj";
	}
	~OdinApplication() {}


	private:
	std::string m_ProjectPath;
	std::filesystem::path m_PersistentStoragePath;
};

Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv)
{
	std::string_view projectPath;
	if (argc > 1)
		projectPath = argv[1];

	VH_CORE_ERROR("Current file path: {0}", projectPath);

	Valhalla::ApplicationSpecification specification;
	specification.Name = "Odin";
	specification.WindowWidth = 1280;
	specification.WindowHeight = 800;
	specification.StartMaximized = true;
	specification.VSync = true;

	return new OdinApplication(specification, projectPath);
}