#include <high_level\application.hpp>
#include <high_level\graphics_system.hpp>

class MyApp : public lum::Application
{
public:
	virtual void Init()
	{
		m_graphics_system = new lum::GraphicsSystem();
	}

	virtual void Loop()
	{
		m_graphics_system->NewFrame( /* args */ ); // Prep for new frame
		m_graphics_system->Render( /* args */); // Render main game with all render passess and the complete scene graph passed as arguments.
		auto texture = m_graphics_system->Render( /* args */); // Render maybe some model viewer or something to be used with imgui or some other engine backend.
		m_graphics_system->Present();
	}

	virtual void OnResize(int width, int height)
	{

	}

private:
	lum::GraphicsSystem* m_graphics_system;
};

LUMI_ENTRY(MyApp, 1280, 720, "Luimi - Window example")
