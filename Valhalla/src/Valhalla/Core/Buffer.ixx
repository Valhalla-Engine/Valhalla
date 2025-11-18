export module Valhalla.Core.Buffer;

import Valhalla.Core.Types;
import Valhalla.Core.Assert;

import <cstring>;

export namespace Valhalla
{
	struct Buffer
	{
		byte* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0) {}

		Buffer(byte* data, uint32_t size)
			: Data(data), Size(size) {}

		static Buffer Copy(const void* data, uint32_t size)
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
			Assert(offset + size <= Size, "buffer overflow!");
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

		byte& operator[](int32_t index) const
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