#pragma once

#include <wx/wx.h>
#include "MainFrame.h"

class App: public wxApp
{
public:
	App();
    virtual bool OnInit();

	wxWeakRef<MainFrame> main_frame;
};