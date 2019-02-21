#include <gtest/gtest.h>
#include "crossmidi/MidiMessage.hpp"

using namespace crossmidi;

TEST(MidiMessage, none) {
	std::vector<unsigned char> data1;
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::NONE);

	std::vector<unsigned char> data2;
	EXPECT_FALSE(m.ToBytes(data2));
}

TEST(MidiMessage, noteon) {
	std::vector<unsigned char> data1 = { 0x91, 0x02, 0x03 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::NOTE_ON);
    EXPECT_EQ(m.note.ch, 1);
    EXPECT_EQ(m.note.note, 2);
    EXPECT_EQ(m.note.velocity, 3);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, noteoff) {
	std::vector<unsigned char> data1 = { 0x81, 0x02, 0x03 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::NOTE_OFF);
    EXPECT_EQ(m.note.ch, 1);
    EXPECT_EQ(m.note.note, 2);
    EXPECT_EQ(m.note.velocity, 3);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, cc) {
	std::vector<unsigned char> data1 = { 0xB1, 0x02, 0x03 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::CC);
    EXPECT_EQ(m.cc.ch, 1);
    EXPECT_EQ(m.cc.number, 2);
    EXPECT_EQ(m.cc.value, 3);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, mtc) {
	std::vector<unsigned char> data1 = { 0xF0, 0x7F, 0x7F, 0x01, 0x01, 0x22, 0x03, 0x04, 0x05, 0xF7 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC);
    EXPECT_EQ(m.time_code.rate, 1);
    EXPECT_EQ(m.time_code.hour, 2);
    EXPECT_EQ(m.time_code.minute, 3);
    EXPECT_EQ(m.time_code.second, 4);
    EXPECT_EQ(m.time_code.frame, 5);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_0) {
	std::vector<unsigned char> data1 = { 0xF1, 0x01 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_FRAME_LSB);
    EXPECT_EQ(m.time_code.frame, 0x01);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_1) {
	std::vector<unsigned char> data1 = { 0xF1, 0x12 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_FRAME_MSB);
    EXPECT_EQ(m.time_code.frame, 0x20);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_2) {
	std::vector<unsigned char> data1 = { 0xF1, 0x23 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_SECOND_LSB);
    EXPECT_EQ(m.time_code.second, 0x03);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_3) {
	std::vector<unsigned char> data1 = { 0xF1, 0x34 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_SECOND_MSB);
    EXPECT_EQ(m.time_code.second, 0x40);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_4) {
	std::vector<unsigned char> data1 = { 0xF1, 0x45 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_MINUTE_LSB);
    EXPECT_EQ(m.time_code.minute, 0x05);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_5) {
	std::vector<unsigned char> data1 = { 0xF1, 0x56 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_MINUTE_MSB);
    EXPECT_EQ(m.time_code.minute, 0x60);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_6) {
	std::vector<unsigned char> data1 = { 0xF1, 0x67 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_HOUR_LSB);
    EXPECT_EQ(m.time_code.hour, 0x07);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, quarter_7) {
	std::vector<unsigned char> data1 = { 0xF1, 0x73 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::MTC_RATE_HOUR_MSB);
    EXPECT_EQ(m.time_code.rate, 1);
    EXPECT_EQ(m.time_code.hour, 0x10);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, clock) {
	std::vector<unsigned char> data1 = { 0xF8 };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::CLOCK);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, clock_start) {
	std::vector<unsigned char> data1 = { 0xFA };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::CLOCK_START);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, clock_continue) {
	std::vector<unsigned char> data1 = { 0xFB };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::CLOCK_CONTINUE);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}

TEST(MidiMessage, clock_stop) {
	std::vector<unsigned char> data1 = { 0xFC };
	MidiMessage m = MidiMessage::FromBytes(data1);
    EXPECT_EQ(m.type, MidiMessage::CLOCK_STOP);

	std::vector<unsigned char> data2;
	EXPECT_TRUE(m.ToBytes(data2));
	EXPECT_EQ(data1, data2);
}
