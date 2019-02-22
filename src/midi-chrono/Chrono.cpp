#include "Chrono.h"
#include <iostream>

Chrono::Chrono() : m_listener(nullptr)
{
	m_status = STOP;
	m_time_code.clear();
	m_time_code.rate = crossmidi::MidiTimeCode::FRAMERATE_25;
	m_tick = 0;
	m_bpm = 120;
	m_thread = std::thread(&Chrono::run, this);
}

Chrono::~Chrono()
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_status = NONE;
		m_con.notify_all();
	}
	m_thread.join();
}

void Chrono::start()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_status != STOP)
	{
		return;
	}
	m_status = START;
	m_con.notify_all();
}

void Chrono::stop()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_status != START)
	{
		return;
	}
	m_status = STOP;
	m_con.notify_all();
}

void Chrono::rewind()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_status != STOP)
	{
		return;
	}
	m_time_code.clear();
	m_time_code.rate = crossmidi::MidiTimeCode::FRAMERATE_25;
	m_tick = 0;
	m_con.notify_all();
}

void Chrono::set_listener(Listener* listener)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_listener = listener;
}

void Chrono::run()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_status == STOP)
		{
			m_con.wait(lock);
		}
		if (m_status == NONE)
		{
			// end of thread
			break;
		}
		else if (m_status == STOP)
		{
			if (m_listener)
			{
				m_listener->seek(m_time_code, m_tick);
			}
			continue;
		}

		ClockT::time_point start_tp = ClockT::now();
		ClockT::time_point next_quarter_tp;
		ClockT::time_point next_clock_tp;
		crossmidi::MidiTimeCode temp_mtc = m_time_code;

		uint64_t start_tick = m_tick;
		uint64_t tick = 0;

		if (m_listener)
		{
			m_listener->start(m_time_code);
		}
		size_t quarter_count = 0;
		if (m_listener)
		{
			m_listener->quarter_frame(m_time_code, quarter_count);
		}
		quarter_count = 1;
		std::chrono::duration<int, std::ratio<1, 25 * 4> > quarter_time(1);
		next_quarter_tp = start_tp + quarter_time;

		tick = 1;
		std::chrono::duration<double> tick_time(60.0 / 24 / m_bpm);
		next_clock_tp = start_tp + std::chrono::duration_cast<ClockT::duration>(tick_time);

		// started
		while (m_status == START)
		{
			lock.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			lock.lock();
			if (next_quarter_tp < next_clock_tp)
			{
				if (next_quarter_tp < ClockT::now())
				{
					if (m_listener)
					{
						m_listener->quarter_frame(m_time_code, quarter_count);
					}
					++quarter_count;
					++quarter_time;
					next_quarter_tp = start_tp + quarter_time;
					if (quarter_count == 4 || quarter_count == 8)
					{
						++temp_mtc.frame;
						if (temp_mtc.frame >= 25)
						{
							temp_mtc.frame = 0;
							++temp_mtc.second;
							if (temp_mtc.second >= 60)
							{
								temp_mtc.second = 0;
								++temp_mtc.minute;
								if (temp_mtc.minute >= 60)
								{
									++temp_mtc.hour;
								}
							}
						}
					}
					if (quarter_count >= 8)
					{
						quarter_count = 0;
						m_time_code = temp_mtc;
					}
				}
			}
			else
			{
				if (next_clock_tp < ClockT::now())
				{
					m_tick = start_tick + tick;
					if (m_listener)
					{
						m_listener->clock(m_tick);
					}
					++tick;
					std::chrono::duration<double> tick_time(60.0 / 24 * tick / m_bpm);
					next_clock_tp = start_tp + std::chrono::duration_cast<ClockT::duration>(tick_time);
				}
			}
		}

		if (m_status == NONE)
		{
			// end of thread
			break;
		}
		else if (m_status == STOP)
		{
			// stopped
			m_time_code = temp_mtc;
			if (m_listener)
			{
				m_listener->stop(m_time_code);
			}
		}
	}
}
