#pragma once

#include "string"
#include "vector"

#define  SEND_BUFF_LENGTH		1024*2-20
#define  PACKET_HEADER_LENGTH	11

#define  USB_DATA				"0000"
#define  USB_FILE				"0001"

namespace USB{
	class UsbData{
	public:
		UsbData();
		~UsbData();
		
		enum dataType{
			dtNull,
			dtData, 
			dtFile,
		};
		enum fileType{
			ftContactInfo,
			ftRing,
			ftMusic,
			ftPicture,
		};//文件类型
		
		dataType					dataType_; //数据类型
		fileType					fileType_; //文件类型
		int							nDataLength;
		bool						bHaveNext; //是否有下一包
		std::vector<std::string>	vData ;//数据长度
		
		//添加数据
		void					addData(char*data,int length);
		//获得数据
		std::string				GetData();
		//是否是一个包
		bool					isOnePacket();
		//文件类型				
		 void 					parseDataHeader(char *data);
		//发送文件
		static void				SendFile();
		//发送回应
		void					writeAckToPc();
		//包错误，从传
		void					writeErrorToPc();
	};
};

using namespace USB ;
