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

	typedef std::function<void(const crossmidi::MidiTimeCode& mtc, int quarter_frame)> Callback;
	void set_callback(Callback callback);

private:
	void run();

	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_con;
	Status m_status;

	crossmidi::MidiTimeCode m_time_code;
	Callback m_callback;

};