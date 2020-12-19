#pragma once

#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Log.h"

namespace Valhalla
{
	struct VALHALLA_API Buffer
	{
		byte* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0) {}

		Buffer(byte* data, uint32_t size)
			:Data(data), Size(size) {}

		static Buffer copy(void* data, uint32_t size)
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			delete[] Data;
			Data = nullptr;

			if (size == 0)
				return;

			Data = new byte[size];
			Size = size;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			return *(T*)(Data + offset);
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0)
		{
			VH_CORE_ASSERT(offset + size <= Size, "Buffer Overflow");
			memcpy(Data + offset, data, size);
		}

		operator bool() const
		{
			return Data;
		}

		byte& operator[](int32_t index)
		{
			return Data[index];
		}

		byte operator[](int32_t index) const
		{
			return Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }
	};
}
