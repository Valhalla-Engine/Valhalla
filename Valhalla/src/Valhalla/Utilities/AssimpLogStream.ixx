module;
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

export module Valhalla.Utils.AssimpLogStream;

import Valhalla.Core.Log;


namespace Valhalla
{
	struct AssimpLogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream, Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Warn | Assimp::Logger::Err);
			}
		}

		virtual void write(const char* message) override
		{
			std::string msg(message);
			if (!msg.empty() && msg[msg.length() - 1] == '\n')
			{
				msg.erase(msg.length() - 1);
			}
			if (strncmp(message, "Debug", 5) == 0)
			{
				Log::CoreTraceTag("Assimp", msg);
			}
			else if (strncmp(message, "Info", 4) == 0)
			{
				Log::CoreInfoTag("Assimp", msg);
			}
			else if (strncmp(message, "Warn", 4) == 0)
			{
				Log::CoreWarnTag("Assimp", msg);
			}
			else
			{
				Log::CoreErrorTag("Assimp", msg);
			}
		}
	};
}