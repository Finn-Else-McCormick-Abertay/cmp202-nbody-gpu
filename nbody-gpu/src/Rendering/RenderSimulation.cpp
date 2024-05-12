#include "RenderSimulation.h"

#include <imgui.h>
#include <string>

void RenderSimulation(const SimulationWorld& world, DrawQueue& drawQueue) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	/*
	auto project = [&](float3 point) -> std::pair<ImVec2,float> {
		float3 projectedPos = cam.Project(point, windowSize);
		return std::make_pair(ImVec2(projectedPos.x + windowPos.x, projectedPos.y + windowPos.y), projectedPos.z);
		};

	auto drawAxis = [&](float3 axis, ImColor color, float size = 100.f) {
		auto [p1, d1] = project(axis * size);
		auto [p2, d2] = project(axis * -size);

		if (d1 < 1.f && d2 < 1.f) {
			drawList->AddLine(p1, p2, color, 3.f);
		}
		};

	drawAxis(X_AXIS, ImColor(1.f, 0.f, 0.f));
	drawAxis(Y_AXIS, ImColor(0.f, 1.f, 0.f));
	drawAxis(Z_AXIS, ImColor(0.f, 0.f, 1.f));
	*/

	for (int i = -256; i < 256; ++i) {
		drawQueue.Push(std::make_unique<Drawable::Point>(float3(i * 1.f, i * 1.f, i * 1.f), 5.f, Color::GREEN));
	}

	for (auto it = world.cbegin(); it != world.cend(); ++it) {
		const Body& body = *it;

		drawQueue.Push(std::make_unique<Drawable::Point>(body.pos, 5.f, Color::RED));
	}
}