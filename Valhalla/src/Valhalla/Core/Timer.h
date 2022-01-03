#pragma once

#include <chrono>
#include <unordered_map>

#include "Log.h"

namespace Valhalla
{
	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Timer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float_t Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.0001f * 0.001f * 0.0001f;
		}

		float_t Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const std::string& name)
			: m_Name(name) {}
		~ScopedTimer()
		{
			float_t time = m_Timer.ElapsedMillis();
			VH_CORE_TRACE("[Timer] {0} - {1}ms", m_Name, time);
		}
	private:
		std::string m_Name;
		Timer m_Timer;
	};

	class PerformanceProfiler
	{
	public:
		void SetOerFrameTiming(const char* name, float_t time)
		{
			if (m_PerFrameData.find(name) == m_PerFrameData.end())
				m_PerFrameData[name] = 0.0f;

			m_PerFrameData[name] += time;
		}

		void Clear() { m_PerFrameData.clear(); }

		const std::unordered_map<const char*, float_t>& GetPerFrameData() const { return m_PerFrameData; }
	private:
		std::unordered_map<const char*, float_t> m_PerFrameData;
	};

	class ScopePerfTimer
	{
	public:
		ScopePerfTimer(const char* name, PerformanceProfiler* profiler)
			: m_Name(name), m_Profiler(profiler) {}

		~ScopePerfTimer()
		{
			float_t time = m_Timer.ElapsedMillis();
			m_Profiler->SetOerFrameTiming(m_Name, time);
		}
	private:
		const char* m_Name;
		PerformanceProfiler* m_Profiler;
		Timer m_Timer;
	};

#define VH_SCOPE_PERF(name)\
		ScopePerfTimer timer__LINE__(name, Application::Get().GetPerformanceProfiler());

#define VH_SCOPE_TIMER(name)\
		ScopedTimer timer__LINE__(name);
}