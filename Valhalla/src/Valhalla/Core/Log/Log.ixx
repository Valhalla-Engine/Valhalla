export module Valhalla.Core.Log;

export import :Core;
export import :Client;
import Valhalla.Core.Log.Base;

export namespace Valhalla 
{
	namespace Log
	{
		void Init()
		{
			Logger::Init();
		}

		void Shutdown()
		{
			Logger::Shutdown();
		}
	}
}