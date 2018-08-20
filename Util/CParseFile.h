#pragma once

#include "Prot.h"
#include "stringOp.h"
#include "PSerialPort.h"
class CCParseFile
{
public:
	CCParseFile(void);
	~CCParseFile(void);
public:
	static void SendORReciveData(CPSerialPort *serialport);
	static void SendFileContent(CPSerialPort *serialport,char * reviceData,int readCount);
	static BOOL parsePacket(CPSerialPort *serialport,char *data,DWORD dwLength);
	static void packageErrorPacket(CPSerialPort *serialport,char *recivedata,int cmdType,char *senddata);
	static void SetPcFileName(std::string fileName);
	static void PackageFilePacket(char *senddata,int packetid,int preCmdType,int cmdType,char *returndata,int opreatorType);
	static void PackageAuthPacket(char *senddata,int packetid,int preCmdType,int cmdType,BOOL isRequest,char *returndata);
	static void PackagePasswordPacket(char *senddata,int packetid,int preCmdType,int cmdType,BOOL isRequest,char * returndata);
	static void SendFileContent(CPSerialPort *serialport,std::string filename,char * senddata,int readcount);
	static void ReadFileTopacket(FILE * fp,char *senddata);
	static void ReciveFileContent(CPSerialPort *serialport,char *reciveData,int reciveCount);
	static void ReciveFileContent(CPSerialPort *serialport,std::string filePath,char *reciveData,int reciveCount);
	static void TelReciveFileContent(CPSerialPort *serialport);
	static int  parsePacketCount(char * fileName);
	static void SerchRecordFile(CPSerialPort *serialport,CString filePath,char *data,int readcount);
	static void SerchRingFile(CPSerialPort *serialport,CString filePath,char *data,int readcount);
	static BOOL IsRoot(CString lpszPath); 
	static BOOL CreatRingList(CString filePath,FILE *fp);
	static BOOL CreatRingListFile();
	static BOOL CreatRingListFile(CString filePath);
	static void resetData();
};
