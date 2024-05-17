#include "Integration.h"

using namespace Simulation;

Body Simulation::LeapfrogIntegrate(Body body, float3 acceleration, Duration timeStep) {
	float3 vel_halfStep = body.velocity + acceleration * static_cast<float>(timeStep);
	float3 pos_fullStep = body.position + vel_halfStep * static_cast<float>(timeStep);

	body.position = pos_fullStep;
	body.velocity = vel_halfStep;

	return body;
}