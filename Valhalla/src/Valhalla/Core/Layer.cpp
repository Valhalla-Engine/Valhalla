#include "vhpch.h"
#include "Layer.h"

namespace Valhalla
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) {}

	Layer::~Layer() {}
}