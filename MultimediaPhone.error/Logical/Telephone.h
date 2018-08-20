#ifndef __LOGICAL_TELEPHONE1_H__
#define __LOGICAL_TELEPHONE1_H__
#define TEL_IO_VERSION  0
#if(TEL_IO_VERSION == 1)
#include "Telephone_IO.h"
#else 
#include "Telephone_rs232.h"
#endif

#endif //__LOGICAL_TELEPHONE_H__