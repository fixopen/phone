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
		};//�ļ�����
		
		dataType					dataType_; //��������
		fileType					fileType_; //�ļ�����
		int							nDataLength;
		bool						bHaveNext; //�Ƿ�����һ��
		std::vector<std::string>	vData ;//���ݳ���
		
		//�������
		void					addData(char*data,int length);
		//�������
		std::string				GetData();
		//�Ƿ���һ����
		bool					isOnePacket();
		//�ļ�����				
		 void 					parseDataHeader(char *data);
		//�����ļ�
		static void				SendFile();
		//���ͻ�Ӧ
		void					writeAckToPc();
		//�����󣬴Ӵ�
		void					writeErrorToPc();
	};
};

using namespace USB ;
