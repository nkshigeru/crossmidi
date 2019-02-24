#pragma once

#include <wx/wx.h>
#include "DigitPanel.h"

class BPMPanel : public wxPanel
{
public:
	BPMPanel(wxWindow *parent, wxWindowID winid = wxID_ANY);
	virtual ~BPMPanel();

	void SetValue(int value);
	int GetValue() const;

private:

	void OnKey100(wxCommandEvent&);
	void OnKey10(wxCommandEvent&);
	void OnKey1(wxCommandEvent&);

	DigitPanel* d100;
	DigitPanel* d10;
	DigitPanel* d1;

	int m_value;

	wxDECLARE_EVENT_TABLE();
};
