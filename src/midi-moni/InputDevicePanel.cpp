#include "InputDevicePanel.h"
#include "InputDeviceLogFrame.h"

std::unique_ptr<RtMidiIn> _CreateMidiIn()
{
	return std::unique_ptr<RtMidiIn>(new RtMidiIn());
}
std::function<std::unique_ptr<RtMidiIn>()> CreateMidiIn = _CreateMidiIn;

enum {
	ID_ = wxID_HIGHEST,
	ID_INPUT_DEVICE_LIST,
};

wxBEGIN_EVENT_TABLE(InputDevicePanel, wxPanel)
	EVT_LISTBOX_DCLICK(ID_INPUT_DEVICE_LIST, InputDevicePanel::OnDeviceListDClick)
wxEND_EVENT_TABLE()

InputDevicePanel::InputDevicePanel(wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name) : wxPanel(parent, winid, pos, size, style, name)
{
    device_list_ctrl = new wxListBox(this, ID_INPUT_DEVICE_LIST);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(FromDIP(10));
	{
		wxSizer* s1 = new wxBoxSizer(wxVERTICAL);
		s1->AddSpacer(FromDIP(10));
		s1->Add(new wxStaticText(this, wxID_ANY, _T("Input Devices:")), wxSizerFlags(0).Expand());
		s1->Add(device_list_ctrl, wxSizerFlags(1).Expand());
		s1->AddSpacer(FromDIP(10));
		sizer->Add(s1, wxSizerFlags(1).Expand());
	}
	sizer->AddSpacer(FromDIP(10));
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

void InputDevicePanel::OnDeviceListDClick(wxCommandEvent& evt)
{
	int n = evt.GetSelection();
	if (0 <= n && n < m_midi_in->getPortCount())
	{
		InputDeviceLogFrame* new_frame = new InputDeviceLogFrame(n);
		new_frame->Show();
	}
}
