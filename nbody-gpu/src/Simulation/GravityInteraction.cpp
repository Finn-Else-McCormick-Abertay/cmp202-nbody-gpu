#include "GravityInteraction.h"

#include <Simulation/Constants.h>

using namespace Simulation;

// a_i = G * (m_j * r_ij) / (|r_ij|³)

float3 Simulation::BodyBodyInteractionNewtonian(GravityBody b1, GravityBody b2) {
	float3 r = b2.position - b1.position;
	float distCubed = powf(r.length(), 3);

	float3 a = GRAVITATIONAL_CONSTANT * b2.mass * r / distCubed;
	return a;
}


// a_i = G * (m_j * r_ij) / (|r_ij|² + ε²)^³⁄₂

float3 Simulation::BodyBodyInteractionPlummer(GravityBody b1, GravityBody b2, float softening) {
	float3 r = b2.position - b1.position;
	float distCubedSoftened = powf(powf(r.length(), 2) + powf(softening, 2), 3.f/2.f);

	float3 a = GRAVITATIONAL_CONSTANT * b2.mass * r / distCubedSoftened;
	return a;
}