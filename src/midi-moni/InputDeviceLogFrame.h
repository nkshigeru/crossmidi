#pragma once

#include <RtMidi.h>
#include <wx/wx.h>
#include <mutex>
#include "crossmidi/MidiMessage.hpp"

class InputDeviceLogFrame : public wxFrame
{
public:
    InputDeviceLogFrame(unsigned int port);
    virtual ~InputDeviceLogFrame();

	wxTextCtrl* log_area;
private:

	void OnIdle(wxIdleEvent&);

	static void _RtMidiCallback(double timeStamp, std::vector<unsigned char> *message, void *userData);
	void RtMidiCallback(double timeStamp, std::vector<unsigned char> *message);

	std::unique_ptr<RtMidiIn> m_midi_in;
	crossmidi::MidiTimeCode m_mtc;
	wxTextAttr m_textattr_error;
	wxTextAttr m_textattr_raw;
	wxTextAttr m_textattr_info;

	struct Log
	{
		std::vector<unsigned char> message;
	};
	std::vector<Log> m_buffer;
	std::vector<Log> m_buffer_ui;
	std::mutex m_buffer_mutex;

	wxDECLARE_EVENT_TABLE();
};