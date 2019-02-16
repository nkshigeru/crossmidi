#include "MainFrame.h"

MainFrame::MainFrame()
{
    Create(NULL, wxID_ANY, wxT("MIDI moni"), wxDefaultPosition, FromDIP(wxSize(500, 500)), wxDEFAULT_FRAME_STYLE);
	input_device_panel = new InputDevicePanel(this, wxID_ANY);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(input_device_panel, wxSizerFlags(1).Expand());
	SetSizer(sizer);
}

MainFrame::~MainFrame()
{

}
