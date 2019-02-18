#include "InputDeviceLogFrame.h"

InputDeviceLogFrame::InputDeviceLogFrame(unsigned int port)
{
    Create(NULL, wxID_ANY, wxString::Format(wxT("MIDI moni #%d"), port), wxDefaultPosition, FromDIP(wxSize(500, 500)), wxDEFAULT_FRAME_STYLE);
	log_area = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(log_area, wxSizerFlags(1).Expand());
	SetSizer(sizer);

	m_midi_in = std::unique_ptr<RtMidiIn>(new RtMidiIn());
	if (port < m_midi_in->getPortCount())
	{
		wxString name = wxString::FromUTF8(m_midi_in->getPortName(port));
		SetTitle(wxString::Format(wxT("MIDI moni #%d %s"), port, name.c_str()));
		log_area->AppendText(name + wxT("\n"));

		m_midi_in->setCallback(&InputDeviceLogFrame::_RtMidiCallback, this);
		m_midi_in->openPort(port);
		m_midi_in->ignoreTypes(false, false, false);
		log_area->AppendText(wxT("OK\n"));
	}
}

InputDeviceLogFrame::~InputDeviceLogFrame()
{

}

void InputDeviceLogFrame::_RtMidiCallback(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
	if (userData)
	{
		((InputDeviceLogFrame*)userData)->RtMidiCallback(timeStamp, message);
	}
}

void InputDeviceLogFrame::RtMidiCallback(double timeStamp, std::vector<unsigned char>* message)
{
	struct MidiInData
	{
		double timeStamp;
		std::vector<unsigned char> message;
	};
	MidiInData data;
	data.timeStamp = timeStamp;
	if (message)
	{
		data.message.resize(message->size());
		for (size_t i = 0; i < message->size(); ++i)
		{
			data.message[i] = (*message)[i];
		}
	}
	CallAfter([this, data]() {
		for (size_t i = 0; i < data.message.size(); ++i)
		{
			log_area->AppendText(wxString::Format(wxT("%02x "), (int)data.message[i]));
		}
		log_area->AppendText(wxT("\n"));
	});
}
