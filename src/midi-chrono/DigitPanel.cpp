#include "DigitPanel.h"

wxBEGIN_EVENT_TABLE(DigitPanel, wxWindow)
	EVT_PAINT(DigitPanel::OnPaint)
	EVT_SET_FOCUS(DigitPanel::OnSetFocus)
	EVT_KILL_FOCUS(DigitPanel::OnKillFocus)
	EVT_CHAR(DigitPanel::OnChar)
wxEND_EVENT_TABLE()

static const wxColour BackgroundColor(60, 60, 60);
static const wxColour ForegroundColor(255, 255, 255);
static const wxColour FocusColor(39, 179, 238);

DigitPanel::DigitPanel(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxDefaultPosition, parent->FromDIP(wxSize(15, 30))), m_value(-1)
{
	wxFont display_font(FromDIP(wxSize(15, 30)), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	SetFont(display_font);
}

DigitPanel::~DigitPanel()
{
}

void DigitPanel::SetValue(int value)
{
	if (m_value != value)
	{
		m_value = value;
		Refresh();
	}
}

int DigitPanel::GetValue() const
{
	return m_value;
}

void DigitPanel::OnPaint(wxPaintEvent&)
{
	wxPaintDC dc(this);

	wxSize size(dc.GetSize());
	if (HasFocus())
	{
		dc.SetPen(wxPen(FocusColor, 1));
	}
	else
	{
		dc.SetPen(*wxTRANSPARENT_PEN);
	}
	dc.SetBrush(BackgroundColor);
	dc.DrawRectangle(size);

	dc.SetTextForeground(*wxWHITE);
	if (0 <= m_value && m_value < 10)
	{
		dc.DrawLabel(wxString::Format(wxT("%d"), m_value), size, wxALIGN_CENTER);
	}
}

void DigitPanel::OnSetFocus(wxFocusEvent& evt)
{
	Refresh(true);
	evt.Skip();
}

void DigitPanel::OnKillFocus(wxFocusEvent& evt)
{
	Refresh(true);
	evt.Skip();
}

void DigitPanel::OnChar(wxKeyEvent& evt)
{
	wxChar c = evt.GetUnicodeKey();
	int v;
	switch (c)
	{
	case L'0':
		v = 0;
		break;
	case L'1':
		v = 1;
		break;
	case L'2':
		v = 2;
		break;
	case L'3':
		v = 3;
		break;
	case L'4':
		v = 4;
		break;
	case L'5':
		v = 5;
		break;
	case L'6':
		v = 6;
		break;
	case L'7':
		v = 7;
		break;
	case L'8':
		v = 8;
		break;
	case L'9':
		v = 9;
		break;
	default:
		v = -1;
	}
	if (v >= 0)
	{
		wxCommandEvent cmd(wxEVT_TEXT, GetId());
		cmd.SetString(c);
		cmd.SetInt(v);
		wxPostEvent(this, cmd);
	}
}
