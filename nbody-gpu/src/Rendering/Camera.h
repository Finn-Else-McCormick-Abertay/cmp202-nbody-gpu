#pragma once

#include <MathsTypes.h>
#include <imgui.h>

class Camera
{
public:
	virtual ~Camera() = default;

	float3 Project(const float3&) const;

	virtual void UpdateProjection() = 0;

	float3 Position() const;
	float3 Rotation() const;
	float Scale() const;
	quat RotationQuaternion() const;

	void SetPosition(const float3&); void Move(const float3&);
	void SetRotation(float pitch, float yaw, float roll); void SetRotation(const float3&);
	void Rotate(float pitch, float yaw, float roll); void Rotate(const float3&);
	void SetScale(float);

	const ImVec2& Viewport() const;
	void SetViewport(const ImVec2&);

protected:
	Camera(double near, double far);

	mat4x4 m_projectionMat;
	double m_near, m_far;
	ImVec2 m_viewport;

	mat4x4 m_viewMat;
	float3 m_pos; float3 m_angles; float m_scale;
	void UpdateViewMatrix();
};


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(double fov, double near, double far);

	virtual void UpdateProjection() override;

private:
	double m_fov;
};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(double near, double far);

	virtual void UpdateProjection() override;
};