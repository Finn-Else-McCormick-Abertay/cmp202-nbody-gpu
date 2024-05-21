#include "Profiler.h"

#include <queue>

void Profiler::StartTimer() {
	if (m_isTiming) { StopTimer(); }

	m_startPoint = std::chrono::steady_clock::now();
	m_isTiming = true;
}

void Profiler::StopTimer() {
	auto endPoint = std::chrono::steady_clock::now();
	std::chrono::nanoseconds duration = endPoint - m_startPoint;

	m_tickTimes.push_back(duration);

	m_isTiming = false;
}

const std::vector<std::chrono::nanoseconds>& Profiler::Times() const { return m_tickTimes; }

int Profiler::Count() const { return m_tickTimes.size(); }

std::chrono::nanoseconds Profiler::Mean() const {
	if (m_tickTimes.empty()) { return std::chrono::nanoseconds(); }

	std::chrono::nanoseconds sumTime = std::chrono::nanoseconds(0);
	for (auto& time : m_tickTimes) { sumTime += time; }
	return sumTime / m_tickTimes.size();
}

std::chrono::nanoseconds Profiler::Median() const {
	if (m_tickTimes.empty()) { return std::chrono::nanoseconds(); }

	std::priority_queue<std::chrono::nanoseconds> queue;
	for (auto& time : m_tickTimes) { queue.push(time); }
	std::vector<std::chrono::nanoseconds> orderedTimes;
	while (!queue.empty()) { orderedTimes.push_back(queue.top()); queue.pop(); }

	auto numValues = orderedTimes.size();
	if (numValues % 2 == 0) {
		std::chrono::nanoseconds below = orderedTimes.at(numValues / 2), above = orderedTimes.at((numValues / 2) + 1);
		return below + ((above - below) / 2);
	}
	else {
		return orderedTimes.at((numValues / 2) + 1);
	}
}