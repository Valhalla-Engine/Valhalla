#pragma once

#include "Valhalla/Core/Base.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Valhalla
{
	class VALHALLA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define VH_CORE_TRACE(...)		Valhalla::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VH_CORE_INFO(...)		Valhalla::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VH_CORE_WARN(...)		Valhalla::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VH_CORE_ERROR(...)		Valhalla::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VH_CORE_FATAL(...)		Valhalla::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define VH_TRACE(...)		Valhalla::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VH_INFO(...)		Valhalla::Log::GetClientLogger()->info(__VA_ARGS__)
#define VH_WARN(...)		Valhalla::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VH_ERROR(...)		Valhalla::Log::GetClientLogger()->error(__VA_ARGS__)
#define VH_FATAL(...)		Valhalla::Log::GetClientLogger()->critical(__VA_ARGS__)