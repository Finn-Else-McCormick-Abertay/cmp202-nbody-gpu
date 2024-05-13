#include "CameraController.h"

#include <glm/gtx/euler_angles.hpp>
#include <string>
#include <Rendering/DrawQueue.h>
#include <Rendering/DrawFunctions.h>

CameraController::CameraController(double fov, double nearPlane, double farPlane) : m_fov(fov), m_nearPlane(nearPlane), m_farPlane(farPlane) {
	RebuildCamera();
}

Camera& CameraController::Camera() { return *m_camera; }

CameraController::View CameraController::CurrentView() const { return m_view; }
CameraController::CameraType CameraController::CurrentType() const { return m_currentType; }

void CameraController::SetView(View view) {
	if (view == m_view) { return; }

	if (view == View::USER) {
		m_currentType = m_returnType;
	}
	else {
		if (m_view == View::USER) { m_returnType = m_currentType; }
		m_currentType = ORTHOGRAPHIC;
	}
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
	if (type == m_currentType) { return; }

	//if (type == PERSPECTIVE) { m_camera->SetPosition(m_camera->Position() * ORTHO_TO_PERSPECTIVE_SCALE_FACTOR); }
	//else { m_camera->SetPosition(m_camera->Position() / ORTHO_TO_PERSPECTIVE_SCALE_FACTOR); }

	m_currentType = type;
	m_returnType = type;
	RebuildCamera();
}


void CameraController::RebuildCamera() {
	float3 prevPosition, prevRotation;
	if (m_camera) {
		prevPosition = m_camera->Position();
		prevRotation = m_camera->Rotation();
	}

	if (m_currentType == ORTHOGRAPHIC) { m_camera = std::make_unique<OrthographicCamera>(m_nearPlane, m_farPlane); }
	else							   { m_camera = std::make_unique<PerspectiveCamera>(m_fov, m_nearPlane, m_farPlane); }

	m_camera->SetScale(m_scale * (m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR) * m_scaleFactor);
	m_camera->SetViewport(m_viewport);

	switch (m_view) {
	case View::FRONT:	m_camera->SetRotation(0.f, 0.f, 0.f); break;
	case View::BACK:	m_camera->SetRotation(0.f, glm::radians(180.f), 0.f); break;
	case View::LEFT:	m_camera->SetRotation(0.f, glm::radians(-90.f), 0.f); break;
	case View::RIGHT:	m_camera->SetRotation(0.f, glm::radians(90.f), 0.f); break;
	case View::TOP:		m_camera->SetRotation(glm::radians(90.f), 0.f, 0.f); break;
	case View::BOTTOM:	m_camera->SetRotation(glm::radians(-90.f), 0.f, 0.f); break;
	case View::USER:
		m_camera->SetPosition(prevPosition);
		m_camera->SetRotation(prevRotation);
		break;
	}

	m_camera->UpdateProjection();
}


void CameraController::Pan(const float2& mouseMove) {
	float3 moveCamSpace = float3(1.f * mouseMove, 0.f) * m_panSpeed;// *(m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR);
	float3 moveWorldSpace = glm::inverse(m_camera->RotationQuaternion()) * glm::vec4(moveCamSpace, 1.f);
	m_camera->Move(moveWorldSpace);
}

void CameraController::Rotate(const float2& mouseMove) {
	if (m_view != View::USER) { SetView(View::USER); }
	m_camera->Rotate(mouseMove.y * m_rotSpeed, mouseMove.x * m_rotSpeed, 0.f);
}

void CameraController::Scale(float mouseWheel) {
	m_scale *= 1 + mouseWheel * m_scaleSpeed;
	m_camera->SetScale(m_scale * (m_currentType == ORTHOGRAPHIC ? 1.f : ORTHO_TO_PERSPECTIVE_SCALE_FACTOR) * m_scaleFactor);
}

void CameraController::SetViewport(ImVec2 viewport) {
	m_viewport = viewport;
	m_camera->SetViewport(m_viewport);
	m_camera->UpdateProjection();
}


void CameraController::DisplayInfoChildWindow() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoMouseInputs;
	ImGuiChildFlags childFlags = ImGuiChildFlags_None;
	//childFlags |= ImGuiChildFlags_FrameStyle;

	ImVec2 parentPos = ImGui::GetWindowPos();

	ImVec2 gimbalOffset = ImVec2(25.f, 20.f);
	ImGui::SetNextWindowPos(ImVec2(parentPos.x + gimbalOffset.x, parentPos.y + gimbalOffset.y));
	ImGui::BeginChild("Camera Gimbal", ImVec2(80.f, 80.f), childFlags, windowFlags);

	DrawQueue drawQueue;
	OrthographicCamera gimbalCam = OrthographicCamera(0.05f, 1.1f);
	gimbalCam.SetViewport(ImGui::GetWindowSize());
	gimbalCam.SetRotation(m_camera->Rotation());

	drawQueue.SetCamera(gimbalCam);
	drawQueue.SetWindowOffset(to_float2(ImGui::GetWindowPos()));

	DrawGimbal(drawQueue);

	drawQueue.ImGuiRender(ImGui::GetWindowDrawList());

	ImGui::EndChild();

	ImVec2 infoOffset = ImVec2(125.f, 20.f);
	ImGui::SetNextWindowPos(ImVec2(parentPos.x + infoOffset.x, parentPos.y + infoOffset.y));
	ImGui::BeginChild("Camera Info", ImVec2(200.f, 100.f), childFlags, windowFlags);

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
	ImGui::Text("(%.3g %.3g %.3g)", pos.x, pos.y, pos.z);
	ImGui::Text("%.3g %.3g %.3g", glm::degrees(rot.x), glm::degrees(rot.y), glm::degrees(rot.z));
	ImGui::Text("%.5gx", m_scale);

	ImGui::EndChild();
}