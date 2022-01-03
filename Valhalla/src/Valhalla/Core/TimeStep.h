#pragma once

namespace Valhalla
{
	class Timestep
	{
	public:
		Timestep() = default;
		Timestep(float_t time);

		inline float_t GetSeconds() { return m_Time; }
		inline float_t GetMilliSeconds() const { return m_Time * 1000.0f; }

		operator float_t() { return m_Time; }
	private:
		float_t m_Time = 0.0f;
	};
}