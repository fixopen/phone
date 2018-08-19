#ifndef __UTIL_DATETIME_H__
#define __UTIL_DATETIME_H__

#include <afx.h> //must include before atltime.h
#include <atltime.h> //for CTime CTimeSpan

namespace Util {
	typedef CTime Timestamp;
	typedef CTimeSpan TimeSpan;
}

#endif //__UTIL_DATETIME_H__