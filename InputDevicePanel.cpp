#include "InputDevicePanel.h"

InputDevicePanel::InputDevicePanel(wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name) : wxPanel(parent, winid, pos, size, style, name)
{
    device_list_ctrl = new wxListBox(this, wxID_ANY);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, _T("Input Devices:")), wxSizerFlags(0).Expand());
	sizer->Add(device_list_ctrl, wxSizerFlags(1).Expand());
	SetSizer(sizer);
}

InputDevicePanel::~InputDevicePanel()
{

}
