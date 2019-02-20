#pragma once

#include <wx/wx.h>
#include <RtMidi.h>
#include "crossmidi/MidiMessage.hpp"
#include "Chrono.h"

class ChronoFrame : public wxFrame
{
public:
    ChronoFrame();
    virtual ~ChronoFrame();

	wxChoice* output_device_choice;
	wxStaticText* time_display;
	wxButton* start_button;
	wxButton* stop_button;
	wxButton* rewind_button;

private:
	void LoadDevices();
	void UpdateTimeDisplay();

	void OnDeviceSelect(wxCommandEvent&);
	void OnStart(wxCommandEvent&);
	void OnStop(wxCommandEvent&);
	void OnRewind(wxCommandEvent&);
	void OnIdle(wxIdleEvent&);

	void ChronoCallback(const crossmidi::MidiTimeCode& mtc, int quarter_frame);

	RtMidiOut* m_midi_out;

	Chrono m_chrono;
	crossmidi::MidiTimeCode m_time_code_ui;


	wxDECLARE_EVENT_TABLE();
};