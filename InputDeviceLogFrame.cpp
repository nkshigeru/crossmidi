#include "InputDeviceLogFrame.h"
#include <iomanip>
#include <sstream>
#include "crossmidi/MidiMessage.hpp"

static inline std::ostream& operator <<(std::ostream& os, const crossmidi::MidiTimeCode& mtc)
{
	os << std::setfill('0') << std::dec
		<< std::setw(2) << (int)mtc.hour << ':'
		<< std::setw(2) << (int)mtc.minute << ':'
		<< std::setw(2) << (int)mtc.second << '.'
		<< std::setw(2) << (int)mtc.frame;
	switch (mtc.rate) {
	case crossmidi::MidiTimeCode::FRAMERATE_24:
		os << " (24 frames/s)";
		break;
	case crossmidi::MidiTimeCode::FRAMERATE_25:
		os << " (25 frames/s)";
		break;
	case crossmidi::MidiTimeCode::FRAMERATE_29_97:
		os << " (29.97 frames/s)";
		break;
	case crossmidi::MidiTimeCode::FRAMERATE_30:
		os << " (30 frames/s)";
		break;
	}
	return os;
}

std::string midi2str(const unsigned char* data, size_t len, crossmidi::MidiTimeCode& mtc)
{
	std::ostringstream os;
	crossmidi::MidiMessage m = crossmidi::MidiMessage::FromBytes(data, len);
	switch (m.type)
	{
	case crossmidi::MidiMessage::CLOCK:
		os << "Clock";
		break;
	case crossmidi::MidiMessage::CLOCK_START:
		os << "Start";
		break;
	case crossmidi::MidiMessage::CLOCK_CONTINUE:
		os << "Continue";
		break;
	case crossmidi::MidiMessage::CLOCK_STOP:
		os << "Stop";
		break;
	case crossmidi::MidiMessage::MTC_FRAME_LSB:
		mtc.frame = (mtc.frame & 0xF0) | (m.time_code.frame & 0x0F);
		os << "Frame LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.frame
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_FRAME_MSB:
		mtc.frame = (m.time_code.frame & 0xF0) | (mtc.frame & 0x0F);
		os << "Frame MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.frame
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_SECOND_LSB:
		mtc.second = (mtc.second & 0xF0) | (m.time_code.second & 0x0F);
		os << "Second LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.second
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_SECOND_MSB:
		mtc.second = (m.time_code.second & 0xF0) | (mtc.second & 0x0F);
		os << "Second MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.second
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_MINUTE_LSB:
		mtc.minute = (mtc.minute & 0xF0) | (m.time_code.minute & 0x0F);
		os << "Minute LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.second
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_MINUTE_MSB:
		mtc.minute = (m.time_code.minute & 0xF0) | (mtc.minute & 0x0F);
		os << "Minute MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.minute
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_HOUR_LSB:
		mtc.hour = (mtc.hour & 0xF0) | (m.time_code.hour & 0x0F);
		os << "Hour LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.hour
			<< " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::MTC_RATE_HOUR_MSB:
		mtc.rate = m.time_code.rate;
		mtc.hour = (m.time_code.hour & 0xF0) | (mtc.hour & 0x0F);
		os << "Hour MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)m.time_code.hour;
		switch (m.time_code.rate) {
		case crossmidi::MidiTimeCode::FRAMERATE_24:
			os << " FrameRate: 24";
			break;
		case crossmidi::MidiTimeCode::FRAMERATE_25:
			os << " FrameRate: 25";
			break;
		case crossmidi::MidiTimeCode::FRAMERATE_29_97:
			os << " FrameRate: 29.97";
			break;
		case crossmidi::MidiTimeCode::FRAMERATE_30:
			os << " FrameRate: 30";
			break;
		}
		os << " TimeCode: " << mtc;
		break;
	case crossmidi::MidiMessage::NOTE_OFF:
		os << "NoteOff ch:" << (int)m.note.ch + 1
			<< " note:" << (int)m.note.note
			<< " velocity:" << (int)m.note.velocity;
		break;
	case crossmidi::MidiMessage::NOTE_ON:
		os << "NoteOn  ch:" << (int)m.note.ch + 1
			<< " note:" << (int)m.note.note
			<< " velocity:" << (int)m.note.velocity;
		break;
	case crossmidi::MidiMessage::CC:
		os << "ControlChange ch:" << (int)m.cc.ch + 1
			<< " number:" << (int)m.cc.number
			<< " value:" << (int)m.cc.value;
		break;
	case crossmidi::MidiMessage::MTC:
		mtc = m.time_code;
		os << "Time Code " << m.time_code;
		break;
	}
	return os.str();
}

InputDeviceLogFrame::InputDeviceLogFrame(unsigned int port)
{
    Create(NULL, wxID_ANY, wxString::Format(wxT("MIDI moni #%d"), port), wxDefaultPosition, FromDIP(wxSize(500, 500)), wxDEFAULT_FRAME_STYLE);
	log_area = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(log_area, wxSizerFlags(1).Expand());
	SetSizer(sizer);

	wxFont default_font = log_area->GetFont();
	m_textattr_error = wxTextAttr(*wxRED, *wxWHITE, default_font);
	wxFont raw_font = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Consolas"));
	m_textattr_raw = wxTextAttr(*wxWHITE, wxColour(60, 60, 60), raw_font);
	m_textattr_info = wxTextAttr(wxColour(20, 200, 20), *wxWHITE, default_font);

	m_mtc.clear();
	m_midi_in = std::unique_ptr<RtMidiIn>(new RtMidiIn());
	if (port < m_midi_in->getPortCount())
	{
		try
		{
			wxString name = wxString::FromUTF8(m_midi_in->getPortName(port));
			SetTitle(wxString::Format(wxT("MIDI moni #%d %s"), port, name.c_str()));
			log_area->AppendText(name + wxT("\n"));

			m_midi_in->setCallback(&InputDeviceLogFrame::_RtMidiCallback, this);
			m_midi_in->openPort(port);
			m_midi_in->ignoreTypes(false, false, false);
			log_area->AppendText(wxT("OK\n"));
		}
		catch (RtMidiError& e)
		{
			log_area->SetDefaultStyle(m_textattr_error);
			log_area->AppendText(e.getMessage());
		}
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
		if (!data.message.empty())
		{
			log_area->SetDefaultStyle(m_textattr_raw);
			std::ostringstream os;
			for (size_t i = 0; i < data.message.size(); ++i)
			{
				if (i != 0)
				{
					os << " ";
				}
				os << std::setw(2) << std::setfill('0') << std::hex << (int)data.message[i];
			}
			log_area->AppendText(os.str());
			log_area->SetDefaultStyle(m_textattr_info);
			log_area->AppendText(wxT(" "));
			std::string info = midi2str(&data.message[0], data.message.size(), m_mtc);
			log_area->AppendText(info);
			log_area->AppendText(wxT("\n"));
		}
	});
}
