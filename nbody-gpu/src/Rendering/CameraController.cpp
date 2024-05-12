#include "CameraController.h"

#include <glm/gtx/euler_angles.hpp>
#include <string>

CameraController::CameraController(double fov, double nearPlane, double farPlane) : m_fov(fov), m_nearPlane(nearPlane), m_farPlane(farPlane) {
	RebuildCamera();
}

const Camera& CameraController::Camera() const { return *m_camera; }

CameraController::View CameraController::CurrentView() const { return m_view; }
CameraController::CameraType CameraController::CurrentType() const { return m_currentType; }

void CameraController::SetView(View view) {
	if (view == m_view) { return; }

	if (view == View::USER) { m_currentType = m_returnType; } else if (m_view == View::USER) { m_returnType = m_currentType; }
	m_view = view;

	RebuildCamera();
}

void CameraController::ReverseView() {
	switch (m_view) {
	case View::FRONT:	SetView(View::BACK); break;
	case View::BACK:	SetView(View::FRONT); break;
	case View::LEFT:	SetView(View::RIGHT); break;
	case View::RIGHT:	SetView(View::LEFT); break;
	case View::TOP:		SetView(View::BOTTOM); break;
	case View::BOTTOM:	SetView(View::TOP); break;
	case View::USER: {
		float3 rot = m_camera->Rotation();
		m_camera->SetRotation(glm::radians(180.f) - rot.x, glm::radians(180.f) - rot.y, 0.f);
	} break;
	}
}

void CameraController::SetType(CameraType type) {
	m_currentType = type;
	m_returnType = type;
	RebuildCamera();
}


void CameraController::RebuildCamera() {
	float3 prevRotation;
	if (m_camera) { prevRotation = m_camera->Rotation(); }

	if (m_currentType == ORTHOGRAPHIC) { m_camera = std::make_unique<OrthographicCamera>(m_nearPlane, m_farPlane); }
	else							   { m_camera = std::make_unique<PerspectiveCamera>(m_fov, m_nearPlane, m_farPlane); }

	m_camera->SetScale(m_scale * (m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR));
	m_camera->SetViewport(m_viewport);

	switch (m_view) {
	case View::FRONT:	m_camera->SetRotation(0.f, 0.f, 0.f); break;
	case View::BACK:	m_camera->SetRotation(0.f, glm::radians(180.f), 0.f); break;
	case View::LEFT:	m_camera->SetRotation(0.f, glm::radians(-90.f), 0.f); break;
	case View::RIGHT:	m_camera->SetRotation(0.f, glm::radians(90.f), 0.f); break;
	case View::TOP:		m_camera->SetRotation(glm::radians(90.f), 0.f, 0.f); break;
	case View::BOTTOM:	m_camera->SetRotation(glm::radians(-90.f), 0.f, 0.f); break;
	case View::USER:	m_camera->SetRotation(prevRotation); break;
	}

	m_camera->UpdateProjection();
}


void CameraController::Pan(const float2& mouseMove) {
	float3 moveCamSpace = float3(1.f * mouseMove, 0.f) * m_panSpeed * (m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR);
	float3 moveWorldSpace = glm::inverse(m_camera->RotationQuaternion()) * glm::vec4(moveCamSpace, 1.f);
	m_camera->Move(moveWorldSpace);
}

void CameraController::Rotate(const float2& mouseMove) {
	if (m_view != View::USER) { SetView(View::USER); }
	m_camera->Rotate(mouseMove.y * m_rotSpeed, mouseMove.x * m_rotSpeed, 0.f);
}

void CameraController::Scale(float mouseWheel) {
	m_scale *= 1 + mouseWheel * m_scaleSpeed;
	m_camera->SetScale(m_scale * (m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR));
}

void CameraController::SetViewport(ImVec2 viewport) {
	m_viewport = viewport;
	m_camera->SetViewport(m_viewport);
	m_camera->UpdateProjection();
}


void CameraController::DisplayInfoWindow() {
	ImGui::Begin("Camera Info", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);

	std::string viewString, typeString;
	switch (m_view) {
	case View::FRONT:	viewString = "FRONT"; break;
	case View::BACK:	viewString = "BACK"; break;
	case View::LEFT:	viewString = "LEFT"; break;
	case View::RIGHT:	viewString = "RIGHT"; break;
	case View::TOP:		viewString = "TOP"; break;
	case View::BOTTOM:	viewString = "BOTTOM"; break;
	case View::USER:	viewString = "USER"; break;
	}
	switch (m_currentType) {
	case ORTHOGRAPHIC:	typeString = "ORTHO"; break;
	case PERSPECTIVE:	typeString = "PERSPECTIVE"; break;
	}

	ImGui::Text("%s %s", viewString.c_str(), typeString.c_str());
	float3 pos = m_camera->Position();
	float3 rot = m_camera->Rotation();
	ImGui::Text("(%g %g %g)", pos.x, pos.y, pos.z);
	ImGui::Text("%g %g %g", glm::degrees(rot.x), glm::degrees(rot.y), glm::degrees(rot.z));
	ImGui::Text("%gx", m_scale);

	ImGui::End();
}