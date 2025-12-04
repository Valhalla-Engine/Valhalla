#define VH_HAS_CONSOLE !VH_DIST

#define VH_ASSERT_MESSAGE_BOX (!VH_DIST && VH_PLATFORM_WINDOWS)

#if VH_ASSERT_MESSAGE_BOX
#ifdef VH_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#endif

#include "spdlog/sinks/basic_file_sink.h";
#include "spdlog/sinks/stdout_color_sinks.h";

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


module Valhalla.Core.Log.Base;

import <filesystem>;
import <map>;

import Valhalla.Core.Log.Types;

namespace Valhalla
{
	namespace Log
	{
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::map<std::string, Log::TagDetails> s_EnabledTags;

		void Logger::Init()
		{
			std::string logsDirectory = "Logs";
			if (!std::filesystem::exists(logsDirectory))
				std::filesystem::create_directory(logsDirectory);

			std::vector<spdlog::sink_ptr> valhallaSinks =
			{
				std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Valhalla.log", true),
	#if VH_HAS_CONSOLE
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	#endif
			};

			std::vector<spdlog::sink_ptr> appSinks =
			{
				std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/APP.log", true),
	#if VH_HAS_CONSOLE
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	#endif
			};

			valhallaSinks[0]->set_pattern("[%T] [%l] %n: %v");
			appSinks[0]->set_pattern("[%T] [%l] %n: %v");

#if VH_HAS_CONSOLE
			valhallaSinks[1]->set_pattern("[%T] [%l] %n: %v%$");
			appSinks[1]->set_pattern("[%T] [%l] %n: %v%$");
#endif


			s_CoreLogger = std::make_shared<spdlog::logger>("Valhalla", valhallaSinks.begin(), valhallaSinks.end());
			s_CoreLogger->set_level(spdlog::level::trace);

			s_ClientLogger = std::make_shared<spdlog::logger>("App", appSinks.begin(), appSinks.end());
			s_ClientLogger->set_level(spdlog::level::trace);
		}

		void Logger::Shutdown()
		{
			s_ClientLogger.reset();
			s_CoreLogger.reset();
			spdlog::drop_all();
		}

		std::map<std::string, Log::TagDetails>& Logger::EnabledTags()
		{
			return s_EnabledTags;
		}

		bool Logger::HasTag(const std::string & tag)
		{
			return s_EnabledTags.find(tag) != s_EnabledTags.end();
		}

		const char* Logger::LevelToString(Level level)
		{
			switch (level)
			{
			case Level::Trace: return "Trace";
			case Level::Info: return "Info";
			case Level::Warn: return "Warn";
			case Level::Error: return "Error";
			case Level::Fatal: return "Fatal";
			}

			return "";
		}

		Log::Level Logger::StringToLevel(std::string_view levelStr)
		{
			if (levelStr == "Trace") return Level::Trace;
			if (levelStr == "Info")  return Level::Info;
			if (levelStr == "Warn")  return Level::Warn;
			if (levelStr == "Error") return Level::Error;
			if (levelStr == "Fatal") return Level::Fatal;

			return Level::Trace;

		}

		void Logger::LogMessageInternal(Type type, Level level, std::string_view tag, const std::string & message)
		{
			auto detail = s_EnabledTags[std::string(tag)];
			if (!detail.Enabled || detail.LevelFilter > level)
				return;

			auto& logger = (type == Type::Core) ? s_CoreLogger : s_ClientLogger;

			std::string logFormat = tag.empty() ? "{0}{1}" : "[{0}] {1}";

			switch (level)
			{
				case Valhalla::Log::Level::Trace: logger->trace(logFormat, tag, message); break;
				case Valhalla::Log::Level::Info: logger->info(logFormat, tag, message); break;
				case Valhalla::Log::Level::Warn: logger->warn(logFormat, tag, message); break;
				case Valhalla::Log::Level::Error: logger->error(logFormat, tag, message); break;
				case Valhalla::Log::Level::Fatal: logger->critical(logFormat, tag, message); break;
			}
		}

		void Logger::AssertMessageInternal(Type type, std::string_view prefix, const std::string & message)
		{
			auto& logger = (type == Type::Core) ? s_CoreLogger : s_ClientLogger;
			if (!message.empty())
				logger->error("{0}: {1}", prefix, message);
			else
				logger->error("{0}", prefix);
#if VH_ASSERT_MESSAGE_BOX
			MessageBoxA(nullptr, "No message :(", "Valhalla Assert", MB_OK | MB_ICONERROR);
#endif
		}
	}
}