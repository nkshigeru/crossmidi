#pragma once

#include <wx/wx.h>
#include "InputDevicePanel.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
    virtual ~MainFrame();

	InputDevicePanel* input_device_panel;
};