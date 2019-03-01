#pragma once

#include <wx/wx.h>
#include "DigitPanel.h"
#include "crossmidi/MidiMessage.hpp"

class TimeCodeEvent;
wxDECLARE_EVENT(TIME_CODE_CHANGED_EVENT, TimeCodeEvent);

class TimeCodeEvent : public wxCommandEvent
{
public:
	TimeCodeEvent(wxEventType commandType, int id, crossmidi::MidiTimeCode& time_code)
		: wxCommandEvent(commandType, id), m_time_code(time_code) { }

	wxEvent* Clone() const { return new TimeCodeEvent(*this); }

	void SetTimeCode(const crossmidi::MidiTimeCode& time_code) { m_time_code = time_code; }
	crossmidi::MidiTimeCode GetTimeCode() const { return m_time_code; }

private:
	crossmidi::MidiTimeCode m_time_code;
};

typedef void (wxEvtHandler::*TimeCodeEventFunction)(TimeCodeEvent&);
#define TimeCodeEventHandler(func) wxEVENT_HANDLER_CAST(TimeCodeEventFunction, func)                    
#define EVT_TIME_CODE_CHANGED(id, func) \
 	wx__DECLARE_EVT1(TIME_CODE_CHANGED_EVENT, id, TimeCodeEventHandler(func))

class TimeCodePanel : public wxPanel
{
public:
	TimeCodePanel(wxWindow *parent, wxWindowID winid = wxID_ANY);
	virtual ~TimeCodePanel();

	bool Enable(bool b) override;

	void SetValue(const crossmidi::MidiTimeCode& value);
	crossmidi::MidiTimeCode GetValue() const;

private:

	void OnKeyH10(wxCommandEvent&);
	void OnKeyH1(wxCommandEvent&);
	void OnKeyM10(wxCommandEvent&);
	void OnKeyM1(wxCommandEvent&);
	void OnKeyS10(wxCommandEvent&);
	void OnKeyS1(wxCommandEvent&);
	void OnKeyF10(wxCommandEvent&);
	void OnKeyF1(wxCommandEvent&);

	DigitPanel* h10;
	DigitPanel* h1;
	DigitPanel* m10;
	DigitPanel* m1;
	DigitPanel* s10;
	DigitPanel* s1;
	DigitPanel* f10;
	DigitPanel* f1;

	crossmidi::MidiTimeCode m_value;

	wxDECLARE_EVENT_TABLE();
};
