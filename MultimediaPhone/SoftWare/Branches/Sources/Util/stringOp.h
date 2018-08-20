#ifndef __UTIL_STRINGOP_H__
#define __UTIL_STRINGOP_H__

#include "DateTime.h"

#pragma warning(disable: 4786)
#include <string>

namespace Util
{
	class StringOp
	{
	public:
		static int ToInt(std::string const& content, size_t const mode = 10);
		static double ToDouble(std::string const& content);
		static bool ToBoolean(std::string const& content);
		static Timestamp ToTimestamp(std::string const& content);
		static TimeSpan ToTimeSpan(std::string const& content);
		static CString ToCString(std::string const& value);
		/*
		static Structure::Color ToColor(std::string& content);
		static Structure::Rectangle* ToRectangle(std::string& content); //[(x0,y0)(x1,y1)]
		static Structure::LinkMode ToEnumLinkMode(std::string& content);
		static Structure::IPMode ToEnumIPMode(std::string& content);
		static Structure::EncryptionMode ToEnumEncryptionMode(std::string& content);
		static Structure::Position ToEnumPosition(std::string& content);
		static Structure::DisplayMode ToEnumDisplayMode(std::string& content);
		static Structure::VideoDisplayMode ToEnumVideoDisplayMode(std::string& content);
		static Structure::ImageDisplayMode ToEnumImageDisplayMode(std::string& content);
		static Structure::TextDisplayMode ToEnumTextDisplayMode(std::string& content);
		static Structure::FileType ToEnumFileType(std::string& content);
		static Structure::LightModifyMode ToEnumLightModifyMode(std::string& content);
		static Structure::SoundModifyMode ToEnumSoundModifyMode(std::string& content);
		static Structure::Direction ToEnumDirection(std::string& content);
		static Structure::Font ToEnumFont(std::string& content);
		static Structure::FontStyle ToEnumFontStyle(std::string& content);
		*/
		static std::string FromInt(int const value);
		static std::string FromDouble(double const value);
		static std::string FromTimestamp(Timestamp const& timestamp);
		static std::string FromTimeSpan(TimeSpan const& timeSpan);
		static std::string FromCString(CString const& value);
		/*
		static std::string FromEnumLogLevel(Structure::LogLevel const logLevel);
		static std::string FromEnumErrorCode(Structure::ErrorCode const ErrorCode);
		static std::string FromEnumFileType(Structure::FileType const fileType);
		*/
	};
}

#endif //__UTIL_STRINGOP_H__