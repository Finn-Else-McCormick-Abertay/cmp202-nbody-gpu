#include "GravityInteraction.h"

#include <Simulation/Contants.h>

// a_i = G * (m_j * r_ij) / (|r_ij|³)

float3 BodyBodyInteractionNewtonian(Body b1, Body b2) {
	float3 r = b2.pos - b1.pos;
	float distCubed = powf(r.length(), 3);

	float3 a = GRAVITATIONAL_CONSTANT * b2.mass * r / distCubed;
	return a;
}


// a_i = G * (m_j * r_ij) / (|r_ij|² + ε²)^³⁄₂

float3 BodyBodyInteractionPlummer(Body b1, Body b2, float softening) {
	float3 r = b2.pos - b1.pos;
	float distCubedSoftened = powf(powf(r.length(), 2) + powf(softening, 2), 3.f/2.f);

	float3 a = GRAVITATIONAL_CONSTANT * b2.mass * r / distCubedSoftened;
	return a;
}