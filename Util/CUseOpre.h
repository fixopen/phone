#pragma once
#include <string>

#define YYUSBOPER_EXPORT

extern "C" BOOL  InitUsbFunction();
extern "C"  CloseUsbFunction();
extern "C" BOOL YYUSBOPER_EXPORT CheckUsbState();


class YYUSBOPER_EXPORT CCUseOpre
{
public:
	 CCUseOpre(void);
	 ~CCUseOpre(void);	 
public:
	static BOOL StartReadThread();
	static BOOL openUsbPort();
	static void SendQuest();
	static void SendDBQuest(); //NO USE
	static void SendRingListQuest(); //NO USE
	static BOOL StartReadWrite();
	static void StopReadWrite();
	static void SendFileQuest(std::string FileName,int FileType); //NO USE
	static void SendFileToPc(std::string FileName,int FileType); //NO USE
	static void CloseUsbPort();


};
