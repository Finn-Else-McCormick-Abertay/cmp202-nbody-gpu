#include "Time.h"

#include <sstream>
#include <iomanip>
#include <map>

TimeUnit::TimeUnit(Unit unit) : m_unit(unit) {}

TimeUnit& TimeUnit::operator=(const Unit& rhs) { m_unit = rhs; return *this; }
TimeUnit::operator Unit() const { return m_unit; }

TimeUnit::operator float() const { return AsSeconds(); }
TimeUnit::operator std::string() const { return NameSingle(); }


bool TimeUnit::operator==(const TimeUnit::Unit& rhs) const { return m_unit == rhs; }
bool TimeUnit::operator==(const TimeUnit& rhs) const { return m_unit == rhs.m_unit; }

std::strong_ordering TimeUnit::operator<=>(const TimeUnit& rhs) const {
	return static_cast<int>(m_unit) <=> static_cast<int>(rhs.m_unit);
}

float TimeUnit::AsSeconds(Unit unit) {
	switch (unit) {
	case NANOSECOND:	return 1.e-9f;
	case MICROSECOND:	return 1.e-6f;
	case MILLISECOND:	return 0.001f;
	case SECOND: return 1.f;
	case MINUTE: return 60.f;
	case HOUR:	 return 60.f * AsSeconds(MINUTE);
	case DAY:	 return 24.f * AsSeconds(HOUR);
	case YEAR:	 return 365.f * AsSeconds(DAY);
	}
}
float TimeUnit::AsSeconds() const { return AsSeconds(m_unit); }

std::string TimeUnit::NameSingle(Unit unit) {
	switch (unit) {
	case NANOSECOND:	return "nanosecond";
	case MICROSECOND:	return "microsecond";
	case MILLISECOND:	return "millisecond";
	case SECOND: return "second";
	case MINUTE: return "minute";
	case HOUR:	 return "hour";
	case DAY:	 return "day";
	case YEAR:	 return "year";
	}
}
std::string TimeUnit::NameSingle() const { return NameSingle(m_unit); }

std::string TimeUnit::NamePlural(Unit unit) {
	switch (unit) {
	case NANOSECOND:	return "nanoseconds";
	case MICROSECOND:	return "microseconds";
	case MILLISECOND:	return "milliseconds";
	case SECOND: return "seconds";
	case MINUTE: return "minutes";
	case HOUR:	 return "hours";
	case DAY:	 return "days";
	case YEAR:	 return "years";
	}
}
std::string TimeUnit::NamePlural() const { return NamePlural(m_unit); }

std::string TimeUnit::Symbol(Unit unit) {
	switch (unit) {
	case NANOSECOND:	return "ns";
	case MICROSECOND:	return "us";
	case MILLISECOND:	return "ms";
	case SECOND: return "s";
	case MINUTE: return "m";
	case HOUR:	 return "h";
	case DAY:	 return "d";
	case YEAR:	 return "y";
	}
}
std::string TimeUnit::Symbol() const { return Symbol(m_unit); }

std::vector<TimeUnit> TimeUnit::UnitsLowOrdered() {
	return std::vector<TimeUnit>{
		TimeUnit(NANOSECOND),
		TimeUnit(MICROSECOND),
		TimeUnit(MILLISECOND),
		TimeUnit(SECOND),
		TimeUnit(MINUTE),
		TimeUnit(HOUR),
		TimeUnit(DAY),
		TimeUnit(YEAR)
	};
}

std::vector<TimeUnit> TimeUnit::UnitsHighOrdered() {
	auto units = UnitsLowOrdered(); std::reverse(units.begin(), units.end());
	return units;
}



Duration::Duration(float seconds) : m_seconds(seconds) {  }
Duration::Duration() : m_seconds(0.f) {}

float Duration::AsSeconds() const { return m_seconds; }
Duration::operator float() const { return AsSeconds(); }

Duration& Duration::operator=(const Duration& rhs) { m_seconds = rhs.m_seconds; return *this; }
Duration& Duration::operator=(const float& rhs) { m_seconds = rhs; return *this; }
Duration& Duration::operator+=(const Duration& rhs) { m_seconds += rhs.m_seconds; return *this; }
Duration& Duration::operator+=(const float& rhs) { m_seconds += rhs; return *this; }

std::string Duration::AsFormattedString(TimeUnit minUnit, bool symbols) const {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);

	std::map<TimeUnit, float> splitDuration;
	splitDuration[TimeUnit::SECOND] = AsSeconds();

	auto performSplit = [&](TimeUnit unit) {
		if (unit < minUnit || unit == TimeUnit::SECOND) { return; }
		if (splitDuration[TimeUnit::SECOND] >= (float)unit) {
			float remainingUnit = splitDuration[TimeUnit::SECOND] / (float)unit;
			float intPart;
			modf(remainingUnit, &intPart);

			splitDuration[unit] = intPart;
			splitDuration[TimeUnit::SECOND] -= splitDuration[unit] * (float)unit;
		}
	};

	auto writeUnit = [&](TimeUnit unit, bool* isFirst) {
		float val = splitDuration[unit];
		if (val == 0.f || unit < minUnit) { return; }

		if (!(*isFirst)) { ss << ' '; }
		(*isFirst) = false;

		bool showFrac = false;

		float intPart, fracPart;
		fracPart = modf(val, &intPart);
		if (fracPart > 0.001f) { showFrac = true; }

		if (showFrac) { ss << val; } else { ss << static_cast<int>(val); }
		ss << ' ';

		if (symbols) { ss << unit.Symbol(); }
		else {
			if (fabs(val - 1.f) < 0.01f) { ss << unit.NameSingle(); }
			else { ss << unit.NamePlural(); }
		}
	};

	auto allUnits = TimeUnit::UnitsHighOrdered();
	for (auto& unit : allUnits) { performSplit(unit); }

	bool isFirst = true;
	for (auto& unit : allUnits) { writeUnit(unit, &isFirst); }

	return ss.str();
}