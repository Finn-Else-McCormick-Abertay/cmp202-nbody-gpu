#pragma once

#include <imgui.h>
#include <Rendering/Camera.h>
#include <Rendering/Color.h>
#include <vector>

namespace Drawable {

	class AbstractDrawable
	{
	public:
		virtual ~AbstractDrawable() = default;

		virtual void Project(const Camera&, const float2& offset) = 0;
		virtual void Draw(ImDrawList*) = 0;

		float Depth() const { return m_depth; }

	protected:
		float m_depth = 0.f;
	};

	class Point : public AbstractDrawable
	{
	public:
		Point(const float3& pos, float size = 1.f, ImU32 color = Color::WHITE);

		virtual void Project(const Camera&, const float2& offset) override;
		virtual void Draw(ImDrawList*) override;

	private:
		float3 m_pos; float m_size; ImU32 m_color;
	};

	class Line : public AbstractDrawable
	{
	public:
		Line(const float3& p1, const float3& p2, float thickness = 1.f, ImU32 color = Color::WHITE);

		virtual void Project(const Camera&, const float2& offset) override;
		virtual void Draw(ImDrawList*) override;

	private:
		float3 m_p1, m_p2; float m_thickness; ImU32 m_color;
	};

	class BezierCurve : public AbstractDrawable
	{
	public:
		enum CurveType { QUADRATIC, CUBIC };

		BezierCurve(const std::vector<float3>& points, CurveType type = CUBIC, float thickness = 1.f, ImU32 color = Color::WHITE);

		virtual void Project(const Camera&, const float2& offset) override;
		virtual void Draw(ImDrawList*) override;

	private:
		std::vector<float3> m_points; CurveType m_curveType;
		float m_thickness; ImU32 m_color;
	};
}