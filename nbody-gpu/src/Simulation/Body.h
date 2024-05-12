#pragma once

#include <MathsTypes.h>

struct Body
{
	float3 pos;
	float mass;

	Body(float3 pos, float mass) : pos(pos), mass(mass) {}
	Body() = default;
	Body(const Body&) = default;
	Body(Body&) = default;
};