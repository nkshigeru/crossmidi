#include "BPMPanel.h"

enum
{
	ID_ = wxID_HIGHEST,
	ID_DIGIT_100,
	ID_DIGIT_10,
	ID_DIGIT_1,
};

wxBEGIN_EVENT_TABLE(BPMPanel, wxPanel)
	EVT_TEXT(ID_DIGIT_100, BPMPanel::OnKey100)
	EVT_TEXT(ID_DIGIT_10, BPMPanel::OnKey10)
	EVT_TEXT(ID_DIGIT_1, BPMPanel::OnKey1)
wxEND_EVENT_TABLE()

BPMPanel::BPMPanel(wxWindow* parent, wxWindowID id)
	: wxPanel(parent, id), m_value(-1)
{
	d100 = new DigitPanel(this, ID_DIGIT_100);
	d10 = new DigitPanel(this, ID_DIGIT_10);
	d1 = new DigitPanel(this, ID_DIGIT_1);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(d100);
	sizer->Add(d10);
	sizer->Add(d1);
	SetSizerAndFit(sizer);
}

BPMPanel::~BPMPanel()
{
}

void BPMPanel::SetValue(int value)
{
	if (m_value != value)
	{
		m_value = value;
		int v100 = value / 100;
		value -= v100 * 100;
		int v10 = value / 10;
		value -= v10 * 10;
		int v1 = value;
		d100->SetValue(v100);
		d10->SetValue(v10);
		d1->SetValue(v1);
	}
}

int BPMPanel::GetValue() const
{
	return m_value;
}

void BPMPanel::OnKey100(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		int tmp = v * 100 + d10->GetValue() * 10 + d1->GetValue();
		SetValue(tmp);
		NavigateIn();
	}
}

void BPMPanel::OnKey10(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		int tmp = d100->GetValue() * 100 + v * 10 + d1->GetValue();
		SetValue(tmp);
		NavigateIn();
	}
}

void BPMPanel::OnKey1(wxCommandEvent& evt)
{
	int v = evt.GetInt();
	if (0 <= v && v < 10)
	{
		int tmp = d100->GetValue() * 100 + d10->GetValue() * 10 + v;
		SetValue(tmp);
		Navigate();
	}
}
