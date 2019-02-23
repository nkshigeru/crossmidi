#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include "crossmidi/MidiMessage.hpp"

class Chrono
{
public:

	typedef std::chrono::high_resolution_clock ClockT;

	Chrono();
	~Chrono();

	void start();
	void stop();
	void rewind();

	enum Status
	{
		NONE,
		STOP,
		START,
	};

	class Listener
	{
	public:
		virtual void start(const crossmidi::MidiTimeCode& mtc) = 0;
		virtual void stop(const crossmidi::MidiTimeCode& mtc) = 0;
		virtual void seek(const crossmidi::MidiTimeCode& mtc, uint64_t tick) = 0;
		virtual void quarter_frame(const crossmidi::MidiTimeCode& mtc, int quarter_frame) = 0;
		virtual void clock(uint64_t tick) = 0;
	};
	void set_listener(Listener* listener);

private:
	void run();

	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_con;
	Status m_status;

	crossmidi::MidiTimeCode m_time_code;
	uint64_t m_tick;
	double m_bpm;
	struct {
		bool on;
		crossmidi::MidiTimeCode time_code;
		uint64_t tick;
	} m_seek;

	Listener* m_listener;

};