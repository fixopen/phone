
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
	
	//ȡ��ͷ
	dataType dType;
	std::string packet(data,PACKET_HEADER_LENGTH);
	
	if (packet[0] == 'B')
	{	
		size_t t = packet.find('E');//����
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
				//����ļ�����
			}
			
			//�Ƿ�����һ��
			bHaveNext = !!atoi(packet.substr(t+5,1).c_str());
		}
	}
	else
	{
		dType = dtNull; //������
	}
	
	dataType_ = dType ;
	
}

void UsbData::addData(char*data,int length)
{	
	//�������м������ݵĳ���
	nDataLength	 = atoi(data+1);
	//�յ��ĳ��ȸ�ʵ�ʵĳ��ȱȽ�
/*	if (nDataLength != (length - PACKET_HEADER_LENGTH))
	{	
		//�������
		vData.clear();
		//����
		writeErrorToPc();
		//
		CloseFile();
		return ;
	}*/
	 
	switch (dataType_)
	{
	case  dtData://�������
		{
			vData.push_back(std::string(data+PACKET_HEADER_LENGTH,nDataLength));
			writeAckToPc();
		}
		break;
	case  dtFile://д�ļ�
		{
			if (g_File.m_hFile != CFile::hFileNull)
			{	
				g_File.Write(data+PACKET_HEADER_LENGTH,length-PACKET_HEADER_LENGTH);
				writeAckToPc();
				if (!bHaveNext)
				{	
					//û��һ��,�ر��ļ�
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