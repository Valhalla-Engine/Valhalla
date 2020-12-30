#include "vhpch.h"
#include "Camera.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Camera::Camera(const glm::mat4& projectionMatrix)
			: m_ProjectionMatrix(projectionMatrix) {}
	}
}


