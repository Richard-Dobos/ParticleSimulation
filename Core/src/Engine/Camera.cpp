#include "Camera.h"

namespace Core::Engine
{
	Camera2d::Camera2d(Transform transform, uint16_t resolutionWidth, uint16_t resolutionHeight, float aspectRatio, float nearClip, float farClip)
		:m_Transform(transform)
	{
		m_CameraProperties.resolutionWidth = resolutionWidth;
		m_CameraProperties.resolutionHeight = resolutionHeight;
		m_CameraProperties.aspectRatio = aspectRatio;
		m_CameraProperties.nearClip = nearClip;
		m_CameraProperties.farClip = farClip;
	}

	const glm::mat4 Camera2d::getViewMatrix() const 
	{
		return glm::translate(glm::mat4(1.0f), m_Transform.pos);
	}

	const glm::mat4 Camera2d::getProjectionMatrix() const
	{
		return glm::ortho(0.0f, m_CameraProperties.resolutionWidth / m_Transform.scale.x, m_CameraProperties.resolutionHeight / m_Transform.scale.y, 0.0f, m_CameraProperties.nearClip, m_CameraProperties.farClip);
	}

	const CameraProperties* Camera2d::getCameraProperties() const
	{
		return &m_CameraProperties;
	}
}
