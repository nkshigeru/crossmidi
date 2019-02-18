#pragma once

#include <RtMidi.h>
#include <wx/wx.h>

class InputDeviceLogFrame : public wxFrame
{
public:
    InputDeviceLogFrame(unsigned int port);
    virtual ~InputDeviceLogFrame();

	wxTextCtrl* log_area;
private:

	static void _RtMidiCallback(double timeStamp, std::vector<unsigned char> *message, void *userData);
	void RtMidiCallback(double timeStamp, std::vector<unsigned char> *message);

	std::unique_ptr<RtMidiIn> m_midi_in;
	wxTextAttr m_textattr_error;
};