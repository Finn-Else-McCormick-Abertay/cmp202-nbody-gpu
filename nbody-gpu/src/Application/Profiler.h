#pragma once
#pragma once

#include <chrono>
#include <Simulation/Time.h>

class Profiler
{
public:
	Profiler() = default;

	void StartTimer();
	void StopTimer();

	const std::vector<std::chrono::nanoseconds>& Times() const;

	int Count() const;

	std::chrono::nanoseconds Mean() const;
	std::chrono::nanoseconds Median() const;

private:
	std::vector<std::chrono::nanoseconds> m_tickTimes;

	std::chrono::steady_clock::time_point m_startPoint;
	bool m_isTiming = false;
};