#ifndef __UTIL_SYMBOLS_H__
#define __UTIL_SYMBOLS_H__

#ifdef EXPORT
#define SYMBOL __declspec(dllexport)
#elif IMPORT
#define SYMBOL __declspec(dllimport)
#else
#define SYMBOL
#endif

#endif //__UTIL_SYMBOLS_H__
