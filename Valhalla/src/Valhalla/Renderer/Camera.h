#pragma once
#include "Valhalla/Core/Base.h"

#include <glm/glm.hpp>

namespace Valhalla
{
	namespace Heimdal
	{
		class VALHALLA_API Camera
		{
		public:
			Camera() = default;
			Camera(const glm::mat4& projectionMatrix);
			virtual ~Camera() = default;

			const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }

			float_t GetExposure() const { return m_Exposure; }
			float_t& GetExposure() { return m_Exposure; }

		protected:
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
			float_t m_Exposure = 0.8f;
		};
	}
}