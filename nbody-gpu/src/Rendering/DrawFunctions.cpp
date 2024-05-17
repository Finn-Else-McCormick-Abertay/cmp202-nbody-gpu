#include "DrawFunctions.h"

#include <imgui.h>
#include <string>
#include <glm/gtx/quaternion.hpp>

#define X_COLOR Color::RED
#define Y_COLOR Color::GREEN
#define Z_COLOR Color::BLUE

using namespace Rendering;

void Rendering::DrawAxes(DrawQueue& q) {
	auto drawAxis = [&](float3 axis, ImColor color, float size = 2.f, int count = 50) {
		float3 posAxis = axis * size, negAxis = axis * -size;
		for (int i = 0; i < count; ++i) {
			DRAW(q, Line, posAxis * (float)i, posAxis * (float)(i + 1), 3.f, color);
			DRAW(q, Line, negAxis * (float)i, negAxis * (float)(i + 1), 3.f, color);
		}
		};

	drawAxis(X_AXIS, X_COLOR);
	drawAxis(Y_AXIS, Y_COLOR);
	drawAxis(Z_AXIS, Z_COLOR);
}

void Rendering::DrawGimbal(DrawQueue& q) {

	float thickness = 3.f;

	auto drawCircle = [&](quat rotation, ImU32 color, float scale = 1.f) {
		const float c = (4.f / 3.f) * (sqrtf(2.f) - 1.f);

		const std::vector<float3> points = { { 0.f, 1.f, 0.f }, { c, 1.f, 0.f }, { 1.f, c, 0.f }, { 1.f, 0.f, 0.f } };

		for (int i = 0; i < 4; ++i) {
			mat4x4 segmentTrans = glm::toMat4(glm::angleAxis(i * glm::radians(90.f), Z_AXIS) * rotation) * glm::scale(mat4x4(1.f), float3(scale, scale, scale));
			std::vector<float3> segmentPoints = points;
			for (auto& point : segmentPoints) { point = float4(point, 1.f) * segmentTrans; }
			DRAW(q, BezierCurve, segmentPoints, Drawable::BezierCurve::CUBIC, thickness, color);
		}
		};

	drawCircle(glm::angleAxis(0.f, Y_AXIS), Z_COLOR, 0.9f);
	drawCircle(glm::angleAxis(glm::radians(90.f), Y_AXIS), X_COLOR, 0.9f);
	drawCircle(glm::angleAxis(glm::radians(90.f), X_AXIS), Y_COLOR, 0.9f);
}

void Rendering::DrawGrid(DrawQueue& q, bool skipLinesOnAxis) {
	float cellSize = 10.f;
	int gridSize = 10;

	float thickness = 1.f;

	ImVec4 colorVec = (ImColor)Color::LIGHT_GRAY;
	colorVec.w = 0.5f;
	ImColor color = colorVec;

	for (int i = 0; i < gridSize; ++i) {
		float x1 = i * cellSize, x2 = (i + 1) * cellSize;
		for (int j = 0; j < gridSize; ++j) {
			float z1 = j * cellSize, z2 = (j + 1) * cellSize;
			auto drawCell = [&](float xSign, float zSign) {
				if (!skipLinesOnAxis || i != 0) { DRAW(q, Line, float3(xSign * x1, 0.f, zSign * z1), float3(xSign * x1, 0.f, zSign * z2), thickness, color); }
				if (!skipLinesOnAxis || j != 0) { DRAW(q, Line, float3(xSign * x1, 0.f, zSign * z1), float3(xSign * x2, 0.f, zSign * z1), thickness, color); }
				};

			drawCell(1.f, 1.f);
			drawCell(-1.f, 1.f);
			drawCell(1.f, -1.f);
			drawCell(-1.f, -1.f);
		}
	}
}

void Rendering::DrawSimulation(const Simulation::World& world, DrawQueue& q, const std::set<int>& highlighted) {
	for (int i = 0; i < world.size(); ++i) {
		auto& body = world.at(i);

		DRAW(q, Point, body.position, 5.f, (highlighted.contains(i) ? Color::YELLOW : Color::RED));
	}
}