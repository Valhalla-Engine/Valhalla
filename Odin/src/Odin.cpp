#include <Valhalla.h>
#include <Valhalla/Entrypoint.h>

class OdinApplication : public Valhalla::Application
{
public:
	OdinApplication(const Valhalla::ApplicationSpecification& specifications) : Application(specifications){}
	~OdinApplication() {}
};

Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv)
{
	Valhalla::ApplicationSpecification specification;
	specification.Name = "Odin";
	specification.WindowWidth = 1280;
	specification.WindowHeight = 800;
	specification.StartMaximized = true;
	specification.VSync = true;

	return new OdinApplication(specification);
}