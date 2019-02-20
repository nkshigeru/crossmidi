#include "ChronoFrame.h"

enum {
	ID_ = wxID_HIGHEST,
	ID_OUTPUT_DEVICE,
	ID_START,
	ID_STOP,
	ID_REWIND,
};

wxBEGIN_EVENT_TABLE(ChronoFrame, wxFrame)
	EVT_CHOICE(ID_OUTPUT_DEVICE, ChronoFrame::OnDeviceSelect)
	EVT_BUTTON(ID_START, ChronoFrame::OnStart)
	EVT_BUTTON(ID_STOP, ChronoFrame::OnStop)
	EVT_BUTTON(ID_REWIND, ChronoFrame::OnRewind)
	EVT_IDLE(ChronoFrame::OnIdle)
wxEND_EVENT_TABLE()


ChronoFrame::ChronoFrame()
{
    Create(NULL, wxID_ANY, wxT("MIDI Chrono"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	SetBackgroundColour(*wxLIGHT_GREY);
	output_device_choice = new wxChoice(this, ID_OUTPUT_DEVICE);
	time_display = new wxStaticText(this, wxID_ANY, wxT("00:00:00.00"), wxDefaultPosition, FromDIP(wxSize(400, 70)),
		wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
	wxFont time_display_font(FromDIP(wxSize(0, 60)), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	time_display->SetFont(time_display_font);
	start_button = new wxButton(this, ID_START, wxT(">"));
	stop_button = new wxButton(this, ID_STOP, wxT("[ ]"));
	rewind_button = new wxButton(this, ID_REWIND, wxT("|<"));

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(FromDIP(5));
	{
		wxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		s1->AddSpacer(FromDIP(5));
		s1->Add(new wxStaticText(this, wxID_ANY, _("Output Device:")), wxSizerFlags(0).CenterVertical());
		s1->AddSpacer(FromDIP(5));
		s1->Add(output_device_choice, wxSizerFlags(1).Expand());
		s1->AddSpacer(FromDIP(5));
		sizer->Add(s1, wxSizerFlags(0).Expand());
	}
	sizer->AddSpacer(FromDIP(10));
	sizer->Add(time_display, wxSizerFlags(0).Expand());
	sizer->AddSpacer(FromDIP(10));
	{
		wxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		s1->Add(rewind_button, wxSizerFlags(1));
		s1->Add(stop_button, wxSizerFlags(1));
		s1->Add(start_button, wxSizerFlags(1));
		sizer->Add(s1, wxSizerFlags(0).Expand());
	}
	SetSizerAndFit(sizer);

	m_midi_out = new RtMidiOut();
	LoadDevices();
	
	m_chrono.set_callback(std::bind(&ChronoFrame::ChronoCallback, this, std::placeholders::_1, std::placeholders::_2));
	m_time_code_ui.clear();
	UpdateTimeDisplay();

	start_button->SetFocus();
}

ChronoFrame::~ChronoFrame()
{
	delete m_midi_out;
}

void ChronoFrame::LoadDevices()
{
	unsigned int n = m_midi_out->getPortCount();
	std::vector<wxString> list(n);
	for (unsigned int i = 0; i < n; ++i)
	{
		try
		{
			list[i] = wxString::FromUTF8(m_midi_out->getPortName(i));
		}
		catch (RtMidiError& e)
		{
			//TODO error
		}
	}
	output_device_choice->Set(list);
}

void ChronoFrame::UpdateTimeDisplay()
{
	time_display->SetLabel(wxString::Format(wxT("%02d:%02d:%02d.%02d"),
		(int)m_time_code_ui.hour, (int)m_time_code_ui.minute, (int)m_time_code_ui.second, (int)m_time_code_ui.frame));
}

void ChronoFrame::OnDeviceSelect(wxCommandEvent& evt)
{
	unsigned int n = evt.GetSelection();
	if (n < m_midi_out->getPortCount())
	{
		if (m_midi_out->isPortOpen())
		{
			m_midi_out->closePort();
		}
		m_midi_out->openPort(n);
	}
}

void ChronoFrame::OnStart(wxCommandEvent&)
{
	m_chrono.start();
}

void ChronoFrame::OnStop(wxCommandEvent&)
{
	m_chrono.stop();
}

void ChronoFrame::OnRewind(wxCommandEvent&)
{
	m_chrono.rewind();
}

void ChronoFrame::OnIdle(wxIdleEvent&)
{
	UpdateTimeDisplay();
}

void ChronoFrame::ChronoCallback(const crossmidi::MidiTimeCode& mtc, int quarter_frame)
{
	std::vector<unsigned char> data;
	crossmidi::MidiMessage m;
	if (quarter_frame == -1)
	{
		m.type = crossmidi::MidiMessage::MTC;
		m.time_code = mtc;
		if (m.ToBytes(data))
		{
			m_midi_out->sendMessage(&data);
		}
	}
	else if (quarter_frame < 8)
	{
		m.type = (crossmidi::MidiMessage::Type)(crossmidi::MidiMessage::MTC_FRAME_LSB + quarter_frame);
		m.time_code = mtc;
		if (m.ToBytes(data))
		{
			m_midi_out->sendMessage(&data);
		}
	}

	m_time_code_ui = mtc;
	wxWakeUpIdle();
}
