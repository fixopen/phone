#ifndef COMWARP_H
#define COMWARP_H

namespace Util
{
	class ComWarp
	{
	public:
		ComWarp();
		~ComWarp();

		BOOL OpenComm(const WCHAR* pPort, int nBaudRate=9600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=1); 
		BOOL CloseComm(); 
		int ReadComm(void* pData, int nLength); 
		int WriteComm(void* pData, int nLength); 
		// ´®¿ÚÉè±¸¾ä±ú    
		HANDLE hComm;   
	};
}
#endif // COMWARP_H