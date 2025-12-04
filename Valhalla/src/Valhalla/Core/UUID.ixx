module;
#include <xhash>

export module Valhalla.Core.UUID;

import Valhalla.Core.Types;

export namespace Valhalla
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

	class UUID32
	{
	public:
		UUID32();
		UUID32(uint32_t uuid);
		UUID32(const UUID32& other);

		operator uint32_t() const { return m_UUID; }
	private:
		uint32_t m_UUID;
	};
}

export namespace std
{
	template<>
	struct hash<Valhalla::UUID>
	{
		std::size_t operator()(const Valhalla::UUID& uuid) const
		{
			return uuid;
		}
	};

	template<>
	struct hash<Valhalla::UUID32>
	{
		std::size_t operator()(const Valhalla::UUID32& uuid) const
		{
			return hash<uint32_t>()((uint32_t)uuid);
		}
	};
}