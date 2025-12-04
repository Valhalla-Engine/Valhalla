export module Valhalla.Core.Log:Core;

import <string>;
import Valhalla.Core.Log.Types;
import Valhalla.Core.Log.Base;


export namespace Valhalla 
{
	namespace Log
	{
		//---------- Core Log Functions with Tag-----------//
		template<typename... Args>
		void CoreTraceTag(std::string_view tag, std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Trace, tag, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreInfoTag(std::string_view tag, std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Info, tag, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreWarnTag(std::string_view tag, std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Warn, tag, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreErrorTag(std::string_view tag, std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Error, tag, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreFatalTag(std::string_view tag, std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Fatal, tag, fmt, std::forward<Args>(args)...);
		}

		//---------- Core Log Functions without Tag-----------//
		template<typename... Args>
		void CoreTrace(std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Trace, "", fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreInfo(std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Info, "", fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreWarn(std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Warn, "", fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreError(std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Error, "", fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreFatal(std::string_view fmt, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Core, Log::Level::Fatal, "", fmt, std::forward<Args>(args)...);
		}
	}
}