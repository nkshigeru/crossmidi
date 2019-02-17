#pragma once

#include <wx/wx.h>
#include <RtMidi.h>

std::unique_ptr<RtMidiIn> _CreateMidiIn();
extern std::function<std::unique_ptr<RtMidiIn>()> CreateMidiIn;

class InputDevicePanel : public wxPanel
{
public:
    InputDevicePanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
    virtual ~InputDevicePanel();

	void Load();
    wxListBox* device_list_ctrl;

private:

	void OnDeviceListDClick(wxCommandEvent&);

	std::unique_ptr<RtMidiIn> m_midi_in;

	wxDECLARE_EVENT_TABLE();
};