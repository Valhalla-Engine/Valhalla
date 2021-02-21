#include <Valhalla.h>
#include <Valhalla/Entrypoint.h>

class Odin : public Valhalla::Application
{
public:
	Odin() {}
	~Odin() {}
};

Valhalla::Application* Valhalla::CreateApplication()
{
	return new Odin();
}