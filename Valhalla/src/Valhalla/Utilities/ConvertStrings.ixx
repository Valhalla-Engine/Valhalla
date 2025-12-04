#include <Windows.h>

export module Valhalla.Utils.ConvertStrings;

import <string>;

export namespace Valhalla
{
	export namespace Utils
	{
		inline std::string WideToUtf8(const wchar_t* w)
		{
			if (!w) return {};

			int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
			std::string result(sizeNeeded - 1, 0);
			WideCharToMultiByte(CP_UTF8, 0, w, -1, result.data(), sizeNeeded, nullptr, nullptr);
			return result;
		}
	}
}