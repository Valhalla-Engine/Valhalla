#pragma once

export module Valhalla.Editor.EditorPanel;

import Valhalla.Core.Ref;
import Valhalla.Core.Events;


export namespace Valhalla
{
	class EditorPanel : public RefCounted
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender(bool& isOpen) = 0;
		virtual void OnEvent(Valhalla::Event& e) = 0;
	};
}