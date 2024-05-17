#pragma once

#include <Simulation/Body.h>
#include <Simulation/Time.h>

namespace Simulation {
	Body LeapfrogIntegrate(Body body, float3 acceleration, Duration timeStep);
}