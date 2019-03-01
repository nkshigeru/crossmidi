#include "TickPanel.h"

wxDEFINE_EVENT(TICK_CHANGED_EVENT, TickEvent);

enum
{
	ID_ = wxID_HIGHEST,
	ID_DIGIT_A1000,
	ID_DIGIT_A100,
	ID_DIGIT_A10,
	ID_DIGIT_A1,
	ID_DIGIT_E10,
	ID_DIGIT_E1,
	ID_DIGIT_T10,
	ID_DIGIT_T1,
};

wxBEGIN_EVENT_TABLE(TickPanel, wxPanel)
	EVT_TEXT(ID_DIGIT_A1000, TickPanel::OnKeyA1000)
	EVT_TEXT(ID_DIGIT_A100, TickPanel::OnKeyA100)
	EVT_TEXT(ID_DIGIT_A10, TickPanel::OnKeyA10)
	EVT_TEXT(ID_DIGIT_A1, TickPanel::OnKeyA1)
	EVT_TEXT(ID_DIGIT_E10, TickPanel::OnKeyE10)
	EVT_TEXT(ID_DIGIT_E1, TickPanel::OnKeyE1)
	EVT_TEXT(ID_DIGIT_T10, TickPanel::OnKeyT10)
	EVT_TEXT(ID_DIGIT_T1, TickPanel::OnKeyT1)
wxEND_EVENT_TABLE()

struct Bars
{
	unsigned int bars;
	unsigned int beats;
	unsigned int tick;

	void set_from_tick(uint64_t t)
	{
		bars = t / (24 * 4);
		t -= bars * 24 * 4;
		beats = t / 24;
		t -= beats * 24;
		tick = t;
	}

	uint64_t to_tick() const
	{
		return bars * 24 * 4 + beats * 24 + tick;
	}

	bool set_bars_from_ui(unsigned int v)
	{
		if (v > 0)
		{
			bars = v - 1;
			return true;
		}
		return false;
	}
	bool set_beats_from_ui(unsigned int v)
	{
		if (v > 0)
		{
			beats = v - 1;
			return true;
		}
		return false;
	}
	bool set_tick_from_ui(unsigned int v)
	{
		if (v < 24)
		{
			tick = v;
			return true;
		}
		return false;
	}
	unsigned int bars_digit_for_ui(unsigned int d) const
	{
		return ((bars + 1) / d) % 10;
	}
	unsigned int beats_digit_for_ui(unsigned int d) const
	{
		return ((beats + 1) / d) % 10;
	}
	unsigned int tick_digit_for_ui(unsigned int d) const
	{
		return (tick / d) % 10;
	}
};

TickPanel::TickPanel(wxWindow* parent, wxWindowID id)
	: wxPanel(parent, id)
{
	SetForegroundColour(*wxWHITE);

	a1000 = new DigitPanel(this, ID_DIGIT_A1000);
	a100 = new DigitPanel(this, ID_DIGIT_A100);
	a10 = new DigitPanel(this, ID_DIGIT_A10);
	a1 = new DigitPanel(this, ID_DIGIT_A1);
	e10 = new DigitPanel(this, ID_DIGIT_E10);
	e1 = new DigitPanel(this, ID_DIGIT_E1);
	t10 = new DigitPanel(this, ID_DIGIT_T10);
	t1 = new DigitPanel(this, ID_DIGIT_T1);

	wxStaticText* label0 = new wxStaticText(this, wxID_ANY, wxT("."), wxDefaultPosition, a1->GetSize());
	label0->SetFont(a1->GetFont());
	wxStaticText* label1 = new wxStaticText(this, wxID_ANY, wxT("."), wxDefaultPosition, a1->GetSize());
	label1->SetFont(a1->GetFont());

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(a1000);
	sizer->Add(a100);
	sizer->Add(a10);
	sizer->Add(a1);
	sizer->Add(label0);
	sizer->Add(e10);
	sizer->Add(e1);
	sizer->Add(label1);
	sizer->Add(t10);
	sizer->Add(t1);
	SetSizerAndFit(sizer);

	m_value = 0;
	Bars bars;
	bars.set_from_tick(m_value);
	a1000->SetValue(bars.bars_digit_for_ui(1000));
	a100->SetValue(bars.bars_digit_for_ui(100));
	a10->SetValue(bars.bars_digit_for_ui(10));
	a1->SetValue(bars.bars_digit_for_ui(1));
	e10->SetValue(bars.beats_digit_for_ui(10));
	e1->SetValue(bars.beats_digit_for_ui(1));
	t10->SetValue(bars.tick_digit_for_ui(10));
	t1->SetValue(bars.tick_digit_for_ui(1));
}

TickPanel::~TickPanel()
{
}

bool TickPanel::Enable(bool b)
{
	a1000->Enable(b);
	a100->Enable(b);
	a10->Enable(b);
	a1->Enable(b);
	e10->Enable(b);
	e1->Enable(b);
	t10->Enable(b);
	t1->Enable(b);
	return true;
}

void TickPanel::SetValue(uint64_t value)
{
	if (m_value != value)
	{
		Bars bars;
		bars.set_from_tick(value);
		a1000->SetValue(bars.bars_digit_for_ui(1000));
		a100->SetValue(bars.bars_digit_for_ui(100));
		a10->SetValue(bars.bars_digit_for_ui(10));
		a1->SetValue(bars.bars_digit_for_ui(1));
		e10->SetValue(bars.beats_digit_for_ui(10));
		e1->SetValue(bars.beats_digit_for_ui(1));
		t10->SetValue(bars.tick_digit_for_ui(10));
		t1->SetValue(bars.tick_digit_for_ui(1));
		m_value = value;
	}
}

uint64_t TickPanel::GetValue() const
{
	return m_value;
}

void TickPanel::OnKeyA1000(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_bars_from_ui(1000 * v + 100 * bars.bars_digit_for_ui(100)
			+ 10 * bars.bars_digit_for_ui(10) + bars.bars_digit_for_ui(1)))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyA100(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_bars_from_ui(1000 * bars.bars_digit_for_ui(1000) + 100 * v
			+ 10 * bars.bars_digit_for_ui(10) + bars.bars_digit_for_ui(1)))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyA10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_bars_from_ui(1000 * bars.bars_digit_for_ui(1000) + 100 * bars.bars_digit_for_ui(100)
			+ 10 * v + bars.bars_digit_for_ui(1)))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyA1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_bars_from_ui(1000 * bars.bars_digit_for_ui(1000) + 100 * bars.bars_digit_for_ui(100)
			+ 10 * bars.bars_digit_for_ui(10) + v))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyE10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (v == 0)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_beats_from_ui(10 * v + bars.beats_digit_for_ui(1)))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyE1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (1 <= v && v <= 4)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_beats_from_ui(v))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyT10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v <= 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_tick_from_ui(10 * v + bars.tick_digit_for_ui(1)))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TickPanel::OnKeyT1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v <= 9)
	{
		Bars bars;
		bars.set_from_tick(m_value);
		if (bars.set_tick_from_ui(10 * bars.tick_digit_for_ui(10) + v))
		{
			uint64_t tmp = bars.to_tick();
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TickEvent(TICK_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

