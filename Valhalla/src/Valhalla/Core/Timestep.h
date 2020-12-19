#pragma once
#include "Valhalla/Core/Base.h"

namespace Valhalla
{
	class VALHALLA_API Timestep
	{
	public:
		Timestep() {}
		Timestep(float_t time);

		inline float_t GetSeconds() const { return m_Time; }
		inline float_t GetMilliSecond() const { return m_Time * 1000.0f; }

		operator float() { return m_Time; }

	private:
		float_t m_Time;
	};
}