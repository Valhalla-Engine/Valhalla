#include <DirectXMath.h>

export module Valhalla.Renderer.Camera;

namespace Valhalla
{
	namespace Heimdall
	{
		class Camera
		{
		public:
			Camera();
			Camera(const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4X4& unReversedProjection);
			Camera(const float_t degFov, const float_t width, const float_t height, const float_t nearP, const float_t farP);

			const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			const DirectX::XMFLOAT4X4& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }

			void SetProjectionMatrix(const DirectX::XMFLOAT4X4 projection, const DirectX::XMFLOAT4X4 unReversedProjection)
			{
				m_ProjectionMatrix = projection;
				m_UnReversedProjectionMatrix = unReversedProjection;
			}

			void SetPerspectiveProjectionMatrix(const float_t radFov, const float_t width, const float_t height, const float_t nearP, const float_t farP)
			{
				DirectX::XMStoreFloat4x4(&m_ProjectionMatrix,
					DirectX::XMMatrixPerspectiveFovRH(radFov, (width / height), nearP, farP));
				DirectX::XMStoreFloat4x4(&m_UnReversedProjectionMatrix,
					DirectX::XMMatrixPerspectiveFovRH(radFov, (width / height), nearP, farP));
			}

			void SetOrthoProjectionMatrix(const float_t width, const float_t height, const float_t nearP, const float_t farP)
			{
				DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixOrthographicRH(width, height, nearP, farP));
				DirectX::XMStoreFloat4x4(&m_UnReversedProjectionMatrix, DirectX::XMMatrixOrthographicRH(width, height, nearP, farP));
			}

			float_t GetExposure() const { return m_Exposure; }
			float_t& GetExposure() { return m_Exposure; }

		protected:
			float_t m_Exposure = 0.8f;
		private:
			DirectX::XMFLOAT4X4 m_ProjectionMatrix;
			DirectX::XMFLOAT4X4 m_UnReversedProjectionMatrix;
		};
	}
}