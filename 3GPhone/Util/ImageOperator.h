#pragma once

#include "Symbols.h"
#include "DateTime.h"

namespace Util {
    class SYMBOL ImageOperator
    {
    public:
        static void DrawImage(HDC destination, RECT destPosition, CString sourceFilename, RECT sourcePosition);
        static void DrawImage(HDC destination, RECT destPosition, CString sourceFilename);
        static void DrawImage(HDC destination, CString sourceFilename, RECT sourcePosition);
        static void DrawImage(HDC destination, CString sourceFilename);
    private:
        static TCHAR* ImageOperator::findExtension_(const TCHAR * name);
    };
}
