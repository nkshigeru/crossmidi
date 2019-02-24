#pragma once

#include <wx/wx.h>

class DigitPanel : public wxWindow
{
public:
	DigitPanel(wxWindow *parent, wxWindowID winid = wxID_ANY);
	virtual ~DigitPanel();

	void SetValue(int value);
	int GetValue() const;

private:
	void OnPaint(wxPaintEvent&);
	void OnSetFocus(wxFocusEvent&);
	void OnKillFocus(wxFocusEvent&);
	void OnChar(wxKeyEvent&);

	int m_value;

	wxDECLARE_EVENT_TABLE();
};
