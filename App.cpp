#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxFrame* frame = new wxFrame(NULL, wxID_ANY, wxT("MIDI moni"), wxDefaultPosition, wxSize(500, 500));
    frame->Show(true);
    return true;
}