#include "Integration.h"

Body LeapfrogIntegrate(Body body, float3 acceleration, float timeStep) {
	float3 vel_halfStep = body.vel + acceleration * timeStep;
	float3 pos_fullStep = body.pos + vel_halfStep * timeStep;

	body.pos = pos_fullStep;
	body.vel = vel_halfStep;

	return body;
}