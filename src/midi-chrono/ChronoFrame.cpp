#include "ChronoFrame.h"
#include <wx/graphics.h>

enum {
	ID_ = wxID_HIGHEST,
	ID_OUTPUT_DEVICE,
	ID_START,
	ID_STOP,
	ID_REWIND,
	ID_ENABLE_MTC,
	ID_ENABLE_CLOCK,
};

wxBEGIN_EVENT_TABLE(ChronoFrame, wxFrame)
	EVT_CHOICE(ID_OUTPUT_DEVICE, ChronoFrame::OnDeviceSelect)
	EVT_BUTTON(ID_START, ChronoFrame::OnStart)
	EVT_BUTTON(ID_STOP, ChronoFrame::OnStop)
	EVT_BUTTON(ID_REWIND, ChronoFrame::OnRewind)
	EVT_IDLE(ChronoFrame::OnIdle)
	EVT_TIME_CODE_CHANGED(wxID_ANY, ChronoFrame::OnTimeCodeChanged)
	EVT_TICK_CHANGED(wxID_ANY, ChronoFrame::OnTickChanged)
wxEND_EVENT_TABLE()

static const wxColour BackgroundColor(60, 60, 60);
static const wxColour ForegroundColor(255, 255, 255);
static const wxColour ButtonColor(25, 25, 26);
static const wxColour ListBackgroundColor(220, 220, 220);
static const wxColour FocusColor(39, 179, 238);
static const wxColour HoverColor(87, 87, 83);

static void DrawStartButton(wxGraphicsContext* gc)
{
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->SetBrush(*wxWHITE);
	wxPoint2DDouble points[]{
		{ 0, 0 },
		{ 7, 5.5 },
		{ 0, 11 },
	};
	gc->Translate(4.5, 2.5);
	gc->DrawLines(3, points);
}

static void DrawStopButton(wxGraphicsContext* gc)
{
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->SetBrush(*wxWHITE);
	gc->DrawRectangle(3.5, 3.5, 9, 9);
}

static void DrawRewindButton(wxGraphicsContext* gc)
{
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->SetBrush(*wxWHITE);
	wxPoint2DDouble points[]{
		{ 9, 0 },
		{ 2, 5.5 },
		{ 9, 11 },
	};
	gc->Translate(3.5, 2.5);
	gc->DrawLines(3, points);
	gc->DrawRectangle(0, 0, 2, 11);
}

static wxButton* CreateButton(wxWindow* parent, wxWindowID id,
	void (*draw_function)(wxGraphicsContext*),double draw_width, double draw_height)
{
	const int W = 48;
	const int H = 48;
	const double scale = parent->GetContentScaleFactor();
	wxBitmap btn, current, focus;
	btn.CreateScaled(W, H, 24, scale);
	current.CreateScaled(W, H, 24, scale);
	focus.CreateScaled(W, H, 24, scale);
	{
		wxMemoryDC dc(btn);
		wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
		if (gc)
		{
			gc->Scale(scale, scale);
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(ButtonColor);
			gc->DrawRectangle(0, 0, W, H);
			gc->Scale(W / draw_width, H / draw_height);
			draw_function(gc);
			delete gc;
		}
	}
	{
		wxMemoryDC dc(current);
		wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
		if (gc)
		{
			gc->Scale(scale, scale);
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(HoverColor);
			gc->DrawRectangle(0, 0, W, H);
			gc->Scale(W / draw_width, H / draw_height);
			draw_function(gc);
			delete gc;
		}
	}
	{
		wxMemoryDC dc(focus);
		wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
		if (gc)
		{
			gc->Scale(scale, scale);
			gc->SetPen(wxPen(FocusColor, 2));
			gc->SetBrush(ButtonColor);
			gc->DrawRectangle(0, 0, W, H);
			gc->Scale(W / draw_width, H / draw_height);
			draw_function(gc);
			delete gc;
		}
	}
	wxButton* button = new wxButton(parent, id, wxEmptyString, wxDefaultPosition, parent->FromDIP(wxSize(W, H)));
	button->SetBitmap(btn);
	button->SetBitmapCurrent(current);
	button->SetBitmapFocus(focus);
	return button;
}

ChronoFrame::ChronoFrame()
{
    Create(NULL, wxID_ANY, wxT("MIDI Chrono"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	SetInitialSize(FromDIP(wxSize(320, 280)));
	SetBackgroundColour(BackgroundColor);
	SetForegroundColour(ForegroundColor);
	wxFont label_font(FromDIP(wxSize(0, 8)), wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxStaticText* time_label = new wxStaticText(this, wxID_ANY, _T("Time"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	time_label->SetBackgroundColour(*wxBLACK);
	time_label->SetFont(label_font);
	time_code_panel = new TimeCodePanel(this);
	wxStaticText* tick_label = new wxStaticText(this, wxID_ANY, _T("Bars"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	tick_label->SetBackgroundColour(*wxBLACK);
	tick_label->SetFont(label_font);
	tick_panel = new TickPanel(this);
	wxStaticText* bpm_label = new wxStaticText(this, wxID_ANY, _T("BPM"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	bpm_label->SetBackgroundColour(*wxBLACK);
	bpm_label->SetFont(label_font);
	bpm_panel = new BPMPanel(this);
	start_button = CreateButton(this, ID_START, &DrawStartButton, 16, 16);
	stop_button = CreateButton(this, ID_STOP, &DrawStopButton, 16, 16);
	rewind_button = CreateButton(this, ID_REWIND, &DrawRewindButton, 16, 16);

	wxWindow* list_header = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	list_header->SetBackgroundColour(*wxBLACK);
	list_header->SetForegroundColour(ForegroundColor);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* devices_label = new wxStaticText(list_header, wxID_ANY, _("Output Devices"));
		wxStaticText* mtc_label = new wxStaticText(list_header, wxID_ANY, _("MTC"), wxDefaultPosition, FromDIP(wxSize(40, -1)));
		wxStaticText* clock_label = new wxStaticText(list_header, wxID_ANY, _("Clock"), wxDefaultPosition, FromDIP(wxSize(40, -1)));
		sizer->AddSpacer(FromDIP(5));
		sizer->Add(devices_label, wxSizerFlags(1).Expand());
		sizer->Add(mtc_label, wxSizerFlags().Expand());
		sizer->Add(clock_label, wxSizerFlags().Expand());
		list_header->SetSizer(sizer);
	}

	output_device_list = new wxScrolledCanvas(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(-1, 80)), wxVSCROLL);
	output_device_list->SetBackgroundColour(ListBackgroundColor);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(FromDIP(5));
	sizer->Add(list_header, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT, FromDIP(8)));
	sizer->Add(output_device_list, wxSizerFlags(1).Expand().Border(wxLEFT | wxRIGHT, FromDIP(8)));
	sizer->AddSpacer(FromDIP(10));
	{
		wxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		s1->AddSpacer(FromDIP(50));
		s1->AddStretchSpacer(1);
		{
			wxSizer* s2 = new wxBoxSizer(wxVERTICAL);
			s2->Add(time_code_panel, wxSizerFlags());
			s2->Add(time_label, wxSizerFlags().Expand());
			s1->Add(s2, wxSizerFlags());
		}
		s1->AddSpacer(FromDIP(8));
		sizer->Add(s1, wxSizerFlags().Expand());
	}
	{
		wxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		s1->AddSpacer(FromDIP(8));
		s1->AddStretchSpacer(1);
		{
			wxSizer* s2 = new wxBoxSizer(wxVERTICAL);
			s2->Add(bpm_panel, wxSizerFlags());
			s2->Add(bpm_label, wxSizerFlags().Expand());
			s1->Add(s2, wxSizerFlags());
		}
		s1->AddSpacer(FromDIP(20));
		{
			wxSizer* s2 = new wxBoxSizer(wxVERTICAL);
			s2->Add(tick_panel, wxSizerFlags());
			s2->Add(tick_label, wxSizerFlags().Expand());
			s1->Add(s2, wxSizerFlags());
		}
		s1->AddSpacer(FromDIP(8));
		sizer->Add(s1, wxSizerFlags().Expand());
	}
	sizer->AddSpacer(FromDIP(10));
	{
		wxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
		s1->AddStretchSpacer(1);
		s1->Add(rewind_button, wxSizerFlags());
		s1->AddSpacer(FromDIP(10));
		s1->Add(stop_button, wxSizerFlags());
		s1->AddSpacer(FromDIP(10));
		s1->Add(start_button, wxSizerFlags());
		s1->AddStretchSpacer(1);
		sizer->Add(s1, wxSizerFlags(0).Expand());
	}
	sizer->AddSpacer(FromDIP(10));
	SetSizer(sizer);

	m_midi_out = new RtMidiOut();
	LoadDevices();
	
	m_chrono.set_listener(this);
	m_time_code_ui.clear();
	m_time_code_ui.rate = crossmidi::MidiTimeCode::FRAMERATE_25;
	m_tick_ui = 0;
	UpdateTimeDisplay();
	UpdateTickDisplay();
	bpm_panel->SetValue(m_chrono.getBPM());

	start_button->SetFocus();
}

ChronoFrame::~ChronoFrame()
{
	m_chrono.set_listener(nullptr);
	delete m_midi_out;
}

void ChronoFrame::LoadDevices()
{
	unsigned int n = m_midi_out->getPortCount();
	std::vector<wxString> list(n);
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	for (unsigned int i = 0; i < n; ++i)
	{
		try
		{
			wxString name = wxString::FromUTF8(m_midi_out->getPortName(i));
			DeviceItemPanel* item = new DeviceItemPanel(output_device_list, this, i, name);
			sizer->Add(item, wxSizerFlags().Expand());
		}
		catch (RtMidiError& e)
		{
			//TODO error
		}
	}
	output_device_list->SetSizer(sizer);
}

void ChronoFrame::UpdateTimeDisplay()
{
	time_code_panel->SetValue(m_time_code_ui);
}

void ChronoFrame::UpdateTickDisplay()
{
	tick_panel->SetValue(m_tick_ui);
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
	time_code_panel->Enable(false);
	tick_panel->Enable(false);
	int bpm = bpm_panel->GetValue();
	if (bpm == 0)
	{
		bpm_panel->SetValue(m_chrono.getBPM());
	}
	else
	{
		m_chrono.setBPM(bpm);
	}
	bpm_panel->Enable(false);
	m_chrono.start();
}

void ChronoFrame::OnStop(wxCommandEvent&)
{
	m_chrono.stop();
	time_code_panel->Enable(true);
	tick_panel->Enable(true);
	bpm_panel->Enable(true);
}

void ChronoFrame::OnRewind(wxCommandEvent&)
{
	m_chrono.rewind();
}

void ChronoFrame::OnIdle(wxIdleEvent&)
{
	UpdateTimeDisplay();
	UpdateTickDisplay();
}

void ChronoFrame::OnTimeCodeChanged(TimeCodeEvent& evt)
{
	m_time_code_ui = evt.GetTimeCode();
	m_chrono.setTimeCode(m_time_code_ui);
}

void ChronoFrame::OnTickChanged(TickEvent& evt)
{
	m_tick_ui = evt.GetTick();
	m_chrono.setTick(m_tick_ui);
}

void ChronoFrame::start(const crossmidi::MidiTimeCode& mtc)
{
	{
		std::vector<unsigned char> data;
		crossmidi::MidiMessage m;
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_clock_devices.empty())
		{
			if (mtc.hour == 0 && mtc.minute == 0 && mtc.second == 0 && mtc.frame == 0)
			{
				m.type = crossmidi::MidiMessage::CLOCK_START;
			}
			else
			{
				m.type = crossmidi::MidiMessage::CLOCK_CONTINUE;
			}
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_clock_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}

		if (!m_mtc_devices.empty())
		{
			m.type = crossmidi::MidiMessage::MTC;
			m.time_code = mtc;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_mtc_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}
	}

	m_time_code_ui = mtc;
	wxWakeUpIdle();
}

void ChronoFrame::stop(const crossmidi::MidiTimeCode& mtc)
{
	{
		std::vector<unsigned char> data;
		crossmidi::MidiMessage m;
		std::lock_guard<std::mutex> lock(m_mutex);

		if (!m_clock_devices.empty())
		{
			m.type = crossmidi::MidiMessage::CLOCK_STOP;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_clock_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}

		if (!m_mtc_devices.empty())
		{
			m.type = crossmidi::MidiMessage::MTC;
			m.time_code = mtc;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_mtc_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}
	}

	m_time_code_ui = mtc;
	wxWakeUpIdle();
}

void ChronoFrame::seek(const crossmidi::MidiTimeCode& mtc, uint64_t tick)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::vector<unsigned char> data;
		crossmidi::MidiMessage m;
		if (!m_clock_devices.empty())
		{
			m.type = crossmidi::MidiMessage::SPP;
			m.clock = tick / 6;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_clock_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}

		if (!m_mtc_devices.empty())
		{
			m.type = crossmidi::MidiMessage::MTC;
			m.time_code = mtc;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_mtc_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}
	}

	m_time_code_ui = mtc;
	m_tick_ui = tick;
	wxWakeUpIdle();
}

void ChronoFrame::quarter_frame(const crossmidi::MidiTimeCode& mtc, int quarter_frame)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_mtc_devices.empty())
		{
			std::vector<unsigned char> data;
			crossmidi::MidiMessage m;
			if (quarter_frame < 8)
			{
				m.type = (crossmidi::MidiMessage::Type)(crossmidi::MidiMessage::MTC_FRAME_LSB + quarter_frame);
				m.time_code = mtc;
				if (m.ToBytes(data))
				{
					for (RtMidiOut* midi_out : m_mtc_devices)
					{
						midi_out->sendMessage(&data);
					}
				}
			}
		}
	}

	m_time_code_ui = mtc;
	wxWakeUpIdle();
}

void ChronoFrame::clock(uint64_t tick)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_clock_devices.empty())
		{
			std::vector<unsigned char> data;
			crossmidi::MidiMessage m;
			m.type = crossmidi::MidiMessage::CLOCK;
			if (m.ToBytes(data))
			{
				for (RtMidiOut* midi_out : m_clock_devices)
				{
					midi_out->sendMessage(&data);
				}
			}
		}
	}
	m_tick_ui = tick;
	wxWakeUpIdle();
}

void ChronoFrame::RegisterMTC(RtMidiOut* midi_out, bool enabled)
{
	if (midi_out)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (enabled)
		{
			auto it = std::find(m_mtc_devices.begin(), m_mtc_devices.end(), midi_out);
			if (it == m_mtc_devices.end())
			{
				m_mtc_devices.push_back(midi_out);
			}
		}
		else
		{
			auto it = std::remove(m_mtc_devices.begin(), m_mtc_devices.end(), midi_out);
			if (it != m_mtc_devices.end())
			{
				m_mtc_devices.erase(it, m_mtc_devices.end());
			}
		}
	}
}

void ChronoFrame::RegisterClock(RtMidiOut* midi_out, bool enabled)
{
	if (midi_out)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (enabled)
		{
			auto it = std::find(m_clock_devices.begin(), m_clock_devices.end(), midi_out);
			if (it == m_clock_devices.end())
			{
				m_clock_devices.push_back(midi_out);
			}
		}
		else
		{
			auto it = std::remove(m_clock_devices.begin(), m_clock_devices.end(), midi_out);
			if (it != m_clock_devices.end())
			{
				m_clock_devices.erase(it, m_clock_devices.end());
			}
		}
	}
}

wxBEGIN_EVENT_TABLE(DeviceItemPanel, wxPanel)
	EVT_CHECKBOX(ID_ENABLE_MTC, DeviceItemPanel::OnCheckMTC)
	EVT_CHECKBOX(ID_ENABLE_CLOCK, DeviceItemPanel::OnCheckClock)
wxEND_EVENT_TABLE()

DeviceItemPanel::DeviceItemPanel(wxWindow* parent, MidiDeviceRegistry* registry, unsigned int port, const wxString& name)
	: wxPanel(parent), m_registry(registry), m_midi_out(nullptr), m_port(port), m_enable_mtc(false), m_enable_clock(false)
{
	SetBackgroundColour(ListBackgroundColor);
	device_name_label = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
	mtc_check = new wxCheckBox(this, ID_ENABLE_MTC, wxEmptyString, wxDefaultPosition, FromDIP(wxSize(40, -1)));
	clock_check = new wxCheckBox(this, ID_ENABLE_CLOCK, wxEmptyString, wxDefaultPosition, FromDIP(wxSize(40, -1)));
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(FromDIP(5));
	sizer->Add(device_name_label, wxSizerFlags(1).Expand());
	sizer->Add(mtc_check, wxSizerFlags().Expand());
	sizer->Add(clock_check, wxSizerFlags().Expand());
	SetSizer(sizer);
}

DeviceItemPanel::~DeviceItemPanel()
{
	if (m_midi_out)
	{
		delete m_midi_out;
	}
}

void DeviceItemPanel::OnCheckMTC(wxCommandEvent& evt)
{
	m_enable_mtc = evt.IsChecked();
	if (m_enable_mtc)
	{
		UpdateDevice();
		m_registry->RegisterMTC(m_midi_out, true);
	}
	else
	{
		m_registry->RegisterMTC(m_midi_out, false);
		UpdateDevice();
	}
}

void DeviceItemPanel::OnCheckClock(wxCommandEvent& evt)
{
	m_enable_clock = evt.IsChecked();
	if (m_enable_clock)
	{
		UpdateDevice();
		m_registry->RegisterClock(m_midi_out, true);
	}
	else
	{
		m_registry->RegisterClock(m_midi_out, false);
		UpdateDevice();
	}
}

void DeviceItemPanel::UpdateDevice()
{
	bool on = m_enable_mtc || m_enable_clock;
	if (on)
	{
		if (!m_midi_out)
		{
			m_midi_out = new RtMidiOut();
		}
		if (!m_midi_out->isPortOpen())
		{
			m_midi_out->openPort(m_port);
		}
	}
	else
	{
		if (m_midi_out)
		{
			m_midi_out->closePort();
		}
	}
}
