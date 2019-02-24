#pragma once

#include <wx/wx.h>
#include <mutex>
#include <RtMidi.h>
#include "crossmidi/MidiMessage.hpp"
#include "Chrono.h"
#include "BPMPanel.h"

class DeviceItemPanel;

class MidiDeviceRegistry
{
public:
	virtual void RegisterMTC(RtMidiOut* midi, bool enabled) = 0;
	virtual void RegisterClock(RtMidiOut* midi, bool enabled) = 0;
};

class ChronoFrame : public wxFrame, Chrono::Listener, MidiDeviceRegistry
{
public:
	ChronoFrame();
	virtual ~ChronoFrame();

	wxStaticText* time_display;
	wxStaticText* tick_display;
	BPMPanel* bpm_panel;
	wxButton* start_button;
	wxButton* stop_button;
	wxButton* rewind_button;
	wxScrolledCanvas* output_device_list;

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
	
	//MidiDeviceRegistry
	void RegisterMTC(RtMidiOut* midi_out, bool enabled);
	void RegisterClock(RtMidiOut* midi_out, bool enabled);

	std::mutex m_mutex;
	RtMidiOut* m_midi_out;
	std::vector<RtMidiOut*> m_mtc_devices;
	std::vector<RtMidiOut*> m_clock_devices;

	Chrono m_chrono;
	crossmidi::MidiTimeCode m_time_code_ui;
	uint64_t m_tick_ui;

	wxDECLARE_EVENT_TABLE();
};

class DeviceItemPanel : public wxPanel
{
public:
	DeviceItemPanel(wxWindow* parent, MidiDeviceRegistry* registry, unsigned int port, const wxString& name);
	virtual ~DeviceItemPanel();

	wxStaticText* device_name_label;
	wxCheckBox* mtc_check;
	wxCheckBox* clock_check;

private:

	void OnCheckMTC(wxCommandEvent&);
	void OnCheckClock(wxCommandEvent&);
	void UpdateDevice();

	MidiDeviceRegistry* m_registry;
	RtMidiOut* m_midi_out;
	unsigned int m_port;
	bool m_enable_mtc;
	bool m_enable_clock;

	wxDECLARE_EVENT_TABLE();
};

