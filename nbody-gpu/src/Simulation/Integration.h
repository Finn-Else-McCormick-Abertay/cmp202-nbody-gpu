#pragma once

#include <Simulation/Body.h>

Body LeapfrogIntegrate(Body body, float3 acceleration, float timeStep);