#pragma once

#include <string>
#include <functional>
#include <memory>

namespace lum
{

	class Application
	{
	public:
		virtual void Init() = 0;
		virtual void Loop() = 0;

		virtual void OnResize(int width, int height) = 0;

		//std::function<ll::WindowDetails()> GetWindowDetails;
	};

#ifdef PLATFORM_UWP
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class InternalUWPApp sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	internal:
		InternalUWPApp(Application* app, int width, int height);

	public:
		virtual ~InternalUWPApp();

		// IFrameworkView methods.
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers.
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

	private:
		Application* m_internal_app;
		bool m_window_closed;
		bool m_window_visible;
		bool m_initialized;
		// m_title is declared here since the title is needed after run has been called.
		// And its static so I can intiailize it using the macro defintion. (Can't template std::string like with the width and height)
		static std::string m_title;
	};

	template<class T, int width, int height>
	ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
	{
	public:
		virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView()
		{
			auto app = new T();
			return ref new InternalUWPApp(app, width, height);
		}
	};
#endif
} /* lum */

// Macros used to initialize and run a game or the engine.
#ifdef PLATFORM_UWP
#define LUMI_ENTRY(type, width, height, title) [Platform::MTAThread] \
std::string lum::InternalUWPApp::m_title = title; \
int main(Platform::Array<Platform::String^>^) \
{ \
	auto app_source = ref new lum::Direct3DApplicationSource<type, width, height>(); \
	Windows::ApplicationModel::Core::CoreApplication::EnablePrelaunch(false); \
	Windows::ApplicationModel::Core::CoreApplication::Run(app_source); \
	return 0; \
}
#else
#define LUMI_ENTRY(type, width, height, title)
#endif

