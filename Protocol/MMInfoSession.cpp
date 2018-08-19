#include "stdafx.h"
#include "../Data/OTAData.h"
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "MMInfoSession.h"
#include "../Util/Unzip.h"

MMInfoSession::MMInfoSession(void)
{
}

MMInfoSession::~MMInfoSession(void)
{
}

namespace {
    void unzip(std::wstring const& zipFile, std::wstring const& basePath) {
	    HZIP handle = OpenZip(zipFile.c_str(), 0);

	    SetUnzipBaseDir(handle, basePath.c_str());

	    ZIPENTRY entry;
	    GetZipItem(handle, -1, &entry);
	    for (size_t i = 0; i < entry.index; ++i) { 
		    GetZipItem(handle, i, &entry);
		    UnzipItem(handle, i, entry.name);
	    }

	    CloseZip(handle);
    }
}

void MMInfoSession::selfProcess_(ResponseMessage const& response) const {
	/*
<?xml version="1.0" encoding="GB2312"?>
<vascontent>
<head>
</head>

<slide>
  <layout>
    <region id="image" width="128" height="128" left="150" top="94"/>
    <region id="image1" width="64" height="64" left="95" top="275"/>
    <region id="image2" width="64" height="64" left="315" top="275"/>
    <region id="image3" width="64" height="64" left="543" top="275"/>
    <region id="text"  left="35" top="45"/>
    <region id="text1"  left="300" top="100"/>
    <region id="text2"  left="292" top="140"/>
    <region id="text3"  left="300" top="200"/>
    <region id="text4"  left="240" top="240"/>
    <region id="text5"  left="160" top="292"/>
    <region id="text6"  left="110" top="340"/>
    <region id="text7"  left="150" top="372"/>
    <region id="text8"  left="392" top="292"/>
    <region id="text9"  left="336" top="340"/>
    <region id="text10"  left="370" top="372"/>
    <region id="text11"  left="618" top="292"/>
    <region id="text12"  left="562" top="340"/>
    <region id="text13"  left="596" top="372"/>
  </layout>

  <body>
    <Text value="南京" region="text" font="微软雅黑" fontsize="26" backgroundColor="0x000000" color="0xcccccc"/>
    <Img url="http://10.168.8.104:60265/" src="${SESSION_ID}/W090516175353000001010019" type="image/bmp" region="image" alt="晴"/>

    <Text value="05月19日" region="text1" font="微软雅黑" fontsize="24" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="10℃/25℃" region="text2" font="Arial" fontsize="60" backgroundColor="0x000000" color="0xffff00"/>
    <Text value="大好天气" region="text3" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="适合春游,洗车" region="text4" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>


    <Img url="http://10.168.8.104:60265/" src="${SESSION_ID}/W090516175353000001010019" type="image/bmp" region="image5" alt="晴"/>
    <Text value="05月19日" region="text5" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="10℃/25℃" region="text6" font="微软雅黑" fontsize="26" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="大好天气" region="text7" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>

    <Img url="http://10.168.8.104:60265/" src="${SESSION_ID}/W090516175354000001010031" type="image/bmp" region="image8" alt="风"/>
    <Text value="05月19日" region="text8" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="10℃/25℃" region="text9" font="微软雅黑" fontsize="26" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="大好天气" region="text10" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>

    <Img url="http://10.168.8.104:60265/" src="${SESSION_ID}/W090516175351000001010006" type="image/bmp" region="image11" alt="多云"/>
    <Text value="05月19日" region="text11" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="10℃/25℃" region="text12" font="微软雅黑" fontsize="26" backgroundColor="0x000000" color="0xcccccc"/>
    <Text value="大好天气" region="text13" font="微软雅黑" fontsize="18" backgroundColor="0x000000" color="0xcccccc"/>
  </body>
</slide>
</vascontent>
	*/
	FILE* file = fopen("tmp.zip", "wb");
	if (file) {
		fwrite(response.Body.c_str(), 1, response.Body.length(), file);
		fclose(file);
		unzip(L"tmp.zip", L"/FlashDrv/3G/MMInfo");
		Data::MultimediaDownload md;
		md.multimediaInfos.filename = "/FlashDrv/3G/MMInfo/mmstest";
		md.Insert();
	}
}

std::string const MMInfoSession::resourceName_() const {
    return "ZF3G/tic/mminfo";
}

std::string const MMInfoSession::additionParameter_() const {
    return "&taskid=12345";
}
