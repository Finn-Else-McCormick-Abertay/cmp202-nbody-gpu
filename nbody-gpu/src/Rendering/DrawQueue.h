#pragma once

#include <queue>
#include <memory>
#include <functional>

#include <Rendering/Drawable.h>

class DrawQueue
{
public:
	DrawQueue();

	using DrawablePtr = std::unique_ptr<Drawable::AbstractDrawable>;
	void Push(DrawablePtr&&);

	void SetWindowOffset(const float2&);
	void SetCamera(const Camera&);
	void ImGuiRender(ImDrawList*);

private:
	const Camera* p_camera = nullptr; float2 m_offset = float2();
	std::priority_queue<DrawablePtr, std::vector<DrawablePtr>, std::function<bool(const DrawablePtr&, const DrawablePtr&)>> m_queue;
};