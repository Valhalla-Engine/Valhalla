#include <Valhalla.h>
#include <Valhalla/EntryPoint.h>
class Sandbox : public Valhalla::Application
{
public:
	Sandbox(const Valhalla::ApplicationSpecification& specification)
		: Application(specification)
	{
		
	}

	~Sandbox()
	{

	}
};

Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv)
{
	Valhalla::ApplicationSpecification specification;
	specification.Name = "Odin";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.StartMaximized = true;
	specification.VSync = true;
	return new Sandbox(specification);
}