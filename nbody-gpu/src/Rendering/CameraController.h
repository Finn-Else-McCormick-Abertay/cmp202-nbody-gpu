#pragma once

#include <Rendering/Camera.h>
#include <memory>

class CameraController
{
private: using AbstractCamera = Camera;
public:
	enum CameraType { ORTHOGRAPHIC, PERSPECTIVE };
	enum class View { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, USER };

	CameraController(double fov = 120.0, double nearPlane = 0.1, double farPlane = 100'000.0);

	const Camera& Camera() const;

	View CurrentView() const; CameraType CurrentType() const;
	void SetView(View); void SetType(CameraType);
	void ReverseView();

	void Pan(const float2& mouseMove);
	void Rotate(const float2& mouseMove);
	void Scale(float mouseWheel);

	void SetViewport(ImVec2);

	void DisplayInfoWindow();

private:
	std::unique_ptr<AbstractCamera> m_camera = nullptr;

	View m_view = View::FRONT;
	CameraType m_currentType = ORTHOGRAPHIC, m_returnType = ORTHOGRAPHIC;

	float m_scale = 1.f;
	double m_fov, m_nearPlane, m_farPlane;
	ImVec2 m_viewport;

	void RebuildCamera();

	float m_panSpeed = 0.5f;
	float m_rotSpeed = 0.01f;
	float m_scaleSpeed = 0.1f;

	static constexpr float ORTHO_TO_PERSPECTIVE_SCALE_FACTOR = 0.0006f;
};