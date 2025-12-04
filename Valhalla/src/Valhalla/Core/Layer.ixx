export module Valhalla.Core.Layer;

import Valhalla.Core.TimeStep;
import Valhalla.Core.Events;

import <string>;

export namespace Valhalla
{
	export class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}