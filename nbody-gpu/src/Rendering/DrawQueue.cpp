#include "DrawQueue.h"

using namespace Rendering;

DrawQueue::DrawQueue() : m_queue([](const DrawablePtr& lhs, const DrawablePtr& rhs)-> bool { return lhs->Depth() < rhs->Depth(); }) {}

void DrawQueue::SetWindowOffset(const float2& offset) { m_offset = offset; }

void DrawQueue::SetCamera(const Camera& cam) { p_camera = &cam; }

void DrawQueue::Push(DrawablePtr&& ptr) {
	DrawablePtr tempPtr = std::move(ptr);
	if (p_camera) { tempPtr->Project(*p_camera, m_offset); }
	m_queue.emplace(std::move(tempPtr));
}

void DrawQueue::ImGuiRender(ImDrawList* drawList) {
	while (!m_queue.empty()) {
		auto& drawable = *m_queue.top();
		if (drawable.Depth() < 1.f) { drawable.Draw(drawList); }
		m_queue.pop();
	}
}