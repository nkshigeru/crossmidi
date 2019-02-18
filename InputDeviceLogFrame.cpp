#include "InputDeviceLogFrame.h"
#include <iomanip>
#include <sstream>

std::string midi2str(const unsigned char* data, size_t len)
{
	std::ostringstream os;
	struct MidiMessage {
		unsigned int ch;
		union {
			struct {
				unsigned int note;
				unsigned int velocity;
			} note;
			struct {
				unsigned int number;
				unsigned int value;
			} cc;
			struct {
				unsigned int hh;
				unsigned int mm;
				unsigned int ss;
				unsigned int ff;
			} time_code;
		};
	} midi_message;
	if (len == 1)
	{
		switch (data[0])
		{
		case 0xF8:
			os << "Clock";
			break;
		case 0xFA:
			os << "Start";
			break;
		case 0xFB:
			os << "Continue";
			break;
		case 0xFC:
			os << "Stop";
			break;
		}
	}
	else if (len == 2)
	{
		//Quarter frame message
		if (data[0] == 0xF1)
		{
			unsigned int v = data[1] & 0x0F;
			switch (data[1] & 0xF0)
			{
			case 0x00:
				os << "Frame LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << v;
				break;
			case 0x10:
				os << "Frame MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (v << 4);
				break;
			case 0x20:
				os << "Second LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << v;
				break;
			case 0x30:
				os << "Second MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (v << 4);
				break;
			case 0x40:
				os << "Minute LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << v;
				break;
			case 0x50:
				os << "Minute MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (v << 4);
				break;
			case 0x60:
				os << "Hour LSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << v;
				break;
			case 0x70:
				os << "Hour MSB: 0x" << std::setw(2) << std::setfill('0') << std::hex << (v << 4);
				break;
			}
		}
	}
	else if (len == 3)
	{
		switch (data[0] & 0xF0)
		{
		case 0x80://NoteOff
			midi_message.ch = data[0] & 0xF;
			midi_message.note.note = data[1];
			midi_message.note.velocity = data[2];
			os << "NoteOff ch:" << midi_message.ch + 1
				<< " note:" << midi_message.note.note
				<< " velocity:" << midi_message.note.velocity;
			break;
		case 0x90://NoteOn
			midi_message.ch = data[0] & 0xF;
			midi_message.note.note = data[1];
			midi_message.note.velocity = data[2];
			os << "NoteOn  ch:" << midi_message.ch + 1
				<< " note:" << midi_message.note.note
				<< " velocity:" << midi_message.note.velocity;
			break;
		case 0xB0://CC
			midi_message.ch = data[0] & 0xF;
			midi_message.cc.number = data[1];
			midi_message.cc.value = data[2];
			os << "ControlChange ch:" << midi_message.ch + 1
				<< " number:" << midi_message.cc.number
				<< " value:" << midi_message.cc.value;
			break;
		}
	}
	else if (len == 10)
	{
		//Full time code (F0 7F 7F 01 01 hh mm ss ff F7)
		if (data[0] == 0xF0 && data[1] == 0x7F && data[2] == 0x7F
			&& data[3] == 0x01 && data[4] == 0x01 && data[9] == 0xF7)
		{
			midi_message.time_code.hh = data[5];
			midi_message.time_code.mm = data[6];
			midi_message.time_code.ss = data[7];
			midi_message.time_code.ff = data[8];
			os << "Time Code"
				<< std::setw(2) << std::setfill('0')
				<< midi_message.time_code.hh << ':'
				<< midi_message.time_code.mm << ':'
				<< midi_message.time_code.ss << '.'
				<< midi_message.time_code.ff;
		}
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
			log_area->AppendText(midi2str(&data.message[0], data.message.size()));
			log_area->AppendText(wxT("\n"));
		}
	});
}
