#include "App.h"
#include "ChronoFrame.h"

wxIMPLEMENT_APP(App);

App::App()
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif // _WIN32
}

bool App::OnInit()
{
	auto frame = new ChronoFrame();
    frame->Show(true);
    return true;
}