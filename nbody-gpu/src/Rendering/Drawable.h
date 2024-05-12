#pragma once

#include <imgui.h>
#include <Rendering/Camera.h>
#include <Rendering/Color.h>

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
		float3 m_worldPos; ImVec2 m_drawPos; float m_size; ImU32 m_color;
	};
}