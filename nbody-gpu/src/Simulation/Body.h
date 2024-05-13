#pragma once

#include <MathsTypes.h>

struct Body
{
	float3 pos, vel;
	float mass;

	Body(float3 pos, float3 vel, float mass) : pos(pos), vel(vel), mass(mass) {}
	Body() = default;
	Body(const Body&) = default;
	Body(Body&) = default;
};