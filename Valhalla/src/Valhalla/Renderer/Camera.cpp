#include "vhpch.h"
#include "Camera.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Camera::Camera()
		{
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixIdentity());
			DirectX::XMStoreFloat4x4(&m_UnReversedProjectionMatrix, DirectX::XMMatrixIdentity());
		}

		Camera::Camera(const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4X4& UnReversedProjection)
			: m_ProjectionMatrix(projection), m_UnReversedProjectionMatrix(UnReversedProjection)
		{

		}

		Camera::Camera(const float_t degFov, const float_t width, const float_t height, const float_t nearP, const float_t farP)
		{
			DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, 
				DirectX::XMMatrixPerspectiveFovRH(DirectX::XMConvertToRadians(degFov), (width / height), nearP, farP));
			DirectX::XMStoreFloat4x4(&m_UnReversedProjectionMatrix,
				DirectX::XMMatrixPerspectiveFovRH(DirectX::XMConvertToRadians(degFov), (width / height), nearP, farP));
		}
	}
}