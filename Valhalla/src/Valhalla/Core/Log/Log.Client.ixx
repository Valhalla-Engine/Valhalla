export module Valhalla.Core.Log:Client;

import <string>;

import Valhalla.Core.Log.Types;
import Valhalla.Core.Log.Base;


export namespace Valhalla
{
	namespace Log
	{
		//---------- Core Log Functions with Tag-----------//
		template<typename... Args>
		void ClientTraceTag(std::string_view tag, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Trace, tag, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientInfoTag(std::string_view tag, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Info, tag, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientWarnTag(std::string_view tag, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Warn, tag, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientErrorTag(std::string_view tag, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Error, tag, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientFatalTag(std::string_view tag, Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Fatal, tag, std::forward<Args>(args)...);
		}

		//---------- Core Log Functions without Tag-----------//
		template<typename... Args>
		void ClientTrace(Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Trace, "", std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientInfo(Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Info, "", std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientWarn(Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Warn, "", std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientError(Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Error, "", std::forward<Args>(args)...);
		}

		template<typename... Args>
		void ClientFatal(Args&&... args)
		{
			Logger::PrintMessage(Log::Type::Client, Log::Level::Fatal, "", std::forward<Args>(args)...);
		}
	}
}