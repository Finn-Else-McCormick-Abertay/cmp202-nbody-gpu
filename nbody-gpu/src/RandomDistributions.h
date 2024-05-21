#pragma once

#include <random>
#include <variant>

namespace Random
{
	using RandomEngine = std::mt19937;

	using uniform = std::uniform_real_distribution<float>;
	using normal = std::normal_distribution<float>;
	using lognormal = std::lognormal_distribution<float>;
	using cauchy = std::cauchy_distribution<float>;

	using distribution_variant = std::variant<uniform, normal, lognormal, cauchy>;
	enum class DistributionType {
		UNIFORM = 0,
		NORMAL = 1,
		LOGNORMAL = 2,
		CAUCHY = 3
	};
}
