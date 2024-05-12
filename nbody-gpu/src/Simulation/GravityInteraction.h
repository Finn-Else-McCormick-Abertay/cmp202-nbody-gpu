﻿#pragma once

#include <MathsTypes.h>
#include <Simulation/Body.h>


/// <summary>Gravitational interaction between two newtonian point masses
/// <para>a_i = G * (m_j * r_ij) / (|r_ij|³) </para>
/// </summary>
/// <param name="focusBody">i</param>
/// <param name="interactBody">j</param>
/// <returns>Acceleration on body i</returns>
float3 BodyBodyInteractionNewtonian(Body focusBody, Body interactBody);


/// <summary>Gravitational interaction between two plummer point masses
/// <para>a_i = G * (m_j * r_ij) / (|r_ij|² + ε²)^³⁄₂ </para>
/// </summary>
/// <param name="focusBody">i</param>
/// <param name="interactBody">j</param>
/// <param name="softening">ε</param>
/// <returns>Acceleration on body i</returns>
float3 BodyBodyInteractionPlummer(Body focusBody, Body interactBody, float softening);