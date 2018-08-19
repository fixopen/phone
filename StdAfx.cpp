// stdafx.cpp : source file that includes just the standard includes
//	MultimediaPhone.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void Dprintf(CString s)
{
#ifdef _DEBUG
	wprintf(s);
	printf("\r\n");
#else
 //	wprintf(s);
 //	printf("\r\n");
#endif
}

int  Dprintf(const char *format, ...)
{
#ifdef _DEBUG
   char   buf[4096];   
   va_list   ap;   
   
   va_start(ap,   format);   
   int   len   =   vsprintf(buf,   format,   ap);   
   va_end(ap);   
     
   printf(buf); 

#else
// 	char   buf[4096];   
// 	va_list   ap;   
// 
// 	va_start(ap,   format);   
// 	int   len   =   vsprintf(buf,   format,   ap);   
// 	va_end(ap);   
// 


// 	printf(buf); 
   
#endif

  return   0;   
}   

int DMemprintf(char *title)
{
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);  
#ifdef _DEBUG
	Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
	return mem.dwAvailPhys; //mem.dwAvailPhys;
#else
#endif
	return mem.dwAvailPhys;
}

int DMemprintf(char *title, DWORD &total, DWORD &avalid)
{
#ifdef _DEBUG
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
	Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
	total = mem.dwTotalPhys;
	avalid = mem.dwAvailPhys;
	return mem.dwAvailPhys; //mem.dwAvailPhys;
#else
#endif
	return 0;
}

void DWriteLog(char *tile, unsigned char *pData, int len)
{
	FILE *file;
	file = fopen("\\flashdrv\\test.log", "a+");
	if(file)
	{
		if(strlen(tile))
			fwrite(tile, sizeof(char), strlen(tile), file);	
	}
	for(int i = 0; i < len; i++)
	{
		char txt[6];
		sprintf(txt, "%x ", *(pData+i));
		fwrite(txt, sizeof(char), strlen(txt), file);	
		
	}
	char *gTxt = "\r\n";
	fwrite(gTxt, sizeof(char), strlen(gTxt), file);	
	fclose(file);
}

void DWriteDat(char *tile, unsigned char *pData, int len)
{
	FILE *file;
	file = fopen("\\flashdrv\\test.dat", "a+");
	if(file)
	{
		if(strlen(tile))
			fwrite(tile, sizeof(char), strlen(tile), file);	
	}
	fwrite(pData, sizeof(char), len, file);	
	fclose(file);
}