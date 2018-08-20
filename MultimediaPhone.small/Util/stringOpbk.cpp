#include "stringOpbk.h"

namespace Util
{
	int StringOp::ToInt(std::string const& content, size_t const mode)
	{
		char* endPos = 0;
		int result = ::strtol(content.c_str(), &endPos, mode); //atoi(content.c_str());
		return result;
	}

	double StringOp::ToDouble(std::string const& content)
	{
		//char* endPos = 0;
		//double result = ::strtod(content.c_str(), &endPos);
		double result = ::atof(content.c_str());
		return result;
	}

	bool StringOp::ToBoolean(std::string const& content)
	{
		bool result = false;
		if (content == "Yes")
			result = true;
		else if (content == "No")
			result = false;
		return result;
	}
	
	Timestamp StringOp::ToTimestamp(std::string const& content)
	{
		//YYYY:MM:DD:HH:MM:SS format
		//YYYYMMDDHHMMSS format
		enum Type
		{
			tNull,
			tDeli,
			tSeq,
		};
		Type type = tNull;
		size_t pos = content.find(':', 0);
		if (pos != std::string::npos)
			type = tDeli;
		else
			type = tSeq;

		int year = 0;
		int month = 0;
		int day = 0;
		int hour = 0;
		int minute = 0;
		int second = 0;

		switch (type)
		{
		case tDeli:
			year = ToInt(content.substr(0, 4));
			month = ToInt(content.substr(5, 2));
			day = ToInt(content.substr(8, 2));
			hour = ToInt(content.substr(11, 2));
			minute = ToInt(content.substr(14, 2));
			second = ToInt(content.substr(17, 2));
			break;
		case tSeq:
			year = ToInt(content.substr(0, 4));
			month = ToInt(content.substr(4, 2));
			day = ToInt(content.substr(6, 2));
			hour = ToInt(content.substr(8, 2));
			minute = ToInt(content.substr(10, 2));
			second = ToInt(content.substr(12, 2));
			break;
		case tNull:
			break;
		default:
			break;
		}
		Timestamp now = Timestamp::GetCurrentTime();
		if (year == 0)
			year = now.GetYear();
		if (month == 0)
			month = now.GetMonth();
		if (day == 0)
			day = now.GetDay();
		if (hour == 0)
			hour = now.GetHour();
		if (minute == 0)
			minute = now.GetMinute();
		if (second == 0)
			second = now.GetSecond();
		Timestamp result(year, month, day, hour, minute, second);
		return result;
	}

	TimeSpan StringOp::ToTimeSpan(std::string const& content)
	{
		/*
		int year = ToInt(content.substr(0, 4));
		int month = ToInt(content.substr(5, 2));
		int day = ToInt(content.substr(8, 2));
		int hour = ToInt(content.substr(11, 2));
		int minute = ToInt(content.substr(14, 2));
		int second = ToInt(content.substr(17, 2));
		TimeSpan result(year * 365 + month * 31 + day, hour, minute, second);
		return result;
		*/
		TimeSpan result(ToInt(content));
		return result;
	}

	CString StringOp::ToCString(std::string const& content)
	{
		return CString(content.c_str());
#if 0
		//get wide content
		//printf("get wide content\n");
		//char* nullTermContent = new char[textInfo->content.length() + 1];
		//memset(nullTermContent, 0, textInfo->content.length() + 1);
		//memcpy(nullTermContent, textInfo->content.c_str(), textInfo->content.length());
		//size_t wideContentLength = MultiByteToWideChar(CP_UTF7, 0, nullTermContent, -1, 0, 0);
		size_t wideContentLength = mbstowcs(0, currentPlayText_->content.c_str(), currentPlayText_->content.length());
		wchar_t* wideContent = new wchar_t[wideContentLength + 2];
		memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
		size_t convLength = MultiByteToWideChar(CP_ACP, 0, currentPlayText_->content.c_str(), currentPlayText_->content.length(), wideContent, wideContentLength);
		//size_t convLength = mbstowcs(wideContent, nullTermContent, wideContentLength + 1);
		//assert(convLength == wideContentLength);
		wprintf(_T("wide content is %s.	calc Length is %d. convLength is %d\n"), wideContent, wideContentLength, convLength);
		//delete[] nullTermContent;
		return wideContent;
#endif
	}
#if 0
	Color StringOp::ToColor(std::string& content)
	{
		Color result;
		result.parts.reserved = 0;
		result.parts.blue = ToInt(content.substr(4, 2), 16);
		result.parts.green = ToInt(content.substr(6, 2), 16);
		result.parts.red = ToInt(content.substr(8, 2), 16);
		return result;
	}

	Rectangle* StringOp::ToRectangle(std::string& content) //[(x0,y0)(x1,y1)]
	{
		Rectangle* result = new Rectangle();
		result->x0 = 0;
		result->y0 = 0;
		result->x1 = 0;
		result->y1 = 0;
		size_t firstB = content.find('(', 0);
		if (firstB == std::string::npos)
			return result;
		else
		{
			size_t firstComma = content.find(',', firstB + 1);
			if (firstComma == std::string::npos)
				return result;
			else
			{
				std::string x0 = content.substr(firstB + 1, firstComma - (firstB + 1));
				result->x0 = ToInt(x0);
				size_t firstE = content.find(')', firstComma + 1);
				if (firstE == std::string::npos)
					return result;
				else
				{
					std::string y0 = content.substr(firstComma + 1, firstE - (firstComma + 1));
					result->y0 = ToInt(y0);
					size_t secondB = content.find('(', firstE + 1);
					if (secondB == std::string::npos)
						return result;
					else
					{
						size_t secondComma = content.find(',', secondB);
						if (secondComma == std::string::npos)
							return result;
						else
						{
							std::string x1 = content.substr(secondB + 1, secondComma - (secondB + 1));
							result->x1 = ToInt(x1);
							size_t secondE = content.find(')', secondComma + 1);
							if (secondE == std::string::npos)
								return result;
							else
							{
								std::string y1 = content.substr(secondComma + 1, secondE - (secondComma + 1));
								result->y1 = ToInt(y1);
							}
						}
					}
				}
			}
		}
		return result;
	}

	LinkMode StringOp::ToEnumLinkMode(std::string& content)
	{
		LinkMode result;
		if (content == "dial")
			result = lmDial;
		else if (content == "direct")
			result = lmDirect;
		return result;
	}

	IPMode StringOp::ToEnumIPMode(std::string& content)
	{
		IPMode result;
		if (content == "auto")
			result = imAuto;
		else if (content == "manual")
			result = imManual;
		return result;
	}

	EncryptionMode StringOp::ToEnumEncryptionMode(std::string& content)
	{
		EncryptionMode result = emNull;
		//@@encryption mode
		return result;
	}

	Position StringOp::ToEnumPosition(std::string& content)
	{
		Position result = pAbove;
		//position
		return result;
	}

	DisplayMode StringOp::ToEnumDisplayMode(std::string& content)
	{
		DisplayMode result = dmNull;
		//@@display mode
		return result;
	}

	VideoDisplayMode StringOp::ToEnumVideoDisplayMode(std::string& content)
	{
		VideoDisplayMode result = vdmNull;
		if (content == "full")
			result = vdmFull;
		else if (content == "fit")
			result = vdmFit;
		return result;
	}
	
	ImageDisplayMode StringOp::ToEnumImageDisplayMode(std::string& content)
	{
		ImageDisplayMode result = idmNull;
		if (content == "Normal")
			result = idmNormal;
		else if (content == "Fadein")
			result = idmFadein;
		return result;
	}
	
	TextDisplayMode StringOp::ToEnumTextDisplayMode(std::string& content)
	{
		TextDisplayMode result = tdmNull;
		if (content == "righttoleft")
			result = tdmRightToLeft;
		else if (content == "lefttoright")
			result = tdmLeftToRight;
		else if (content == "uptodown")
			result = tdmUpToDown;
		else if (content == "downtoup")
			result = tdmDownToUp;
		return result;
	}

	FileType StringOp::ToEnumFileType(std::string& content)
	{
		FileType result = ftError;
		if (content == "transportControl")
			result = ftTransportControl;
		else if (content == "playControl")
			result = ftPlayControl;
		else if (content == "deleteControl")
			result = ftDeleteControl;
		else if (content == "mediaList")
			result = ftMediaList;
		else if (content == "state")
			result = ftState;
		else if (content == "status")
			result = ftStatus;
		else if (content == "log")
			result = ftLog;
		else if (content == "biz")
			result = ftBiz;
		else if (content == "video")
			result = ftVideo;
		else if (content == "image")
			result = ftImage;
		else if (content == "text")
			result = ftText;
		else if (content == "control")
			result = ftControl;
		else if (content == "configure")
			result = ftConfigure;
		else if (content == "billlist")
			result = ftBillList;
		else if (content == "statuslist")
			result = ftStatusList;
		return result;
	}
	
	LightModifyMode StringOp::ToEnumLightModifyMode(std::string& content)
	{
		LightModifyMode result = lAdjust;
		//@@light modify mode
		return result;
	}

	SoundModifyMode StringOp::ToEnumSoundModifyMode(std::string& content)
	{
		SoundModifyMode result = sAdjust;
		//@@sound modify mode
		return result;
	}

	Direction StringOp::ToEnumDirection(std::string& content)
	{
		Direction result = dError;
		if (content == "download")
			result = dDownload;
		else if (content == "upload")
			result = dUpload;
		return result;
	}

	Font StringOp::ToEnumFont(std::string& content)
	{
		Font result = fNull;
		if (content == "Song")
			result = fSong;
		else if (content == "Courier New")
			result = fCourierNew;
		else if (content == "Tahoma")
			result = fTahoma;
		return result;
	}

	FontStyle StringOp::ToEnumFontStyle(std::string& content)
	{
		FontStyle result = fsNull;
		return result;
	}
#endif
	std::string StringOp::FromInt(int const value)
	{
		char stringValue[32] = {0};
		sprintf(stringValue, "%d", value);
		return stringValue;
	}

	std::string StringOp::FromDouble(double const value)
	{
		char stringValue[64] = {0};
		sprintf(stringValue, "%d", value);
		return stringValue;
	}

	std::string StringOp::FromTimestamp(Timestamp const& timestamp)
	{
		char buffer[256] = {0};
		sprintf(buffer, "%04d%02d%02d%02d%02d%02d", timestamp.GetYear(), timestamp.GetMonth(), timestamp.GetDay(), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond());
		std::string result = std::string((char*)buffer);
		return result;
	}

	std::string StringOp::FromTimeSpan(TimeSpan const& timeSpan)
	{
		/*
		char buffer[256] = {0};
		sprintf(buffer, "%04d:%02d:%02d:%02d:%02d:%02d", timeSpan.GetDays() / 365, timeSpan.GetDays() / 30, timeSpan.GetDays() % 30, timeSpan.GetHours(), timeSpan.GetMinutes(), timeSpan.GetSeconds());
		std::string result = std::string((char*)buffer);
		return result;
		*/
		return FromInt(timeSpan.GetTotalSeconds());
	}

	std::string StringOp::FromCString(CString const& value)
	{
#if 0
		CComBSTR nodeName;
		HRESULT hr = item->get_nodeName(&nodeName);
		if (hr != S_OK)
		{
			//throw "XML Parser Error!";
			printf("XML Parser Error!\n");
		}
		
		int length = nodeName.Length();
		char* temp = (char*)malloc(2 * length + 1);
		memset(temp, 0, length + 1);
		wcstombs(temp, nodeName.m_str, 2 * length);
		//memcpy(temp, nodeName.m_str, 2 * length);
		std::string result = temp;
		free(temp);
		return result;
#endif
		int length = wcstombs(0, (LPCTSTR)value, value.GetLength() * 2);
		char* content = (char*)malloc(length + 1);
		memset(content, 0, length + 1);
		//LPCSTR defaultChar = 0;
		//BOOL usedDefaultChar = false;
		//int convLength = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)value, value.GetLength(), content, defaultChar, &usedDefaultChar);
		int convLength = wcstombs(content, (LPCTSTR)value, value.GetLength() * 2);
		std::string result = content;
		free(content);
		return result;
	}
#if 0
	std::string StringOp::FromEnumLogLevel(LogLevel const logLevel)
	{
		std::string result;
		switch (logLevel)
		{
		case llFailureError:
			result = "failure";
			break;
		case llError:
			result = "error";
			break;
		case llWarning:
			result = "warning";
			break;
		case llNotice:
			result = "notice";
			break;
		case llInfo:
			result = "info";
			break;
		default:
			result = "unknown";
			break;
		}
		return result;
	}

	std::string StringOp::FromEnumErrorCode(ErrorCode const errorCode)
	{
		std::string result;
		switch (errorCode)
		{
		case ecSystemError:
			result = "SystemError";
			break;
		case ecPowerFailure:
			result = "PowerFailure";
			break;
		case ecScreenError:
			result = "ScreenError";
			break;
		case ecScreenControllerError:
			result = "ScreenControllerError";
			break;
		case ecSystemOverRun:
			result = "SystemOverRun";
			break;
		case ecTemperature:
			result = "Temperature";
			break;
		case ecMoisture:
			result = "Moisture";
			break;
		case ecShake:
			result = "Shake";
			break;
		case ecWaterLevel:
			result = "WaterLevel";
			break;
		case ecDeviceBox:
			result = "DeviceBox";
			break;
		case ecControlServerConnectionError:
			result = "ControlServerConnectionError";
			break;
		case ecWatchServerConnectionError:
			result = "WatchServerConnectionError";
			break;
		case ecFileDownloadError:
			result = "FileDownloadError";
			break;
		case ecFileUploadError:
			result = "FileUploadError";
			break;
		case ecNetworkUnreached:
			result = "NetworkUnreached";
			break;
		case ecFileChecksumError:
			result = "FileChecksumError";
			break;
		case ecDiskSpaceLow:
		//case ecStorageFreeSpaceLow:
			result = "DiskSpaceLow";
			break;
		case ecOpenFileError:
		//case ecDownloadFileInUsed:
			result = "OpenFileError";
			break;
		case ecLEDConnectionError:
		//case ecLEDScreenConnectError:
			result = "LEDConnectionError";
			break;
		case ecSDCardRemoved:
			result = "SDCardRemoved";
			break;
		case ecFileNotFound:
			result = "FileNotFound";
			break;
		case ecPlayListError:
			result = "PlayListError";
			break;
		case ecMediaFormatNotSupport:
			result = "MediaFormatNotSupport";
			break;
		case ecRemoteUpgradeFailure:
			result = "RemoteUpgradeFailure";
			break;
		case ecFTPServerConnectError:
			result = "FTPServerConnectError";
			break;
		case ecURLError:
			result = "URLError";
			break;
		case ecUpgradeFailure:
			result = "UpgradeFailure";
			break;
		case ecDeviceStarted:
			result = "DeviceStarted";
			break;
		case ecDeviceClosed:
			result = "DeviceClosed";
			break;
		case ecCurrentSoftwareVersion:
			result = "CurrentSoftwareVersion";
			break;
		case ecLEDOpened:
			result = "LEDOpened";
			break;
		case ecLEDClosed:
			result = "LEDClosed";
			break;
		default:
			result = "unknown";
			break;
		}
		return result;
	}

	std::string StringOp::FromEnumFileType(FileType const fileType)
	{
		std::string result;
		switch (fileType)
		{
		case ftVideo:
			result = "video";
			break;
		case ftImage:
			result = "image";
			break;
		default:
			result = "unknown";
			break;
		}
		return result;
	}
#endif
}
