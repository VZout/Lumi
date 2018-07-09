#include "application.hpp"

#include <algorithm>
#include <wrl\client.h>

namespace lum
{

#ifdef PLATFORM_UWP
	using namespace Windows::ApplicationModel;
	using namespace Windows::ApplicationModel::Core;
	using namespace Windows::ApplicationModel::Activation;
	using namespace Windows::UI::Core;
	using namespace Windows::UI::Input;
	using namespace Windows::System;
	using namespace Windows::Foundation;
	using namespace Windows::Graphics::Display;

	using Microsoft::WRL::ComPtr;

	InternalUWPApp::InternalUWPApp(Application* app, int width, int height) :
		m_window_closed(false),
		m_window_visible(true),
		m_initialized(false),
		m_internal_app(app)
	{
		Windows::UI::ViewManagement::ApplicationView::PreferredLaunchViewSize = Windows::Foundation::Size(width, height);
		Windows::UI::ViewManagement::ApplicationView::PreferredLaunchWindowingMode = Windows::UI::ViewManagement::ApplicationViewWindowingMode::PreferredLaunchViewSize;

		/*app->GetWindowDetails = [&, width, height]() -> rlr::WindowDetails
		{
		using namespace Windows::Foundation;
		using namespace Windows::Graphics::Display;
		using namespace Windows::UI::Core;
		using namespace Platform;

		auto currentDisplayInformation = DisplayInformation::GetForCurrentView();
		auto window = CoreWindow::GetForCurrentThread();

		rlr::WindowDetails details;
		details.native = window;

		if (initialized)
		{
		details.logical_size = { (int)window->Bounds.Width, (int)window->Bounds.Height };
		}
		else
		{
		details.logical_size = { width, height };
		}


		details.native_orientation = currentDisplayInformation->NativeOrientation;
		details.current_orientation = currentDisplayInformation->CurrentOrientation;
		details.dpi = currentDisplayInformation->LogicalDpi;
		details.initial_render_target_size = details.logical_size;

		return details;
		};*/
	}

	InternalUWPApp::~InternalUWPApp()
	{

	}

	// The first method called when the IFrameworkView is being created.
	void InternalUWPApp::Initialize(CoreApplicationView^ applicationView)
	{
		// Register event handlers for app lifecycle. This example includes Activated, so that we
		// can make the CoreWindow active and start rendering on the window.
		applicationView->Activated +=
			ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &InternalUWPApp::OnActivated);

		CoreApplication::Suspending +=
			ref new EventHandler<SuspendingEventArgs^>(this, &InternalUWPApp::OnSuspending);

		CoreApplication::Resuming +=
			ref new EventHandler<Platform::Object^>(this, &InternalUWPApp::OnResuming);
	}

	// Called when the CoreWindow object is created (or re-created).
	void InternalUWPApp::SetWindow(CoreWindow^ window)
	{
		window->SizeChanged +=
			ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &InternalUWPApp::OnWindowSizeChanged);

		window->VisibilityChanged +=
			ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &InternalUWPApp::OnVisibilityChanged);

		window->Closed +=
			ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &InternalUWPApp::OnWindowClosed);

		DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

		currentDisplayInformation->DpiChanged +=
			ref new TypedEventHandler<DisplayInformation^, Object^>(this, &InternalUWPApp::OnDpiChanged);

		currentDisplayInformation->OrientationChanged +=
			ref new TypedEventHandler<DisplayInformation^, Object^>(this, &InternalUWPApp::OnOrientationChanged);

		DisplayInformation::DisplayContentsInvalidated +=
			ref new TypedEventHandler<DisplayInformation^, Object^>(this, &InternalUWPApp::OnDisplayContentsInvalidated);

		/*// Styling
		auto title_bar = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->TitleBar;

		// Title bar colors. Alpha must be 255.
		title_bar->BackgroundColor = Windows::UI::ColorHelper::FromArgb(255, 82, 176, 67);
		title_bar->ForegroundColor = Windows::UI::ColorHelper::FromArgb(255, 0, 0, 0); // text color
		title_bar->InactiveBackgroundColor = Windows::UI::ColorHelper::FromArgb(255, 24, 52, 20);
		title_bar->InactiveForegroundColor = Windows::UI::ColorHelper::FromArgb(255, 255, 255, 255); // text color

		// Title bar button background colors. Alpha is respected when the view is extended
		// into the title bar (see scenario 2). Otherwise, Alpha is ignored and treated as if it were 255.
		float buttonAlpha = 200;
		title_bar->ButtonBackgroundColor = Windows::UI::ColorHelper::FromArgb(buttonAlpha, 82, 176, 67);
		title_bar->ButtonHoverBackgroundColor = Windows::UI::ColorHelper::FromArgb(buttonAlpha, 73, 158, 60);
		title_bar->ButtonPressedBackgroundColor = Windows::UI::ColorHelper::FromArgb(buttonAlpha, 58, 126, 48);
		title_bar->ButtonInactiveBackgroundColor = Windows::UI::ColorHelper::FromArgb(0, 0, 0, 0);

		// Title bar button foreground colors. Alpha must be 255.
		title_bar->ButtonForegroundColor = Windows::UI::ColorHelper::FromArgb(255, 255, 255, 255);
		title_bar->ButtonHoverForegroundColor = Windows::UI::ColorHelper::FromArgb(255, 255, 255, 255);
		title_bar->ButtonPressedForegroundColor = Windows::UI::ColorHelper::FromArgb(255, 255, 255, 255);
		title_bar->ButtonInactiveForegroundColor = Windows::UI::ColorHelper::FromArgb(0, 0, 0, 0);*/
	}

	// Initializes scene resources, or loads a previously saved app state.
	void InternalUWPApp::Load(Platform::String^ entryPoint)
	{
	}

	// This method is called after the window becomes active.
	void InternalUWPApp::Run()
	{
		std::wstring wide_title(m_title.begin(), m_title.end());
		Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->Title = ref new Platform::String(wide_title.c_str());

		while (!m_window_closed)
		{
			if (m_window_visible)
			{
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

				m_internal_app->Loop();
			}
			else
			{
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			}
		}
	}

	// Required for IFrameworkView.
	// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
	// class is torn down while the app is in the foreground.
	void InternalUWPApp::Uninitialize()
	{
	}

	// Application lifecycle event handlers.
	void InternalUWPApp::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
	{
		applicationView->TitleBar->ExtendViewIntoTitleBar = false;

		// Run() won't start until the CoreWindow is activated.
		CoreWindow::GetForCurrentThread()->Activate();

		m_internal_app->Init();

		m_initialized = true;
	}

	void InternalUWPApp::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
	{

	}

	void InternalUWPApp::OnResuming(Platform::Object^ sender, Platform::Object^ args)
	{

	}

	// Window event handlers.
	void InternalUWPApp::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
	{
		if (m_initialized)
		{
			m_internal_app->OnResize(args->Size.Width, args->Size.Height);
		}
	}

	void InternalUWPApp::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
	{
		m_window_visible = args->Visible;
	}

	void InternalUWPApp::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		m_window_closed = true;
	}

	// DisplayInformation event handlers.
	void InternalUWPApp::OnDpiChanged(DisplayInformation^ sender, Object^ args)
	{

	}

	void InternalUWPApp::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
	{

	}

	void InternalUWPApp::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
	{

	}
#endif

} /* lum */