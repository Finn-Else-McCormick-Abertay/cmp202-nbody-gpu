#pragma once

#include <MathsTypes.h>

namespace Simulation {

	struct GravityBody
	{
		float mass;
		float3 position;
	};

	struct Body
	{
		float mass;
		float3 position;
		float3 velocity;

		operator GravityBody() const { return GravityBody{ mass, position }; }
	};

}