#pragma once

#include <variant>

#include "Core.h"
#include "Components/Components.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace Core::Engine
{
	struct CORE_API CameraProperties
	{
		float aspectRatio;
		float nearClip, farClip;
		
		uint16_t *resolutionHeight, *resolutionWidth;
	};

	class CORE_API Camera2d
	{
	public:
		Camera2d(Components::Transform transform, uint16_t* resolutionWidth, uint16_t* resolutionHeight, float aspectRatio, float nearClip, float farClip);
		Camera2d(Components::Transform transform, const CameraProperties& cameraProperties) :
			Camera2d(transform, cameraProperties.resolutionHeight, cameraProperties.resolutionWidth, cameraProperties.aspectRatio, cameraProperties.nearClip, cameraProperties.farClip) {}
		~Camera2d() = default;

		const glm::mat4 getViewMatrix() const;
		const glm::mat4 getProjectionMatrix() const;

		const CameraProperties* getCameraProperties() const;

	public:
		Components::Transform m_Transform;

	private:
		CameraProperties m_CameraProperties;
	};
}