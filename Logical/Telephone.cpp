#include "stdafx.h"
#include "Telephone.h"
#if(TEL_IO_VERSION == 1)
#include "Telephone_IO.cpp"
#else
#include "Telephone_rs232.cpp"
#endif