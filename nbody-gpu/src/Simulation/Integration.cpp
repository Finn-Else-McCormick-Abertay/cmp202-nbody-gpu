#include "Integration.h"

using namespace Simulation;

Body Simulation::LeapfrogIntegrate(Body body, float3 acceleration, float timeStep) {
	float3 vel_halfStep = body.velocity + acceleration * timeStep;
	float3 pos_fullStep = body.position + vel_halfStep * timeStep;

	body.position = pos_fullStep;
	body.velocity = vel_halfStep;

	return body;
}