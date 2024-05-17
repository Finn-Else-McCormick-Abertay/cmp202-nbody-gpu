#pragma once

#include <Rendering/Camera.h>
#include <memory>

namespace Rendering {

	class CameraController
	{
	private: using AbstractCamera = Camera;
	public:
		enum CameraType { ORTHOGRAPHIC, PERSPECTIVE };
		enum class View { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, USER };

		CameraController(double fov = 120.0, double nearPlane = 0.1, double farPlane = 100'000.0);

		Camera& Camera();

		View CurrentView() const; CameraType CurrentType() const;
		void SetView(View); void SetType(CameraType);
		void ReverseView();

		void Pan(const float2& mouseMove);
		void Rotate(const float2& mouseMove);
		void Scale(float mouseWheel);

		void SetViewport(ImVec2);

		void DisplayInfoChildWindow();

	private:
		std::unique_ptr<AbstractCamera> m_camera = nullptr;

		View m_view = View::FRONT;
		CameraType m_currentType = ORTHOGRAPHIC, m_returnType = ORTHOGRAPHIC;

		float m_scale = 1.f;
		double m_fov, m_nearPlane, m_farPlane;
		ImVec2 m_viewport;

		void RebuildCamera();

		float m_panSpeed = 0.2f;
		float m_rotSpeed = 0.005f;
		float m_scaleSpeed = 0.1f;

		float m_scaleFactor = 4.f;

		static constexpr float ORTHO_TO_PERSPECTIVE_SCALE_FACTOR = 0.00061f;
	};

}