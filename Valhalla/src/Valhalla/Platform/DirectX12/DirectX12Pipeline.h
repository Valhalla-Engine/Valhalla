#pragma once

#include "VD3D12.h"

#include "Valhalla/Renderer/Pipeline.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Pipeline : public Heimdall::Pipeline
		{
		public:
			DirectX12Pipeline(const Heimdall::PipelineSpecification& specs);
			virtual ~DirectX12Pipeline();

			virtual Heimdall::PipelineSpecification& GetSpecification() override { return m_Specification; }
			virtual const Heimdall::PipelineSpecification& GetSpecification() const override { return m_Specification; }

			virtual void Invalidate() override;
			virtual void SetUniformBuffer(Ref<Heimdall::UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set = 0) override;

			Microsoft::WRL::ComPtr<ID3D12PipelineState> GetDirectX12Pipeline() { return m_Pipeline; }
			Microsoft::WRL::ComPtr<ID3D12RootSignature> GetDirectX12RootSignature() { return m_PipelineLayout; }

			void RT_SetUniformBuffer(Ref<Heimdall::UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set = 0);

		private:
			Heimdall::PipelineSpecification m_Specification;

			Microsoft::WRL::ComPtr<ID3D12PipelineState> m_Pipeline = nullptr;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_PipelineLayout = nullptr;
			Microsoft::WRL::ComPtr<ID3D12PipelineLibrary> m_PipelineCache = nullptr;
		};
	}
}