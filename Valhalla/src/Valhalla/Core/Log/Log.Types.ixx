export module Valhalla.Core.Log.Types;

import Valhalla.Core.Types;

export namespace Valhalla
{
	namespace Log
	{
		enum class Type : uint8_t
		{
			Core = 0,
			Client = 1
		};

		enum class Level : uint8_t
		{
			Trace = 0, Info, Warn, Error, Fatal
		};

		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};
	}
}