#pragma once

#include <Simulation/Body.h>
#include <Simulation/Time.h>

#include <sycl/sycl.hpp>

namespace Simulation {
	Body SYCL_EXTERNAL LeapfrogIntegrate(Body body, float3 acceleration, float timeStep);
}