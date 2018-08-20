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
  
  char   buf[256];   
  va_list   ap;   
  
  va_start(ap,   format);   
  int   len   =   vsprintf(buf,   format,   ap);   
  va_end(ap);   
    
  printf(buf); 
  
#endif

  return   0;   
}   