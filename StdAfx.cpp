// stdafx.cpp : source file that includes just the standard includes
//	MultimediaPhone.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef UNDER_CE

#ifndef _DEBUG

#include <yvals.h>

#include <Afxmt.h>

CCriticalSection g_csForClassLockit;

namespace std
{
	
	CCriticalSection g_csForClassLockit;
	
/*	_Lockit::_Lockit(void)
	{
		
		g_csForClassLockit.Lock();
		
	}
	_Lockit::~_Lockit(void)
	{
		
		g_csForClassLockit.Unlock();
		
	}	*/
};


#endif // !_DEBUG

#endif // UNDER_CE


int  Dprintf(const char *format, ...)
{
#ifdef DEBUG
  char   buf[256];   
  va_list   ap;   
  
  va_start(ap,   format);   
  int   len   =   vsprintf(buf,   format,   ap);   
  va_end(ap);   
    
  printf(buf); 
#endif
//   char   buf[256];   
//   va_list   ap;   
//   
//   va_start(ap,   format);   
//   int   len   =   vsprintf(buf,   format,   ap);   
//   va_end(ap);   
//     
//   printf(buf);
 
	 return   0;
}   

int DMemprintf(char *title)
{
	return 0;

	MEMORYSTATUS   mem; 
    
	mem.dwLength=sizeof(mem); 
    
	GlobalMemoryStatus(&mem); 
    
	return mem.dwAvailPhys; //mem.dwAvailPhys;
}

int DMemprintf(char *title, DWORD &total, DWORD &avalid)
{
	return 0;
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);
	total = mem.dwTotalPhys;
	avalid = mem.dwAvailPhys;
	return mem.dwAvailPhys; //mem.dwAvailPhys;
}