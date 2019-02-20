#pragma once

#include <wx/wx.h>
#include "ChronoFrame.h"

class App: public wxApp
{
public:
	App();
    virtual bool OnInit();
};