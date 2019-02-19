#include "App.h"

wxIMPLEMENT_APP(App);

App::App()
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif // _WIN32
}

bool App::OnInit()
{
    main_frame = new MainFrame();
    main_frame->Show(true);
    return true;
}