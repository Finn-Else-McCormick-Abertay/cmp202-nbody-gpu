#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Rendering {

	Camera::Camera(double near, double far) : m_projectionMat(glm::identity<mat4x4>()), m_near(near), m_far(far), m_pos(), m_angles(), m_scale(1.f) { UpdateViewMatrix(); }

	float3 Camera::Position() const { return m_pos; }
	float3 Camera::Rotation() const { return m_angles; }
	float Camera::Scale() const { return m_scale; }

	void Camera::SetPosition(const float3& pos) { m_pos = pos; UpdateViewMatrix(); }
	void Camera::Move(const float3& move) { SetPosition(m_pos + move); }

	void Camera::SetRotation(const float3& rot) { m_angles = rot; UpdateViewMatrix(); }
	void Camera::SetRotation(float pitch, float yaw, float roll) { SetRotation(float3(pitch, yaw, roll)); }
	void Camera::Rotate(const float3& rot) {
		m_angles.x += rot.x; m_angles.y += rot.y; m_angles.z += rot.z;
		UpdateViewMatrix();
	}
	void Camera::Rotate(float pitch, float yaw, float roll) { Rotate(float3(pitch, yaw, roll)); }

	void Camera::SetScale(float scale) { m_scale = scale; UpdateViewMatrix(); }

	quat Camera::RotationQuaternion() const {
		return glm::angleAxis(m_angles.x, X_AXIS) * glm::angleAxis(m_angles.y, Y_AXIS) * glm::angleAxis(m_angles.z, Z_AXIS);
	}

	const ImVec2& Camera::Viewport() const { return m_viewport; }
	void Camera::SetViewport(const ImVec2& viewport) { m_viewport = viewport; }

	void Camera::UpdateViewMatrix() {
		m_viewMat = glm::lookAt(m_pos + Z_AXIS * RotationQuaternion(), m_pos, Y_AXIS * RotationQuaternion())
			* glm::translate(glm::scale(glm::translate(mat4x4(1.f), m_pos), float3(m_scale, m_scale, m_scale)), -m_pos);
	}


	float3 Camera::Project(const float3& point) const {
		return glm::project(point, m_viewMat, m_projectionMat, glm::vec4(0.f, 0.f, Viewport().x, Viewport().y));
	}



	PerspectiveCamera::PerspectiveCamera(double fov, double near, double far) : Camera(near, far), m_fov(fov) {}

	void PerspectiveCamera::UpdateProjection() {
		m_projectionMat = glm::perspectiveFov(m_fov, (double)Viewport().x, (double)Viewport().y, m_near, m_far);
	}



	OrthographicCamera::OrthographicCamera(double near, double far) : Camera(near, far) {}

	void OrthographicCamera::UpdateProjection() {
		double halfSizeX = (double)Viewport().x / 2.0, halfSizeY = (double)Viewport().y / 2.0;
		m_projectionMat = glm::ortho(-halfSizeX, halfSizeX, -halfSizeY, halfSizeY, m_near, m_far);
	}

}