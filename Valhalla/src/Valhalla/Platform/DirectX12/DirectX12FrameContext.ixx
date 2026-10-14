export module Valhalla.Yggdrasil.DirectX12.FrameContext;

import Valhalla.Core.Types;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		struct DirectX12FrameContext
		{
			uint64_t FrameNumber = 0;
			uint32_t FrameIndex = 0;

			uint64_t GraphicsFenceValue = 0;
			uint64_t ComputeFenceValue = 0;
			uint64_t CopyFenceValue = 0;
		};
	}
}