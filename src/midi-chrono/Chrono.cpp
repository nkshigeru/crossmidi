#include "Chrono.h"
#include <iostream>

Chrono::Chrono()
{
	m_status = STOP;
	m_time_code.clear();
	m_time_code.rate = crossmidi::MidiTimeCode::FRAMERATE_25;
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
	m_con.notify_all();
}

void Chrono::set_callback(Callback callback)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_callback = callback;
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
			if (m_callback)
			{
				m_callback(m_time_code, -1);
			}
			continue;
		}

		ClockT::time_point start_tp = ClockT::now();
		ClockT::time_point next_quarter_tp;
		crossmidi::MidiTimeCode temp_mtc = m_time_code;
		if (m_callback)
		{
			m_callback(m_time_code, -1);
		}
		size_t quarter_count = 0;
		if (m_callback)
		{
			m_callback(m_time_code, quarter_count);
		}
		quarter_count = 1;
		std::chrono::duration<int, std::ratio<1, 25 * 4> > quarter_time(1);
		next_quarter_tp = start_tp + quarter_time;

		// started
		while (m_status == START)
		{
			lock.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			lock.lock();
			if (next_quarter_tp < ClockT::now())
			{
				if (m_callback)
				{
					m_callback(m_time_code, quarter_count);
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

		if (m_status == NONE)
		{
			// end of thread
			break;
		}
		else if (m_status == STOP)
		{
			// stopped
			m_time_code = temp_mtc;
			if (m_callback)
			{
				m_callback(m_time_code, -1);
			}
		}
	}
}
