#include "vhpch.h"
#include "UUID.h"

#include <cstdlib>
#include <random>
#include <ctime>

namespace Valhalla
{
	//static std::random_device s_RandomDevice;
	//std::srand(std::time(nullptr));
	int32_t seed = std::rand();
	static std::mt19937_64 eng(seed);
	static std::uniform_int_distribution<long long> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(eng)) {}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid) {}

	UUID::UUID(const UUID& other)
		: m_UUID(other.m_UUID) {}
}