#include "stdafx.h"
#include "Telephone.h"
#if(TEL_IO_VERSION == 1)
#include "Telephone_IO.cpp"
#elif(TEL_IO_VERSION == 0)
#include "Telephone_rs232.cpp"
#else
#include "TelephoneWarp.cpp"
#endif