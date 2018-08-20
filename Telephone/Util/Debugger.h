#ifndef __UTIL_DEBUGGER_H__
#define __UTIL_DEBUGGER_H__

#pragma once

#include <WinBase.h> //for MEMORYSTATUS

namespace Util {
	class Debugger {
	public:
		static MEMORYSTATUS const GetMemoryStatus();
		static void Print(const char* format, ...);
	};
}

#endif //__UTIL_DEBUGGER_H__
