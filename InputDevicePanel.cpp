#include "InputDevicePanel.h"

std::unique_ptr<RtMidiIn> _CreateMidiIn()
{
	return std::unique_ptr<RtMidiIn>(new RtMidiIn());
}
std::function<std::unique_ptr<RtMidiIn>()> CreateMidiIn = _CreateMidiIn;

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

	m_midi_in = CreateMidiIn();
	Load();
}

InputDevicePanel::~InputDevicePanel()
{

}

void InputDevicePanel::Load()
{
	unsigned int n = m_midi_in->getPortCount();
	std::vector<wxString> list(n);
	for (unsigned int i = 0; i < n; ++i)
	{
		try
		{
			list[i] = m_midi_in->getPortName(i);
		}
		catch (RtMidiError& e)
		{
			//TODO error
		}
	}
	device_list_ctrl->Set(list);
}
