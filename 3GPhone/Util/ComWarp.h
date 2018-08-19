#ifndef __UTIL_COMWARP_H__
#define __UTIL_COMWARP_H__

#include <winbase.h>

namespace Util
{
	class ComWarp
	{
	public:
		ComWarp();
		~ComWarp();

		BOOL OpenComm(const wchar_t* pPort, int nBaudRate=9600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT); 
		BOOL CloseComm(); 
		int ReadComm(void* pData, int nLength); 
		int WriteComm(void* pData, int nLength); 
		// ´®¿ÚÉè±¸¾ä±ú    
		HANDLE hComm;   
	};
}
#endif // __UTIL_COMWARP_H__