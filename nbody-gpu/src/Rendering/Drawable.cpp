#include "Drawable.h"

using namespace Drawable;

Point::Point(const float3& pos, float size, ImU32 color) : m_pos(pos), m_size(size), m_color(color) {}

void Point::Project(const Camera& cam, const float2& offset) {
	m_pos = cam.Project(m_pos) + float3(offset, 0.f);
	m_depth = m_pos.z;
}

void Point::Draw(ImDrawList* drawList) {
	drawList->AddCircleFilled(to_imvec(m_pos), m_size, m_color);
}




Line::Line(const float3& p1, const float3& p2, float thickness, ImU32 color) : m_p1(p1), m_p2(p2), m_thickness(thickness), m_color(color) {}

void Line::Project(const Camera& cam, const float2& offset) {
	m_p1 = cam.Project(m_p1) + float3(offset, 0.f);
	m_p2 = cam.Project(m_p2) + float3(offset, 0.f);
	m_depth = (m_p1.z + m_p2.z) / 2.f;
}

void Line::Draw(ImDrawList* drawList) {
	drawList->AddLine(to_imvec(m_p1), to_imvec(m_p2), m_color, m_thickness);
}





BezierCurve::BezierCurve(const std::vector<float3>& points, CurveType type, float thickness, ImU32 color)
	: m_points(points), m_curveType(type), m_thickness(thickness), m_color(color) {}

void BezierCurve::Project(const Camera& cam, const float2& offset) {
	if (m_points.empty()) { return; }

	for (auto& point : m_points) {
		point = cam.Project(point) + float3(offset, 0.f);
	}

	m_depth = m_points.front().z;
}

void BezierCurve::Draw(ImDrawList* drawList) {
	if (m_points.empty()) { return; }

	drawList->PathLineTo(to_imvec(m_points.front()));

	if (m_curveType == QUADRATIC) {
		for (int i = 2; i < m_points.size(); i += 2) {
			drawList->PathBezierQuadraticCurveTo(to_imvec(m_points.at(i - 1)), to_imvec(m_points.at(i)));
		}
	}
	else if (m_curveType == CUBIC) {
		for (int i = 3; i < m_points.size(); i += 3) {
			drawList->PathBezierCubicCurveTo(to_imvec(m_points.at(i - 2)), to_imvec(m_points.at(i - 1)), to_imvec(m_points.at(i)));
		}
	}

	drawList->PathStroke(m_color, ImDrawFlags_None, m_thickness);
}