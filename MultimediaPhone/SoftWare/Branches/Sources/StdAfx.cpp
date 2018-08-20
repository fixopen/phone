// stdafx.cpp : source file that includes just the standard includes
//	MultimediaPhone.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

int  Dprintf(const char *format, ...)
{
#ifdef _DEBUG
  char   buf[256];   
  va_list   ap;   
  
  va_start(ap,   format);   
  int   len   =   vsprintf(buf,   format,   ap);   
  va_end(ap);   
    
  printf(buf); 

#else
 
#endif

  return   0;   
}   

int DMemprintf(char *title)
{
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
	Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
	return mem.dwAvailPhys; //mem.dwAvailPhys;
}

int DMemprintf(char *title, DWORD &total, DWORD &avalid)
{
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
	Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
	total = mem.dwTotalPhys;
	avalid = mem.dwAvailPhys;
	return mem.dwAvailPhys; //mem.dwAvailPhys;
}