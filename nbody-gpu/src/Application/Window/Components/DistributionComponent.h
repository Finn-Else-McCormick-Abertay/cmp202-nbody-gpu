#pragma once

#include <variant>
#include <random>
#include <RandomDistributions.h>

class DistributionComponent
{
public:
	DistributionComponent(Random::DistributionType = Random::DistributionType::UNIFORM);

	template<typename Distribution>
	DistributionComponent(const Distribution::param_type& params) {
		Set<Distribution>(params);
	}

	Random::distribution_variant& GetVariant();

	Random::DistributionType GetType() const;

	void SetType(Random::DistributionType);

	template<typename Distribution>
	void Set(const Distribution::param_type& params) {
		m_distributionVariant = Random::distribution_variant(Distribution(params));
	}

	void Draw();

private:
	Random::distribution_variant m_distributionVariant;
};