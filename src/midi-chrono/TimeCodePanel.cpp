#include "TimeCodePanel.h"

wxDEFINE_EVENT(TIME_CODE_CHANGED_EVENT, TimeCodeEvent);

enum
{
	ID_ = wxID_HIGHEST,
	ID_DIGIT_H10,
	ID_DIGIT_H1,
	ID_DIGIT_M10,
	ID_DIGIT_M1,
	ID_DIGIT_S10,
	ID_DIGIT_S1,
	ID_DIGIT_F10,
	ID_DIGIT_F1,
};

wxBEGIN_EVENT_TABLE(TimeCodePanel, wxPanel)
	EVT_TEXT(ID_DIGIT_H10, TimeCodePanel::OnKeyH10)
	EVT_TEXT(ID_DIGIT_H1, TimeCodePanel::OnKeyH1)
	EVT_TEXT(ID_DIGIT_M10, TimeCodePanel::OnKeyM10)
	EVT_TEXT(ID_DIGIT_M1, TimeCodePanel::OnKeyM1)
	EVT_TEXT(ID_DIGIT_S10, TimeCodePanel::OnKeyS10)
	EVT_TEXT(ID_DIGIT_S1, TimeCodePanel::OnKeyS1)
	EVT_TEXT(ID_DIGIT_F10, TimeCodePanel::OnKeyF10)
	EVT_TEXT(ID_DIGIT_F1, TimeCodePanel::OnKeyF1)
wxEND_EVENT_TABLE()

TimeCodePanel::TimeCodePanel(wxWindow* parent, wxWindowID id)
	: wxPanel(parent, id)
{
	SetForegroundColour(*wxWHITE);

	h10 = new DigitPanel(this, ID_DIGIT_H10);
	h1 = new DigitPanel(this, ID_DIGIT_H1);
	m10 = new DigitPanel(this, ID_DIGIT_M10);
	m1 = new DigitPanel(this, ID_DIGIT_M1);
	s10 = new DigitPanel(this, ID_DIGIT_S10);
	s1 = new DigitPanel(this, ID_DIGIT_S1);
	f10 = new DigitPanel(this, ID_DIGIT_F10);
	f1 = new DigitPanel(this, ID_DIGIT_F1);

	wxStaticText* label0 = new wxStaticText(this, wxID_ANY, wxT(":"), wxDefaultPosition, h10->GetSize());
	label0->SetFont(h10->GetFont());
	wxStaticText* label1 = new wxStaticText(this, wxID_ANY, wxT(":"), wxDefaultPosition, h10->GetSize());
	label1->SetFont(h10->GetFont());
	wxStaticText* label2 = new wxStaticText(this, wxID_ANY, wxT("."), wxDefaultPosition, h10->GetSize());
	label2->SetFont(h10->GetFont());

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(h10);
	sizer->Add(h1);
	sizer->Add(label0);
	sizer->Add(m10);
	sizer->Add(m1);
	sizer->Add(label1);
	sizer->Add(s10);
	sizer->Add(s1);
	sizer->Add(label2);
	sizer->Add(f10);
	sizer->Add(f1);
	SetSizerAndFit(sizer);

	m_value.clear();
	h10->SetValue(0);
	h1->SetValue(0);
	m10->SetValue(0);
	m1->SetValue(0);
	s10->SetValue(0);
	s1->SetValue(0);
	f10->SetValue(0);
	f1->SetValue(0);
}

TimeCodePanel::~TimeCodePanel()
{
}

void TimeCodePanel::SetValue(const crossmidi::MidiTimeCode& value)
{
	if (m_value.hour != value.hour)
	{
		int hour = std::min(value.hour, (uint8_t)23);
		int hour10 = hour / 10;
		int hour1 = hour - hour10 * 10;
		h10->SetValue(hour10);
		h1->SetValue(hour1);
		m_value.hour = hour;
	}
	if (m_value.minute != value.minute)
	{
		int minute = std::min(value.minute, (uint8_t)59);
		int minute10 = minute / 10;
		int minute1 = minute - minute10 * 10;
		m10->SetValue(minute10);
		m1->SetValue(minute1);
		m_value.minute = minute;
	}
	if (m_value.second != value.second)
	{
		int second = std::min(value.second, (uint8_t)59);
		int second10 = second / 10;
		int second1 = second - second10 * 10;
		s10->SetValue(second10);
		s1->SetValue(second1);
		m_value.second = second;
	}
	if (m_value.frame != value.frame)
	{
		int frame = std::min(value.frame, (uint8_t)24);
		int frame10 = frame / 10;
		int frame1 = frame - frame10 * 10;
		f10->SetValue(frame10);
		f1->SetValue(frame1);
		m_value.frame = frame;
	}
}

crossmidi::MidiTimeCode TimeCodePanel::GetValue() const
{
	return m_value;
}

void TimeCodePanel::OnKeyH10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v <= 2)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.hour = v * 10 + h1->GetValue();
		SetValue(tmp);
		NavigateIn();
		wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
	}
}

void TimeCodePanel::OnKeyH1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.hour = h10->GetValue() * 10 + v;
		if (tmp.hour < 24)
		{
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TimeCodePanel::OnKeyM10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 6)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.minute = v * 10 + m1->GetValue();
		SetValue(tmp);
		NavigateIn();
		wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
	}
}

void TimeCodePanel::OnKeyM1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.minute = m10->GetValue() * 10 + v;
		SetValue(tmp);
		NavigateIn();
		wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
	}
}

void TimeCodePanel::OnKeyS10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 6)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.second = v * 10 + s1->GetValue();
		SetValue(tmp);
		NavigateIn();
		wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
	}
}

void TimeCodePanel::OnKeyS1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.second = s10->GetValue() * 10 + v;
		SetValue(tmp);
		NavigateIn();
		wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
	}
}

void TimeCodePanel::OnKeyF10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v <= 2)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.frame = v * 10 + f1->GetValue();
		if (tmp.frame < 25)
		{
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

void TimeCodePanel::OnKeyF1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		crossmidi::MidiTimeCode tmp = m_value;
		tmp.frame = f10->GetValue() * 10 + v;
		if (tmp.frame < 25)
		{
			SetValue(tmp);
			NavigateIn();
			wxPostEvent(this, TimeCodeEvent(TIME_CODE_CHANGED_EVENT, GetId(), tmp));
		}
	}
}

