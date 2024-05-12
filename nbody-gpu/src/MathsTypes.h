#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>
#include <imgui.h>

#define GLM_FORCE_LEFT_HANDED

using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;

using mat4x4 = glm::mat4x4;

using quat = glm::quat;

#define X_AXIS float3(1.f, 0.f, 0.f)
#define Y_AXIS float3(0.f, 1.f, 0.f)
#define Z_AXIS float3(0.f, 0.f, 1.f)

inline static ImVec2 to_imvec(const float2& v) { return ImVec2(v.x, v.y); }
inline static float2 to_float2(const ImVec2& v) { return float2(v.x, v.y); }