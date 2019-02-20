#pragma once

#include <vector>

namespace crossmidi {

struct MidiNote
{
	uint8_t ch;
	uint8_t note;
	uint8_t velocity;
};

struct MidiCC
{
	uint8_t ch;
	uint8_t number;
	uint8_t value;
};

struct MidiTimeCode
{
	enum FrameRate {
		FRAMERATE_24 = 0,
		FRAMERATE_25 = 1,
		FRAMERATE_29_97 = 2,
		FRAMERATE_30 = 3,
	} rate;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t frame;

	void clear()
	{
		rate = FRAMERATE_24;
		hour = 0;
		minute = 0;
		second = 0;
		frame = 0;
	}
};

struct MidiMessage {
	enum Type {
		NONE,
		NOTE_ON,
		NOTE_OFF,
		CC,
		MTC,
		MTC_FRAME_LSB,
		MTC_FRAME_MSB,
		MTC_SECOND_LSB,
		MTC_SECOND_MSB,
		MTC_MINUTE_LSB,
		MTC_MINUTE_MSB,
		MTC_HOUR_LSB,
		MTC_RATE_HOUR_MSB,
		CLOCK,
		CLOCK_START,
		CLOCK_CONTINUE,
		CLOCK_STOP,
	} type;
	union {
		MidiNote note;
		MidiCC cc;
		MidiTimeCode time_code;
	};

	static MidiMessage FromBytes(const unsigned char* data, size_t len)
	{
		MidiMessage m;
		switch (len)
		{
		case 1:
			switch (data[0])
			{
			case 0xF8:
				m.type = CLOCK;
				break;
			case 0xFA:
				m.type = CLOCK_START;
				break;
			case 0xFB:
				m.type = CLOCK_CONTINUE;
				break;
			case 0xFC:
				m.type = CLOCK_STOP;
				break;
			}
			break;
		case 2:
			switch (data[0])
			{
			case 0xF1: //Quarter frame message
				m.time_code.clear();
				switch (data[1] & 0xF0)
				{
				case 0x00:
					m.type = MTC_FRAME_LSB;
					m.time_code.frame = data[1] & 0x0F;
					break;
				case 0x10:
					m.type = MTC_FRAME_MSB;
					m.time_code.frame = (data[1] & 0x0F) << 4;
					break;
				case 0x20:
					m.type = MTC_SECOND_LSB;
					m.time_code.second = data[1] & 0x0F;
					break;
				case 0x30:
					m.type = MTC_SECOND_MSB;
					m.time_code.second = (data[1] & 0x0F) << 4;
					break;
				case 0x40:
					m.type = MTC_MINUTE_LSB;
					m.time_code.minute = data[1] & 0x0F;
					break;
				case 0x50:
					m.type = MTC_MINUTE_MSB;
					m.time_code.minute = (data[1] & 0x0F) << 4;
					break;
				case 0x60:
					m.type = MTC_HOUR_LSB;
					m.time_code.hour = data[1] & 0x0F;
					break;
				case 0x70:
					m.type = MTC_RATE_HOUR_MSB;
					m.time_code.rate = (MidiTimeCode::FrameRate)((data[1] >> 1) & 0x03);
					m.time_code.hour = (data[1] & 0x01) << 4;
					break;
				}
			}
			break;
		case 3:
			switch (data[0] & 0xF0)
			{
			case 0x80://NoteOff
				m.type = NOTE_OFF;
				m.note.ch = data[0] & 0xF;
				m.note.note = data[1];
				m.note.velocity = data[2];
				break;
			case 0x90://NoteOn
				m.type = NOTE_ON;
				m.note.ch = data[0] & 0xF;
				m.note.note = data[1];
				m.note.velocity = data[2];
				break;
			case 0xB0://CC
				m.type = CC;
				m.note.ch = data[0] & 0xF;
				m.cc.number = data[1];
				m.cc.value = data[2];
				break;
			}
			break;
		case 10:
			//Full time code (F0 7F 7F 01 01 hh mm ss ff F7)
			if (data[0] == 0xF0 && data[1] == 0x7F && data[2] == 0x7F
				&& data[3] == 0x01 && data[4] == 0x01 && data[9] == 0xF7)
			{
				m.type = MTC;
				m.time_code.rate = (MidiTimeCode::FrameRate)((data[5] >> 5) & 0x03);
				m.time_code.hour = data[5] & 0x1F;
				m.time_code.minute = data[6];
				m.time_code.second = data[7];
				m.time_code.frame = data[8];
			}
			break;
		}
		return m;
	}

	bool ToBytes(std::vector<unsigned char>& data) const
	{
		switch (type)
		{
		case MTC_FRAME_LSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x00 | (time_code.frame & 0x0F);
			return true;
		case MTC_FRAME_MSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x10 | ((time_code.frame >> 4) & 0x0F);
			return true;
		case MTC_SECOND_LSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x20 | (time_code.second & 0x0F);
			return true;
		case MTC_SECOND_MSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x30 | ((time_code.second >> 4) & 0x0F);
			return true;
		case MTC_MINUTE_LSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x40 | (time_code.minute & 0x0F);
			return true;
		case MTC_MINUTE_MSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x50 | ((time_code.minute >> 4) & 0x0F);
			return true;
		case MTC_HOUR_LSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x60 | (time_code.hour & 0x0F);
			return true;
		case MTC_RATE_HOUR_MSB:
			data.resize(2);
			data[0] = 0xF1;
			data[1] = 0x70 | ((time_code.rate & 0x03) << 1) | ((time_code.hour >> 4) & 0x01);
			return true;
		case MTC:
			data.resize(10);
			data[0] = 0xF0;
			data[1] = 0x7F;
			data[2] = 0x7F;
			data[3] = 0x01;
			data[4] = 0x01;
			data[5] = ((time_code.rate & 0x03) << 5) | (time_code.hour & 0x1F);
			data[6] = time_code.minute;
			data[7] = time_code.second;
			data[8] = time_code.frame;
			data[9] = 0xF7;
			return true;
		}

		return false;
	}
};

} // namespace crossmidi
