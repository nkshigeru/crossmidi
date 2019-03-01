#pragma once

#include <wx/wx.h>
#include "DigitPanel.h"
#include "crossmidi/MidiMessage.hpp"

class TickEvent;
wxDECLARE_EVENT(TICK_CHANGED_EVENT, TickEvent);

class TickEvent : public wxCommandEvent
{
public:
	TickEvent(wxEventType commandType, int id, uint64_t tick)
		: wxCommandEvent(commandType, id), m_tick(tick) { }

	wxEvent* Clone() const { return new TickEvent(*this); }

	void SetTick(uint64_t tick) { m_tick = tick; }
	uint64_t GetTick() const { return m_tick; }

private:
	uint64_t m_tick;
};

typedef void (wxEvtHandler::*TickEventFunction)(TickEvent&);
#define TickEventHandler(func) wxEVENT_HANDLER_CAST(TickEventFunction, func)                    
#define EVT_TICK_CHANGED(id, func) \
 	wx__DECLARE_EVT1(TICK_CHANGED_EVENT, id, TickEventHandler(func))

class TickPanel : public wxPanel
{
public:
	TickPanel(wxWindow *parent, wxWindowID winid = wxID_ANY);
	virtual ~TickPanel();

	bool Enable(bool b) override;

	void SetValue(uint64_t tick);
	uint64_t GetValue() const;

private:

	void OnKeyA1000(wxCommandEvent&);
	void OnKeyA100(wxCommandEvent&);
	void OnKeyA10(wxCommandEvent&);
	void OnKeyA1(wxCommandEvent&);
	void OnKeyE10(wxCommandEvent&);
	void OnKeyE1(wxCommandEvent&);
	void OnKeyT10(wxCommandEvent&);
	void OnKeyT1(wxCommandEvent&);

	DigitPanel* a1000;
	DigitPanel* a100;
	DigitPanel* a10;
	DigitPanel* a1;
	DigitPanel* e10;
	DigitPanel* e1;
	DigitPanel* t10;
	DigitPanel* t1;

	uint64_t m_value;

	wxDECLARE_EVENT_TABLE();
};
