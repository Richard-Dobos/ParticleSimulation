#pragma once

#include "Core.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Components/Components.h"

namespace Core::Engine
{
	struct CORE_API CameraProperties
	{
		uint16_t resolutionHeight, resolutionWidth;
		
		float FOV;
		float aspectRatio;
		float nearClip, farClip;
	};

	class CORE_API Camera2d
	{
	public:
		Camera2d(Transform transform, uint16_t resolutionWidth, uint16_t resolutionHeight, float aspectRatio, float nearClip, float farClip);
		Camera2d(Transform transform, const CameraProperties& cameraProperties) : Camera2d(transform, cameraProperties.resolutionHeight, cameraProperties.resolutionWidth, cameraProperties.aspectRatio, cameraProperties.nearClip, cameraProperties.farClip) {}
		~Camera2d() = default;

		const glm::mat4 getViewMatrix() const;
		const glm::mat4 getProjectionMatrix() const;

		const CameraProperties* getCameraProperties() const;
		
	public:
		Transform m_Transform;

	private:
		CameraProperties m_CameraProperties;
	};
}