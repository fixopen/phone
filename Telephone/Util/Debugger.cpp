#include <Windows.h> //for GlobalMemoryStatus

#include "Debugger.h"

namespace Util {
	MEMORYSTATUS const Debugger::GetMemoryStatus() {
		MEMORYSTATUS result;
		result.dwLength = sizeof(result);
		GlobalMemoryStatus(&result);
		return result;
	}

	void Debugger::Print(const char* format, ...) {
		char buf[256] = {0};
		va_list ap;
		va_start(ap, format);
		int len = vsprintf(buf, format, ap);
		va_end(ap);
		printf(buf);
	}
}
