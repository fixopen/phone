
#include "stdafx.h"

#include "../Util/PSerialPort.h"
#include "UsbData.h"
#include "../Util/stringOp.h"

extern CPSerialPort m_pSerial;
extern CFile		g_File ;
extern void		    CloseFile();


UsbData::UsbData()
{

}

UsbData::~UsbData()
{

}

void  UsbData::parseDataHeader(char *data)
{	
	if (strlen(data) < 10)
	{	
		dataType_ = dtNull;
		return ;
	}
	
	//取包头
	dataType dType;
	std::string packet(data,PACKET_HEADER_LENGTH);
	
	if (packet[0] == 'B')
	{	
		size_t t = packet.find('E');//类型
		if (t != std::string::npos)
		{	
			std::string s = packet.substr(t+1,4);
			if (s == USB_DATA )
			{
				dType = dtData;
			}
			else if (s == USB_FILE)
			{
				dType = dtFile ;
				//检测文件类型
			}
			
			//是否有下一包
			bHaveNext = !!atoi(packet.substr(t+5,1).c_str());
		}
	}
	else
	{
		dType = dtNull; //包不对
	}
	
	dataType_ = dType ;
	
}

void UsbData::addData(char*data,int length)
{	
	//从数据中计算数据的长度
	nDataLength	 = atoi(data+1);
	//收到的长度跟实际的长度比较
/*	if (nDataLength != (length - PACKET_HEADER_LENGTH))
	{	
		//清除数据
		vData.clear();
		//错误
		writeErrorToPc();
		//
		CloseFile();
		return ;
	}*/
	 
	switch (dataType_)
	{
	case  dtData://添加数据
		{
			vData.push_back(std::string(data+PACKET_HEADER_LENGTH,nDataLength));
			writeAckToPc();
		}
		break;
	case  dtFile://写文件
		{
			if (g_File.m_hFile != CFile::hFileNull)
			{	
				g_File.Write(data+PACKET_HEADER_LENGTH,length-PACKET_HEADER_LENGTH);
				writeAckToPc();
				if (!bHaveNext)
				{	
					//没下一包,关闭文件
					CloseFile();
				}
			}
		}
		break;
	case dtNull:
		{	
			CloseFile();
			vData.clear();
		}
		break;
	}

}

std::string UsbData::GetData()
{
	std::string data ;
	for(int i = 0 ; i < vData.size() ;i++)
	{		
		data += vData[i] ;
	}
	vData.clear();
	return Util::StringOp::FromUTF8(data);
}

char fileBuff[SEND_BUFF_LENGTH] = {0};
void UsbData::SendFile()
{	
	int nSize = 0 ;
	while(g_File.m_hFile != CFile::hFileNull)
	{	
		int nReadLen = g_File.Read(fileBuff+PACKET_HEADER_LENGTH,\
						SEND_BUFF_LENGTH-PACKET_HEADER_LENGTH);
		if(nReadLen > 0)
		{	
			char head[PACKET_HEADER_LENGTH] = {0}; 
			if (nReadLen == (SEND_BUFF_LENGTH-PACKET_HEADER_LENGTH))
			{	
				sprintf(head,"B%04dE00011",nReadLen);
				memcpy(fileBuff,head,PACKET_HEADER_LENGTH);
			}
			else
			{	
				sprintf(head,"B%04dE00010",nReadLen);
				memcpy(fileBuff,head,PACKET_HEADER_LENGTH);
				CloseFile();
			}
		}
		else
		{	
			memcpy(fileBuff,"B0000E00010",PACKET_HEADER_LENGTH);
			CloseFile();
		}
		
		nSize += nReadLen;
		m_pSerial.WritePort(fileBuff,nReadLen+PACKET_HEADER_LENGTH);
		
	}
}

void UsbData::writeAckToPc()
{
	if (bHaveNext)
	{	
		std::string header = Util::StringOp::ToUTF8("B0001E00000{");
		m_pSerial.WritePort((char *)header.c_str(),header.size());
	}
}

void UsbData::writeErrorToPc()
{	
	char buff[6] = {"Error"};
	m_pSerial.WritePort(buff,6);
}