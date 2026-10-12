
module Valhalla.Renderer.Camera;
import Valhalla.Math;
namespace Valhalla
{
	namespace Heimdall
	{
		Camera::Camera()
		{
		}

		Camera::Camera(const Math::Mat4x4& projection, const Math::Mat4x4& unReversedProjection)
			: m_ProjectionMatrix(projection), m_UnReversedProjectionMatrix(unReversedProjection)
		{

		}

		Camera::Camera(const float_t degFov, const float_t width, const float_t height, const float_t nearP, const float_t farP)
		{
			m_ProjectionMatrix = Math::MatrixPerspectiveFovRH(Math::ConvertToRadians(degFov), (width / height), nearP, farP);
			m_UnReversedProjectionMatrix = Math::MatrixPerspectiveFovRH(Math::ConvertToRadians(degFov), (width / height), nearP, farP);
		}
	}
}