#include "DistributionComponent.h"

#include <imgui.h>

using namespace Random;

DistributionComponent::DistributionComponent(DistributionType type) {

}

Random::distribution_variant& DistributionComponent::GetVariant() { return m_distributionVariant; }

Random::DistributionType DistributionComponent::GetType() const { return static_cast<DistributionType>(m_distributionVariant.index()); }

void DistributionComponent::SetType(DistributionType type) {

#define DIST_TYPE_SWITCH_LINE(EnumName, TypeName) case DistributionType::EnumName: Set<TypeName>(TypeName::param_type()); break

	switch (type) {
		DIST_TYPE_SWITCH_LINE(UNIFORM, uniform);
		DIST_TYPE_SWITCH_LINE(NORMAL, normal);
		DIST_TYPE_SWITCH_LINE(LOGNORMAL, lognormal);
		DIST_TYPE_SWITCH_LINE(CAUCHY, cauchy);
	}
}

void DistributionComponent::Draw() {
	DistributionType currentType = GetType();

	std::string distributionNames[] = { "Uniform", "Normal", "Lognormal", "Cauchy" };

	ImGui::PushID(this);

	if (ImGui::BeginCombo("Distribution Type", distributionNames[(int)currentType].c_str())) {
		auto drawLine = [&](DistributionType type) {
			bool isSelected = type == currentType;
			if (ImGui::Selectable(distributionNames[(int)type].c_str(), isSelected)) {
				SetType(type);
			}
			if (isSelected) { ImGui::SetItemDefaultFocus(); }
		};

		drawLine(DistributionType::UNIFORM);
		drawLine(DistributionType::NORMAL);
		drawLine(DistributionType::LOGNORMAL);
		drawLine(DistributionType::CAUCHY);

		ImGui::EndCombo();
	}

	currentType = GetType();

	switch (currentType) {
	case DistributionType::UNIFORM: {
		auto& dist = std::get<uniform>(GetVariant());

		float min = dist.a(), max = dist.b();

		bool shouldSet = false;

		shouldSet |= ImGui::InputFloat("Min", &min);
		shouldSet |= ImGui::InputFloat("Max", &max);

		if (shouldSet) {
			dist.param(uniform::param_type(min, max));
		}

	} break;
	case DistributionType::NORMAL: {
		auto& dist = std::get<normal>(GetVariant());

		float mean = dist.mean(), stddev = dist.stddev();

		bool shouldSet = false;

		shouldSet |= ImGui::InputFloat("Mean", &mean);
		shouldSet |= ImGui::InputFloat("Std. Deviation", &stddev);

		if (shouldSet) {
			dist.param(normal::param_type(mean, stddev));
		}

	} break;
	case DistributionType::LOGNORMAL: {
		auto& dist = std::get<lognormal>(GetVariant());

		float mean = dist.m(), stddev = dist.s();

		bool shouldSet = false;

		shouldSet |= ImGui::InputFloat("Mean", &mean);
		shouldSet |= ImGui::InputFloat("Std. Deviation", &stddev);

		if (shouldSet) {
			dist.param(lognormal::param_type(mean, stddev));
		}

	} break;
	case DistributionType::CAUCHY: {
		auto& dist = std::get<cauchy>(GetVariant());

		float peak = dist.a(), scale = dist.b();

		bool shouldSet = false;

		shouldSet |= ImGui::InputFloat("Peak", &peak);
		shouldSet |= ImGui::InputFloat("Scale", &scale);

		if (shouldSet) {
			dist.param(cauchy::param_type(peak, scale));
		}

	} break;
	}

	ImGui::PopID();
}