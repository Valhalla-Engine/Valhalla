module Valhalla.Core.Window;

import Valhalla.Platform.Windows.Window;

namespace Valhalla
{
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
}