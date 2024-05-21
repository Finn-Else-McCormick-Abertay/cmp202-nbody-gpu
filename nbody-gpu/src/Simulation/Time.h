#pragma once

#include <string>
#include <compare>
#include <vector>
#include <chrono>

class TimeUnit
{
public:
	enum Unit {
		NANOSECOND,
		MICROSECOND,
		MILLISECOND,
		SECOND,
		MINUTE,
		HOUR,
		DAY,
		YEAR
	};

	TimeUnit(Unit unit);

	bool operator==(const TimeUnit::Unit& rhs) const;
	bool operator==(const TimeUnit& rhs) const;
	std::strong_ordering operator<=>(const TimeUnit& rhs) const;

	TimeUnit& operator=(const Unit&);
	operator Unit() const;
	operator float() const;
	operator std::string() const;

	static float AsSeconds(Unit); float AsSeconds() const;
	static std::string NameSingle(Unit); std::string NameSingle() const;
	static std::string NamePlural(Unit); std::string NamePlural() const;
	static std::string Symbol(Unit); std::string Symbol() const;

	static std::vector<TimeUnit> UnitsLowOrdered();
	static std::vector<TimeUnit> UnitsHighOrdered();

private:
	Unit m_unit;
};


class Duration
{
public:
	Duration(float seconds);
	Duration(std::chrono::nanoseconds);
	Duration();

	std::string AsFormattedString(TimeUnit minUnit = TimeUnit::SECOND, bool symbols = false) const;

	float AsSeconds() const;
	operator float() const;

	Duration& operator=(const Duration&);
	Duration& operator=(const float&);
	Duration& operator+=(const Duration&);
	Duration& operator+=(const float&);

private:
	float m_seconds;
};