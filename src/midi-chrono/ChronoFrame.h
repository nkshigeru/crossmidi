#pragma once

#include <wx/wx.h>
#include <RtMidi.h>
#include "crossmidi/MidiMessage.hpp"
#include "Chrono.h"

class ChronoFrame : public wxFrame, Chrono::Listener
{
public:
    ChronoFrame();
    virtual ~ChronoFrame();

	wxChoice* output_device_choice;
	wxStaticText* time_display;
	wxStaticText* tick_display;
	wxButton* start_button;
	wxButton* stop_button;
	wxButton* rewind_button;

private:
	void LoadDevices();
	void UpdateTimeDisplay();
	void UpdateTickDisplay();

	void OnDeviceSelect(wxCommandEvent&);
	void OnStart(wxCommandEvent&);
	void OnStop(wxCommandEvent&);
	void OnRewind(wxCommandEvent&);
	void OnIdle(wxIdleEvent&);

	//Chrono::Listener
	void start(const crossmidi::MidiTimeCode& mtc) override;
	void stop(const crossmidi::MidiTimeCode& mtc) override;
	void seek(const crossmidi::MidiTimeCode& mtc, uint64_t tick) override;
	void quarter_frame(const crossmidi::MidiTimeCode& mtc, int quarter_frame) override;
	void clock(uint64_t tick) override;

	RtMidiOut* m_midi_out;

	Chrono m_chrono;
	crossmidi::MidiTimeCode m_time_code_ui;
	uint64_t m_tick_ui;


	wxDECLARE_EVENT_TABLE();
};