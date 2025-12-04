export module Valhalla.Core.Log.Base;

import <string>;
import <string_view>;
import <memory>;
import <map>;
import <format>;


import Valhalla.Core.Log.Types;

export namespace Valhalla {

	namespace Log
	{
		class Logger
		{
		public:
			static void Init();
			static void Shutdown();

			template<typename... Args>
			static void PrintMessage(Type type, Level level, std::string_view tag, std::string_view fmt, Args&&... args)
			{
				std::string text = std::vformat(fmt, std::make_format_args(args...));
				LogMessageInternal(type, level, tag, text);
			}

			template<typename... Args>
			static void PrintAssertMessage(Type type, std::string_view prefix, Args&&... args)
			{
				std::string fmt = std::string(std::forward<Args>(args)...);
				std::string text = std::vformat(fmt, std::make_format_args(args...));
				AssertMessageInternal(type, prefix, text);
			}

			static void PrintAssertMessage(Type type, std::string_view prefix)
			{
				AssertMessageInternal(type, prefix, "");
			}

		private:
			static std::map<std::string, TagDetails>& EnabledTags();
			static bool HasTag(const std::string& tag);

			static const char* LevelToString(Level level);
			static Level StringToLevel(std::string_view levelStr);

			static void LogMessageInternal(Type type, Level level, std::string_view tag, const std::string& msg);
			static void AssertMessageInternal(Type type, std::string_view prefix, const std::string& msg);
		};
	}
}