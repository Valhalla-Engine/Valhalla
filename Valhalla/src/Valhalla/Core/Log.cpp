#include "vhpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>


namespace Valhalla
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::string logsDirectory = "logs";
		if (!std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> valhallaSinks =
		{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Valhalla.log", true)
		};

		std::vector<spdlog::sink_ptr> appSinks =
		{
			std::make_shared<spdlog::sinks::stderr_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Odin.log", true),
		};

		valhallaSinks[0]->set_pattern("%^[%T] %n: %v%$");
		valhallaSinks[1]->set_pattern("[%T] [%l] %n: %v");
		appSinks[0]->set_pattern("%^[%T] %n: %v%$");
		appSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("Valhalla", valhallaSinks.begin(), valhallaSinks.end());
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Odin", appSinks.begin(), appSinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);

	}
}