#pragma once

#include <wx/wx.h>

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

    wxListBox* device_list_ctrl;
};