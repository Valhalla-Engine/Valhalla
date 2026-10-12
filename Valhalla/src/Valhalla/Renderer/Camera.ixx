

export module Valhalla.Renderer.Camera;

import Valhalla.Math;
import Valhalla.Core.Types;
namespace Valhalla
{
	namespace Heimdall
	{
		class Camera
		{
		public:
			Camera();
			Camera(const Math::Mat4x4& projection, const Math::Mat4x4& unReversedProjection);
			Camera(const float_t degFov, const float_t width, const float_t height, const float_t nearP, const float_t farP);

			const Math::Mat4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			const Math::Mat4x4& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }

			void SetProjectionMatrix(const Math::Mat4x4& projection, const Math::Mat4x4& unReversedProjection)
			{
				m_ProjectionMatrix = projection;
				m_UnReversedProjectionMatrix = unReversedProjection;
			}

			void SetPerspectiveProjectionMatrix(const float_t radFov, const float_t width, const float_t height, const float_t nearP, const float_t farP)
			{
				m_ProjectionMatrix = Math::MatrixPerspectiveFovRH(radFov, (width / height), nearP, farP);
				m_UnReversedProjectionMatrix = Math::MatrixPerspectiveFovRH(radFov, (width / height), nearP, farP);
			}

			void SetOrthoProjectionMatrix(const float_t width, const float_t height, const float_t nearP, const float_t farP)
			{
				m_ProjectionMatrix = Math::MatrixOrthographicRH(width, height, nearP, farP);
				m_UnReversedProjectionMatrix = Math::MatrixOrthographicRH(width, height, nearP, farP);
			}

			float_t GetExposure() const { return m_Exposure; }
			float_t& GetExposure() { return m_Exposure; }

		protected:
			float_t m_Exposure = 0.8f;
		private:
			Math::Mat4x4 m_ProjectionMatrix;
			Math::Mat4x4 m_UnReversedProjectionMatrix;
		};
	}
}