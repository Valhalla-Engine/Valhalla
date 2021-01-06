#pragma once

#include "Valhalla/Renderer/Camera.h"
#include "Valhalla/Core/Timestep.h"
#include "Valhalla/Core/Events/MouseEvent.h"

namespace Valhalla
{
	class VALHALLA_API EditorCamera : public Heimdal::Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(const glm::mat4& projectionMatrix);

		void Focus();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float_t GetDistance() const { return m_Distance; }
		inline void SetDistance(float_t distance) { m_Distance = distance; }

		inline void SetViewportSize(uint32_t width, uint32_t height) { m_ViewportWidth = width; m_ViewportHeight = height; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetForwardDirection();
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float_t GetPitch() const { return m_Pitch; }
		float_t GetYaw() const { return m_Yaw; }

	private:
		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float_t delta);

		glm::vec3 CalculatePosition();

		std::pair<float_t, float_t> PanSpeed() const;
		float_t RotationSpeed() const;
		float_t ZoomSpeed() const;
	private:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position, m_Rotation, m_FocalPoint;

		bool m_Panning, m_Rotating;
		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;

		float_t m_Distance;
		float_t m_Pitch, m_Yaw;

		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}