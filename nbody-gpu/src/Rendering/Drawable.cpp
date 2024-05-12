#include "Drawable.h"

using namespace Drawable;

Point::Point(const float3& pos, float size, ImU32 color) : m_worldPos(pos), m_drawPos(), m_size(size), m_color(color) {}

void Point::Project(const Camera& cam, const float2& offset) {
	float3 projectedPoint = cam.Project(m_worldPos);
	m_drawPos = to_imvec(float2(projectedPoint) + offset);
	m_depth = projectedPoint.z;
}

void Point::Draw(ImDrawList* drawList) {
	drawList->AddCircleFilled(m_drawPos, m_size, m_color);
}