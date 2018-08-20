_declspec(dllimport) void __cdecl Atexit(void (__cdecl*)(void));

typedef int Mbstatet;

namespace std {
    using ::_iobuf;

    using ::size_t;
    using ::fpos_t;
    using ::FILE;
    using ::clearerr;
    using ::fclose;
    using ::feof;
    using ::ferror;
    using ::fflush;
    using ::fgetc;
    using ::fgetpos;
    using ::fgets;
    using ::fopen;
    using ::fprintf;
    using ::fputc;
    using ::fputs;
    using ::fread;
    using ::freopen;
    using ::fscanf;
    using ::fseek;
    using ::fsetpos;
    using ::ftell;
    using ::fwrite;
    using ::getc;
    using ::getchar;
    using ::gets;
    using ::perror;
    using ::putc;
    using ::putchar;
    using ::printf;
    using ::puts;
    using ::remove;
    using ::rename;
    using ::rewind;
    using ::scanf;
    using ::setbuf;
    using ::setvbuf;
    using ::sprintf;
    using ::sscanf;
    using ::tmpfile;
    using ::tmpnam;
    using ::ungetc;
    using ::vfprintf;
    using ::vprintf;
    using ::vsprintf;
}

#pragma pack(push,8)

extern "C" {
    typedef int (__cdecl* _onexit_t)(void);
    typedef struct _div_t {
        int quot;
        int rem;
    } div_t;

    typedef struct _ldiv_t {
        long quot;
        long rem;
    } ldiv_t;

#pragma pack(4)
    typedef struct {
        unsigned char ld[10];
    } _LDOUBLE;
#pragma pack()

    typedef struct {
        double x;
    } _CRT_DOUBLE;

    typedef struct {
        float f;
    } _CRT_FLOAT;

    typedef struct {

        long double x;
    } _LONGDOUBLE;

#pragma pack(4)
    typedef struct {
        unsigned char ld12[12];
    } _LDBL12;
#pragma pack()

    __declspec(dllimport) extern int __mb_cur_max;
    __declspec(dllimport) int __cdecl ___mb_cur_max_func(void);
    __declspec(dllimport) int __cdecl ___mb_cur_max_l_func(_locale_t);
    typedef void (__cdecl* _purecall_handler)(void);
    __declspec(dllimport) _purecall_handler __cdecl _set_purecall_handler([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _purecall_handler _Handler);
    __declspec(dllimport) _purecall_handler __cdecl _get_purecall_handler();

    typedef void (__cdecl* _invalid_parameter_handler)(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t);
    __declspec(dllimport) _invalid_parameter_handler __cdecl _set_invalid_parameter_handler([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _invalid_parameter_handler _Handler);
    __declspec(dllimport) _invalid_parameter_handler __cdecl _get_invalid_parameter_handler(void);

    __declspec(dllimport) extern int* __cdecl _errno(void);
    errno_t __cdecl _set_errno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue);
    errno_t __cdecl _get_errno([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* TValue);
    __declspec(dllimport) unsigned long* __cdecl __doserrno(void);
    errno_t __cdecl _set_doserrno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long TValue);
    errno_t __cdecl _get_doserrno([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] unsigned long* TValue);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "strerror" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char** __cdecl __sys_errlist(void);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "strerror" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) int* __cdecl __sys_nerr(void);
    __declspec(dllimport) int*           __cdecl __p___argc(void);
    __declspec(dllimport) char** *       __cdecl __p___argv(void);
    __declspec(dllimport) wchar_t** *    __cdecl __p___wargv(void);
    __declspec(dllimport) char** *       __cdecl __p__environ(void);
    __declspec(dllimport) wchar_t** *    __cdecl __p__wenviron(void);
    __declspec(dllimport) char**         __cdecl __p__pgmptr(void);
    __declspec(dllimport) wchar_t**      __cdecl __p__wpgmptr(void);

    __declspec(dllimport) extern int __argc;
    __declspec(dllimport) extern char** __argv;
    __declspec(dllimport) extern wchar_t** __wargv;

    __declspec(dllimport) extern char** _environ;
    __declspec(dllimport) extern wchar_t** _wenviron;
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_get_pgmptr" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) extern char* _pgmptr;
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_get_wpgmptr" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) extern wchar_t* _wpgmptr;
    errno_t __cdecl _get_pgmptr([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** TValue);
    errno_t __cdecl _get_wpgmptr([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** TValue);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_get_fmode" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) extern int _fmode;

    __declspec(dllimport) errno_t __cdecl _set_fmode([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode);
    __declspec(dllimport) errno_t __cdecl _get_fmode([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PMode);
    extern "C++"
    {
        template <typename _CountofType, size_t _SizeOfArray>
        char(*__countof_helper(_CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];

    }

    __declspec(dllimport) __declspec(noreturn) void __cdecl exit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Code);
    __declspec(dllimport) __declspec(noreturn) void __cdecl _exit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Code);
    __declspec(dllimport) void __cdecl abort(void);
    __declspec(dllimport) unsigned int __cdecl _set_abort_behavior([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Flags, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Mask);

    int     __cdecl abs([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _X);
    long    __cdecl labs([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _X);
    __int64    __cdecl _abs64(__int64);

    int    __cdecl atexit(void (__cdecl*)(void));

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double  __cdecl atof([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double  __cdecl _atof_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport)  int    __cdecl atoi([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl _atoi_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl atol([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl _atol_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) void * __cdecl bsearch_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const void* _Key, [SA_Pre(Null=SA_No, ValidBytes="\n@""_NumOfElements * _SizeOfElements")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* TBase,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _NumOfElements, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeOfElements,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int (__cdecl* _PtFuncCompare)(void*, const void*, const void*), void* _Context);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) void * __cdecl bsearch([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const void* _Key, [SA_Pre(Null=SA_No, ValidBytes="\n@""_NumOfElements * _SizeOfElements")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* TBase,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int (__cdecl* _PtFuncCompare)(const void*, const void*));
    __declspec(dllimport) void __cdecl qsort_s([SA_Pre(Null=SA_No, WritableBytes="\n@""_NumOfElements* _SizeOfElements")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* TBase,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _NumOfElements, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeOfElements,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int (__cdecl* _PtFuncCompare)(void*, const void*, const void*), void* _Context);

    __declspec(dllimport) void __cdecl qsort([SA_Pre(Null=SA_No, WritableBytes="\n@""_NumOfElements * _SizeOfElements")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* TBase,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int (__cdecl* _PtFuncCompare)(const void*, const void*));

[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned short __cdecl _byteswap_ushort([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned short _Short);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned long  __cdecl _byteswap_ulong([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long _Long);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned __int64 __cdecl _byteswap_uint64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 _Int64);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) div_t  __cdecl div([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Numerator, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Denominator);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_dupenv_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char * __cdecl getenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _VarName);

    __declspec(dllimport) errno_t __cdecl getenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _ReturnSize, [SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _VarName);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl getenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _ReturnSize, char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _VarName) {
            return getenv_s(_ReturnSize, Dest, Size, _VarName);
        }
    }

    __declspec(dllimport) errno_t __cdecl _dupenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes, WritableElements="*_PBufferSizeInBytes")] [SA_Post(Deref=2, Valid=SA_Yes)] char** _PBuffer, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PBufferSizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _VarName);

    __declspec(dllimport) errno_t __cdecl _itoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue, [SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _itoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue, char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _itoa_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_itoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _itoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) errno_t __cdecl _i64toa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 Value, [SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_i64toa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char* __cdecl _i64toa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) errno_t __cdecl _ui64toa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ui64toa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char* __cdecl _ui64toa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _atoi64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _atoi64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _strtoi64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _strtoi64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned __int64 __cdecl _strtoui64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned __int64 __cdecl _strtoui64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _String, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int  _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) ldiv_t __cdecl ldiv([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _Numerator, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _Denominator);

    extern "C++"
    {
        inline ldiv_t  div([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _A1, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _A2) {
            return ldiv(_A1, _A2);
        }
    }

    __declspec(dllimport) errno_t __cdecl _ltoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long Value, [SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _ltoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long TValue, char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _ltoa_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ltoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _ltoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl mblen([SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* chr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl _mblen_l([SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* chr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl _mbstrlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl _mbstrlen_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl _mbstrnlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl _mbstrnlen_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) int    __cdecl mbtowc([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstCh, [SA_Pre(Null=SA_Maybe, ValidBytes="_SrcSizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _SrcCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SrcSizeInBytes);
    __declspec(dllimport) int    __cdecl _mbtowc_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstCh, [SA_Pre(Null=SA_Maybe, ValidBytes="_SrcSizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _SrcCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SrcSizeInBytes, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) errno_t __cdecl mbstowcs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="*_PtNumOfCharConverted")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _SrcBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl mbstowcs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount) {
            return mbstowcs_s(_PtNumOfCharConverted, Dest, Size, _Source, _MaxCount);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "mbstowcs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl mbstowcs([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);

    __declspec(dllimport) errno_t __cdecl _mbstowcs_s_l([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="*_PtNumOfCharConverted")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _SrcBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _mbstowcs_s_l([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale) {
            return _mbstowcs_s_l(_PtNumOfCharConverted, Dest, Size, _Source, _MaxCount, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbstowcs_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl _mbstowcs_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl rand(void);
    __declspec(dllimport) int    __cdecl _set_error_mode([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode);

    __declspec(dllimport) void   __cdecl srand([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Seed);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl strtod([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl _strtod_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl strtol([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl _strtol_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned long __cdecl strtoul([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned long __cdecl _strtoul_l(const char* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) int __cdecl system([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Command);

    __declspec(dllimport) errno_t __cdecl _ultoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long Value, [SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _ultoa_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long TValue, char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _ultoa_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ultoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _ultoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "wctomb_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) int    __cdecl wctomb([SA_Pre(Null=SA_Maybe, WritableBytesConst=5)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _MbCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _WCh);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wctomb_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) int    __cdecl _wctomb_l([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _MbCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _WCh, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(dllimport) errno_t __cdecl wctomb_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _SizeConverted, [SA_Pre(Null=SA_Maybe, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="*_SizeConverted")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _MbCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _WCh);

    __declspec(dllimport) errno_t __cdecl _wctomb_s_l([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _SizeConverted, [SA_Pre(Null=SA_Maybe, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _MbCh, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _WCh, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) errno_t __cdecl wcstombs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="*_PtNumOfCharConverted")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCountInBytes);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcstombs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount) {
            return wcstombs_s(_PtNumOfCharConverted, Dest, Size, _Source, _MaxCount);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcstombs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl wcstombs([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(dllimport) errno_t __cdecl _wcstombs_s_l([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="*_PtNumOfCharConverted")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCountInBytes, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcstombs_s_l([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PtNumOfCharConverted, [SA_Pre(Null=SA_Maybe, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale) {
            return _wcstombs_s_l(_PtNumOfCharConverted, Dest, Size, _Source, _MaxCount, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcstombs_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl _wcstombs_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""_NumOfElements* _SizeOfElements")] __declspec(dllimport)  __declspec(noalias) __declspec(restrict)    void * __cdecl calloc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements);
    __declspec(dllimport)                     __declspec(noalias)                                                                             void   __cdecl free([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport)  __declspec(noalias) __declspec(restrict)                              void * __cdecl malloc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                           void * __cdecl realloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""count*Size")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                       void * __cdecl _recalloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    __declspec(dllimport)                     __declspec(noalias)                                                                             void   __cdecl _aligned_free([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                              void * __cdecl _aligned_malloc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                              void * __cdecl _aligned_offset_malloc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                              void * __cdecl _aligned_realloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""count*Size")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                       void * __cdecl _aligned_recalloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                              void * __cdecl _aligned_offset_realloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""count*Size")] __declspec(dllimport) __declspec(noalias) __declspec(restrict)                       void * __cdecl _aligned_offset_recalloc([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport)                                                  size_t __cdecl _aligned_msize([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset);
    __declspec(dllimport) errno_t __cdecl _itow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _itow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue, wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _itow_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_itow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _itow([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) errno_t __cdecl _ltow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long Value, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _ltow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long TValue, wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _ltow_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ltow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _ltow([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) errno_t __cdecl _ultow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long Value, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _ultow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long TValue, wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix) {
            return _ultow_s(TValue, Dest, Size, _Radix);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ultow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _ultow([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long TValue, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl wcstod([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl _wcstod_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl wcstol([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long   __cdecl _wcstol_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned long __cdecl wcstoul([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned long __cdecl _wcstoul_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wdupenv_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) wchar_t * __cdecl _wgetenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _VarName);
    __declspec(dllimport) errno_t __cdecl _wgetenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _ReturnSize, [SA_Pre(Null=SA_No, WritableElements="_DstSizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _VarName);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wgetenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _ReturnSize, wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _VarName) {
            return _wgetenv_s(_ReturnSize, Dest, Size, _VarName);
        }
    }
    __declspec(dllimport) errno_t __cdecl _wdupenv_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes, WritableElements="*_BufferSizeInWords")] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _Buffer, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _BufferSizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _VarName);

    __declspec(dllimport) int __cdecl _wsystem([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Command);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl _wtof([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) double __cdecl _wtof_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wtoi([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wtoi_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long __cdecl _wtol([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long __cdecl _wtol_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(dllimport) errno_t __cdecl _i64tow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 Value, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_i64tow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) wchar_t* __cdecl _i64tow([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) errno_t __cdecl _ui64tow_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ui64tow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) wchar_t* __cdecl _ui64tow([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64   __cdecl _wtoi64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64   __cdecl _wtoi64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64   __cdecl _wcstoi64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64   __cdecl _wcstoi64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned __int64  __cdecl _wcstoui64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) unsigned __int64  __cdecl _wcstoui64_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_ , [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _EndPtr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) char * __cdecl _fullpath([SA_Pre(Null=SA_Maybe, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _FullPath, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Path, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes);

    __declspec(dllimport) errno_t __cdecl _ecvt_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDights, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _ecvt_s(char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double TValue, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign) {
            return _ecvt_s(Dest, Size, TValue, _NumOfDigits, _PtDec, _PtSign);
        }
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ecvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char * __cdecl _ecvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
    __declspec(dllimport) errno_t __cdecl _fcvt_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _fcvt_s(char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double TValue, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign) {
            return _fcvt_s(Dest, Size, TValue, _NumOfDigits, _PtDec, _PtSign);
        }
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char * __cdecl _fcvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
    __declspec(dllimport) errno_t __cdecl _gcvt_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _gcvt_s(char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double TValue, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits) {
            return _gcvt_s(Dest, Size, TValue, _NumOfDigits);
        }
    }
    __declspec(dllimport) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_gcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) char* __cdecl _gcvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atodbl([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CRT_DOUBLE* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atoldbl([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _LDOUBLE* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atoflt([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CRT_FLOAT* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atodbl_l([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CRT_DOUBLE* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atoldbl_l([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _LDOUBLE* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _atoflt_l([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CRT_FLOAT* Result, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned long __cdecl _lrotl([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned long __cdecl _lrotr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
    __declspec(dllimport) errno_t   __cdecl _makepath_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _PathResult, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Ext);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _makepath_s(char(&_Path)[Size], [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Ext) {
            return _makepath_s(_Path, Size, _Drive, _Dir, _Filename, _Ext);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_makepath_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void __cdecl _makepath([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Path, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Ext);
    _onexit_t __cdecl _onexit([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _onexit_t _Func);

#pragma warning (push)
#pragma warning (disable:6540)
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl _putenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _EnvString);
    __declspec(dllimport) errno_t __cdecl _putenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Name, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* TValue);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned int __cdecl _rotl([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned __int64 __cdecl _rotl64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned int __cdecl _rotr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
[returnvalue:SA_Post(MustCheck=SA_Yes)] unsigned __int64 __cdecl _rotr64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned __int64 Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Shift);
#pragma warning (pop)

    __declspec(dllimport) errno_t __cdecl _searchenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _EnvVar, [SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _ResultPath, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _searchenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _EnvVar, char(&_ResultPath)[Size]) {
            return _searchenv_s(_Filename, _EnvVar, _ResultPath, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_searchenv_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void __cdecl _searchenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _EnvVar, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _ResultPath);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_splitpath_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void   __cdecl _splitpath([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _FullPath, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Drive, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dir, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Filename, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Ext);
    __declspec(dllimport) errno_t  __cdecl _splitpath_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _FullPath,
            [SA_Pre(Null=SA_Maybe, WritableElements="_DriveSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Drive, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DriveSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_DirSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dir, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DirSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_FilenameSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _FilenameSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_ExtSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Ext, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ExtSize);
    extern "C++" {
        template <size_t _DriveSize, size_t _DirSize, size_t _NameSize, size_t _ExtSize> inline errno_t __cdecl _splitpath_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const char* Dest, char(&_Drive)[_DriveSize], char(&_Dir)[_DirSize], char(&_Name)[_NameSize], char(&_Ext)[_ExtSize]) {
            return _splitpath_s(Dest, _Drive, _DriveSize, _Dir, _DirSize, _Name, _NameSize, _Ext, _ExtSize);
        }
    }

    __declspec(dllimport) void   __cdecl _swab([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Post(ValidElements="_SizeInBytes")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf1, [SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Post(ValidElements="_SizeInBytes")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf2, int _SizeInBytes);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wfullpath([SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _FullPath, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Path, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);

    __declspec(dllimport) errno_t __cdecl _wmakepath_s([SA_Pre(Null=SA_No, WritableElements="_SIZE")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _PathResult, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SIZE, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Ext);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wmakepath_s(wchar_t (&_ResultPath)[Size], [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Ext) {
            return _wmakepath_s(_ResultPath, Size, _Drive, _Dir, _Filename, _Ext);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wmakepath_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void __cdecl _wmakepath([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _ResultPath, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Drive, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Dir, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Ext);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int    __cdecl _wputenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _EnvString);
    __declspec(dllimport) errno_t __cdecl _wputenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Name, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* TValue);
    __declspec(dllimport) errno_t __cdecl _wsearchenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _EnvVar, [SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _ResultPath, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wsearchenv_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _EnvVar, wchar_t (&_ResultPath)[Size]) {
            return _wsearchenv_s(_Filename, _EnvVar, _ResultPath, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wsearchenv_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void __cdecl _wsearchenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _EnvVar, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _ResultPath);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wsplitpath_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void   __cdecl _wsplitpath([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _FullPath, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Drive, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dir, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Filename, [SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Ext);
    __declspec(dllimport) errno_t __cdecl _wsplitpath_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _FullPath,
            [SA_Pre(Null=SA_Maybe, WritableElements="_DriveSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Drive, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DriveSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_DirSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dir, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DirSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_FilenameSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _FilenameSize,
            [SA_Pre(Null=SA_Maybe, WritableElements="_ExtSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Ext, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ExtSize);
    extern "C++" {
        template <size_t _DriveSize, size_t _DirSize, size_t _NameSize, size_t _ExtSize> inline errno_t __cdecl _wsplitpath_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const wchar_t* _Path, wchar_t (&_Drive)[_DriveSize], wchar_t (&_Dir)[_DirSize], wchar_t (&_Name)[_NameSize], wchar_t (&_Ext)[_ExtSize]) {
            return _wsplitpath_s(_Path, _Drive, _DriveSize, _Dir, _DirSize, _Name, _NameSize, _Ext, _ExtSize);
        }
    }

    __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "SetErrorMode" " instead. See online help for details.")) __declspec(dllimport) void __cdecl _seterrormode([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode);
    __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "Beep" " instead. See online help for details.")) __declspec(dllimport) void __cdecl _beep([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned _Frequency, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned _Duration);
    __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "Sleep" " instead. See online help for details.")) __declspec(dllimport) void __cdecl _sleep([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long _Duration);
#pragma warning(push)
#pragma warning(disable: 4141)
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_ecvt" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ecvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char * __cdecl ecvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fcvt" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char * __cdecl fcvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtDec, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _PtSign);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_gcvt" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))        __declspec(dllimport) char* __cdecl gcvt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] double Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NumOfDigits, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_itoa" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_itoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))        __declspec(dllimport) char* __cdecl itoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_ltoa" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ltoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))        __declspec(dllimport) char* __cdecl ltoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_putenv" ". See online help for details.")) __declspec(dllimport) int    __cdecl putenv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _EnvString);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_swab" ". See online help for details."))                                       __declspec(dllimport) void   __cdecl swab([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInBytes);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_ultoa" ". See online help for details.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ultoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  __declspec(dllimport) char* __cdecl ultoa([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long Value, [SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dstbuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Radix);
#pragma warning(pop)
    _onexit_t __cdecl onexit([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _onexit_t _Func);
}
#pragma pack(pop)

namespace std {

    using ::size_t;
    using ::div_t;
    using ::ldiv_t;

    using ::abort;
    using ::abs;
    using ::atexit;
    using ::atof;
    using ::atoi;
    using ::atol;
    using ::bsearch;
    using ::calloc;
    using ::div;
    using ::exit;
    using ::free;
    using ::getenv;
    using ::labs;
    using ::ldiv;
    using ::malloc;
    using ::mblen;
    using ::mbstowcs;
    using ::mbtowc;
    using ::qsort;
    using ::rand;
    using ::realloc;
    using ::srand;
    using ::strtod;
    using ::strtol;
    using ::strtoul;
    using ::system;
    using ::wcstombs;
    using ::wctomb;
}

extern "C" {
    __declspec(dllimport) void*   __cdecl _memccpy([SA_Pre(Null=SA_Maybe, WritableBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const void *  __cdecl memchr([SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Buffer , [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _memicmp([SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf1, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _memicmp_l([SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf1, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] int     __cdecl memcmp([SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf1, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    void*   __cdecl memcpy([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="Size")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);

    __declspec(dllimport) errno_t  __cdecl memcpy_s([SA_Pre(Null=SA_Maybe, WritableBytes="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    void*   __cdecl memset([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="Size")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);

    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_memccpy" ". See online help for details.")) __declspec(dllimport) void* __cdecl memccpy([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] void* _Dst, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_memicmp" ". See online help for details.")) __declspec(dllimport) int __cdecl memicmp([SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf1, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    __declspec(dllimport) errno_t __cdecl _strset_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strset_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue) {
            return _strset_s(Dest, Size, TValue);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char* __cdecl _strset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TValue);

    __declspec(dllimport) errno_t __cdecl strcpy_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl strcpy_s([SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source) {
            return strcpy_s(Dest, Size, _Source);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "strcpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char* __cdecl strcpy([SA_Pre(Null=SA_No, WritableElementsLength="_Source")] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source);

    __declspec(dllimport) errno_t __cdecl strcat_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl strcat_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source) {
            return strcat_s(Dest, Size, _Source);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "strcat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char* __cdecl strcat([SA_Pre(Null=SA_No, WritableElementsLength="_Source")] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source);
[returnvalue:SA_Post(MustCheck=SA_Yes)] int     __cdecl strcmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] size_t  __cdecl strlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t  __cdecl strnlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);

[returnvalue:SA_Post(MustCheck=SA_Yes)] static __inline size_t  __cdecl strnlen_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount) {
        return (str_ == 0) ? 0 : strnlen(str_, _MaxCount);
    }
    __declspec(dllimport) errno_t __cdecl memmove_s([SA_Pre(Null=SA_Maybe, WritableBytes="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    __declspec(dllimport)  void*   __cdecl memmove([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="Size")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) char *  __cdecl _strdup([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const char *  __cdecl strchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _stricmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strcmpi([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _stricmp_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl strcoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const  char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strcoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _stricoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _stricoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strncoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strncoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strnicoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strnicoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t  __cdecl strcspn([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strerror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char *  __cdecl _strerror([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _ErrMsg);
    __declspec(dllimport) errno_t __cdecl _strerror_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _ErrMsg);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strerror_s(char(&_Buffer)[Size], [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _ErrorMessage) {
            return _strerror_s(_Buffer, Size, _ErrorMessage);
        }
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "strerror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char *  __cdecl strerror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int);

    __declspec(dllimport) errno_t __cdecl strerror_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ErrNum);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl strerror_s(char(&_Buffer)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ErrorMessage) {
            return strerror_s(_Buffer, Size, _ErrorMessage);
        }
    }
    __declspec(dllimport) errno_t __cdecl _strlwr_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strlwr_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&_String)[Size]) {
            return _strlwr_s(_String, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strlwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _strlwr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _String);
    __declspec(dllimport) errno_t __cdecl _strlwr_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strlwr_s_l([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&_String)[Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale) {
            return _strlwr_s_l(_String, Size, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strlwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _strlwr_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(dllimport) errno_t __cdecl strncat_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl strncat_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count) {
            return strncat_s(Dest, Size, _Source, count);
        }
    }
#pragma warning(push)
#pragma warning(disable:6059)

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "strncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl strncat([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count);
#pragma warning(pop)

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl strncmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strnicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _strnicmp_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(dllimport) errno_t __cdecl strncpy_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl strncpy_s(char(&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count) {
            return strncpy_s(Dest, Size, _Source, count);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "strncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl strncpy([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count);
    __declspec(dllimport) errno_t __cdecl _strnset_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strnset_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count) {
            return _strnset_s(Dest, Size, Value, count);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _strnset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const char *  __cdecl strpbrk([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const char *  __cdecl strrchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int chr);
    __declspec(dllimport) char*   __cdecl _strrev([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t  __cdecl strspn([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const char *  __cdecl strstr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _SubStr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "strtok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char *  __cdecl strtok([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Delim);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) char *  __cdecl strtok_s([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char** _Context);

    __declspec(dllimport) errno_t __cdecl _strupr_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strupr_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&_String)[Size]) {
            return _strupr_s(_String, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _strupr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _String);
    __declspec(dllimport) errno_t __cdecl _strupr_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _strupr_s_l([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&_String)[Size], _locale_t _Locale) {
            return _strupr_s_l(_String, Size, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl _strupr_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) size_t  __cdecl strxfrm([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char* _Dst, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(dllimport) size_t  __cdecl _strxfrm_l([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char* _Dst, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int chr) {
            return (char*)strchr((const char*)str_, chr);
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strpbrk([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Control) {
            return (char*)strpbrk((const char*)str_, _Control);
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strrchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int chr) {
            return (char*)strrchr((const char*)str_, chr);
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strstr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _SubStr) {
            return (char*)strstr((const char*)str_, _SubStr);
        }

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline void * __cdecl memchr([SA_Pre(Null=SA_Maybe, ValidBytes="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Pv, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
            return (void*)memchr((const void*)_Pv, _C, _N);
        }

    }

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strdup" ". See online help for details.")) __declspec(dllimport) char * __cdecl strdup([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strcmpi" ". See online help for details.")) __declspec(dllimport) int __cdecl strcmpi([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_stricmp" ". See online help for details.")) __declspec(dllimport) int __cdecl stricmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str2);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strlwr" ". See online help for details.")) __declspec(dllimport) char* __cdecl strlwr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strnicmp" ". See online help for details.")) __declspec(dllimport) int __cdecl strnicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strnset" ". See online help for details.")) __declspec(dllimport) char* __cdecl strnset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strrev" ". See online help for details.")) __declspec(dllimport) char* __cdecl strrev([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strset" ". See online help for details."))         char* __cdecl strset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Value);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strupr" ". See online help for details.")) __declspec(dllimport) char* __cdecl strupr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* str_);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wcsdup([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
    __declspec(dllimport) errno_t __cdecl wcscat_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcscat_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source) {
            return wcscat_s(Dest, Size, _Source);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcscat([SA_Pre(Null=SA_No, WritableElementsLength="_Source")] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const wchar_t * __cdecl wcschr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, wchar_t chr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl wcscmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);

    __declspec(dllimport) errno_t __cdecl wcscpy_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcscpy_s(wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source) {
            return wcscpy_s(Dest, Size, _Source);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcscpy([SA_Pre(Null=SA_No, WritableElementsLength="_Source")] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl wcscspn([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl wcslen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl wcsnlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);

[returnvalue:SA_Post(MustCheck=SA_Yes)] static __inline size_t __cdecl wcsnlen_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount) {
        return (Src == 0) ? 0 : wcsnlen(Src, _MaxCount);
    }
    __declspec(dllimport) errno_t __cdecl wcsncat_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcsncat_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count) {
            return wcsncat_s(Dest, Size, _Source, count);
        }
    }
#pragma warning(push)
#pragma warning(disable:6059)
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsncat([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count);
#pragma warning(pop)
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl wcsncmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);

    __declspec(dllimport) errno_t __cdecl wcsncpy_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcsncpy_s(wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count) {
            return wcsncpy_s(Dest, Size, _Source, count);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsncpy([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t* Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const wchar_t * __cdecl wcspbrk([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const wchar_t * __cdecl wcsrchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t chr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t __cdecl wcsspn([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) const wchar_t * __cdecl wcsstr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _SubStr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcstok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t * __cdecl wcstok([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Delim);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl wcstok_s([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _Context);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcserror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t * __cdecl _wcserror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ErrNum);
    __declspec(dllimport) errno_t __cdecl _wcserror_s([SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ErrNum);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcserror_s(wchar_t (&_Buffer)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Error) {
            return _wcserror_s(_Buffer, Size, _Error);
        }
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "__wcserror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t * __cdecl __wcserror([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);
    __declspec(dllimport) errno_t __cdecl __wcserror_s([SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ErrMsg);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl __wcserror_s(wchar_t (&_Buffer)[Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ErrorMessage) {
            return __wcserror_s(_Buffer, Size, _ErrorMessage);
        }
    }

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsicmp_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsnicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsnicmp_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) errno_t __cdecl _wcsnset_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcsnset_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Dst)[Size], wchar_t Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount) {
            return _wcsnset_s(_Dst, Size, Value, _MaxCount);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcsnset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, wchar_t Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(dllimport) wchar_t* __cdecl _wcsrev([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_);
    __declspec(dllimport) errno_t __cdecl _wcsset_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t TValue);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcsset_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&str_)[Size], wchar_t Value) {
            return _wcsset_s(str_, Size, Value);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcsset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, wchar_t Value);

    __declspec(dllimport) errno_t __cdecl _wcslwr_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcslwr_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_String)[Size]) {
            return _wcslwr_s(_String, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcslwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcslwr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String);
    __declspec(dllimport) errno_t __cdecl _wcslwr_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcslwr_s_l([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_String)[Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale) {
            return _wcslwr_s_l(_String, Size, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcslwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcslwr_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) errno_t __cdecl _wcsupr_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcsupr_s([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_String)[Size]) {
            return _wcsupr_s(_String, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcsupr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String);
    __declspec(dllimport) errno_t __cdecl _wcsupr_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wcsupr_s_l([SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_String)[Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale) {
            return _wcsupr_s_l(_String, Size, _Locale);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wcsupr_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    __declspec(dllimport) size_t __cdecl wcsxfrm([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t* _Dst, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(dllimport) size_t __cdecl _wcsxfrm_l([SA_Pre(Null=SA_Maybe, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t* _Dst, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl wcscoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcscoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsicoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsicoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsncoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsncoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsnicoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wcsnicoll_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
    extern "C++" {
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcschr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* str_, wchar_t chr) {
            return ((wchar_t*)wcschr((const wchar_t*)str_, chr));
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcspbrk([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Control) {
            return ((wchar_t*)wcspbrk((const wchar_t*)str_, _Control));
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcsrchr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t chr) {
            return ((wchar_t*)wcsrchr((const wchar_t*)str_, chr));
        }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcsstr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* str_, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _SubStr) {
            return ((wchar_t*)wcsstr((const wchar_t*)str_, _SubStr));
        }
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsdup" ". See online help for details.")) __declspec(dllimport) wchar_t * __cdecl wcsdup([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsicmp" ". See online help for details.")) __declspec(dllimport) int __cdecl wcsicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsnicmp" ". See online help for details.")) __declspec(dllimport) int __cdecl wcsnicmp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsnset" ". See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsnset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsrev" ". See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsrev([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsset" ". See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsset([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_, wchar_t Value);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcslwr" ". See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcslwr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsupr" ". See online help for details.")) __declspec(dllimport) wchar_t* __cdecl wcsupr([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* str_);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsicoll" ". See online help for details.")) __declspec(dllimport) int __cdecl wcsicoll([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str1, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str2);
}

namespace std {
    using ::size_t;
    using ::memchr;
    using ::memcmp;
    using ::memcpy;
    using ::memmove;
    using ::memset;
    using ::strcat;
    using ::strchr;
    using ::strcmp;
    using ::strcoll;
    using ::strcpy;
    using ::strcspn;
    using ::strerror;
    using ::strlen;
    using ::strncat;
    using ::strncmp;
    using ::strncpy;
    using ::strpbrk;
    using ::strrchr;
    using ::strspn;
    using ::strstr;
    using ::strtok;
    using ::strxfrm;
}

extern "C" {
    __declspec(dllimport) extern unsigned long  __cdecl __threadid(void);

    __declspec(dllimport) extern uintptr_t __cdecl __threadhandle(void);
}

namespace std {
    using ::ptrdiff_t;
    using ::size_t;
}

#pragma pack(push,8)
#pragma warning(push,3)
namespace std {
    enum Uninitialized {
        Noinit
    };
    struct Bool_struct {
        int Member;
    };

    typedef int Bool_struct::* BoolTpe;
}

#pragma warning(push,3)
#pragma pack(push,8)
typedef void (__cdecl* terminate_function)();
typedef void (__cdecl* terminate_handler)();
typedef void (__cdecl* unexpected_function)();
typedef void (__cdecl* unexpected_handler)();
struct _EXCEPTIONTPointerS;

typedef void (__cdecl* _se_translator_function)(unsigned int, struct _EXCEPTIONTPointerS*);
__declspec(dllimport) __declspec(noreturn) void __cdecl terminate(void);
__declspec(dllimport) void __cdecl unexpected(void);

__declspec(dllimport) int __cdecl _is_exceptionTpeof([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const type_info& Type, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] struct _EXCEPTIONTPointerS* _ExceptionPtr);

__declspec(dllimport) terminate_function __cdecl set_terminate([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] terminate_function _NewPtFunc);
extern "C" __declspec(dllimport) terminate_function __cdecl _get_terminate(void);
__declspec(dllimport) unexpected_function __cdecl set_unexpected([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] unexpected_function _NewPtFunc);
extern "C" __declspec(dllimport) unexpected_function __cdecl _get_unexpected(void);
__declspec(dllimport) _se_translator_function __cdecl _set_se_translator([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _se_translator_function _NewPtFunc);

__declspec(dllimport) bool __cdecl __uncaught_exception();

#pragma pack(pop)

#pragma pack(push,8)

extern "C" {
    typedef struct _heapinfo {
        int* _pentry;
        size_t _size;
        int _useflag;
    } _HEAPINFO;
    extern __declspec(deprecated("This function or variable may be unsafe. Consider using " "_get_amblksiz" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) unsigned int _amblksiz;

    __declspec(dllimport) int     __cdecl _resetstkoflw(void);

    __declspec(dllimport) unsigned long __cdecl _set_malloc_crt_max_wait([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned long _NewValue);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) void *  __cdecl _expand([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t  __cdecl _msize([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void* _Memory);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void *          __cdecl _alloca([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) size_t  __cdecl _get_sbh_threshold(void);
    __declspec(dllimport) int     __cdecl _set_sbh_threshold([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewValue);
    __declspec(dllimport) errno_t __cdecl _set_amblksiz([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t TValue);
    __declspec(dllimport) errno_t __cdecl _get_amblksiz([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* TValue);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _heapadd([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void* _Memory, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _heapchk(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int     __cdecl _heapmin(void);
    __declspec(dllimport) int     __cdecl _heapset([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int Fill);
    __declspec(dllimport)  int     __cdecl _heapwalk([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] _HEAPINFO* _EntryInfo);
    __declspec(dllimport) size_t  __cdecl _heapused(size_t* _Used, size_t* _Commit);

    __declspec(dllimport) intptr_t __cdecl _get_heap_handle(void);

    typedef char __static_assert_t[(sizeof(unsigned int) <= 8)];
#pragma warning(push)
#pragma warning(disable:6540)
    __inline void* _MarkAllocaS([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)]  void* Ptr, unsigned int _Marker) {
        if (Ptr) {
            *((unsigned int*)Ptr) = _Marker;
            Ptr = (char*)Ptr + 8;
        }

        return Ptr;
    }
#pragma warning(pop)

    __declspec(noalias) __inline void __cdecl _freea([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory) {
        unsigned int _Marker;

        if (_Memory) {
            _Memory = (char*)_Memory - 8;
            _Marker = *(unsigned int*)_Memory;

            if (_Marker == 0xDDDD) {
                free(_Memory);
            }
        }
    }

}

#pragma pack(pop)

typedef const char* __exString;
extern "C" [returnvalue:SA_Post(MustCheck=SA_Yes)] size_t __cdecl strlen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char*);
extern "C" __declspec(dllimport) errno_t __cdecl strcpy_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* Src);
namespace std {
    class __declspec(dllimport) exception {
    public:
        exception();
        exception(const char* const&);
        exception(const char* const&, int);
        exception(const exception&);
        exception&  operator=(const exception&);
        virtual  ~exception();
        virtual const char*   what() const;

    private:
        const char* _m_what;
        int _m_doFree;
    };

    using ::set_terminate;
    using ::terminate_handler;
    using ::terminate;
    using ::set_unexpected;
    using ::unexpected_handler;
    using ::unexpected;

    typedef void (__cdecl* _Prhand)(const exception&);

    __declspec(dllimport) bool __cdecl uncaught_exception();
}

namespace std {

    class bad_exception : public exception {
    public:
        bad_exception(const char* message = "bad exception")
        throw()
            : exception(message) {
        }

        virtual  ~bad_exception() throw() {
        }
    };
    static const char* _bad_alloc_Message = "bad allocation";

    class bad_alloc : public exception {
    public:
        bad_alloc(const char* message) throw()
            : exception(message) {
        }

        bad_alloc() throw()
            : exception(_bad_alloc_Message, 1) {
        }

        virtual  ~bad_alloc() throw() {
        }

    };
}
#pragma pack(pop)

#pragma warning(push,3)
namespace std {

    typedef void (__cdecl* new_handler)();
    struct nothrow_t {
    };

    extern const nothrow_t nothrow;

    __declspec(dllimport) new_handler __cdecl set_new_handler([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] new_handler)
    throw();
}
void __cdecl operator delete(void*) throw();
#pragma warning (suppress: 4985)
[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void * __cdecl operator new(size_t Size) throw(...);

inline void* __cdecl operator new(size_t, void* Where) throw() {
    return (Where);
}

inline void __cdecl operator delete(void*, void*) throw() {
}
inline void* __cdecl operator new[](size_t, void* Where) throw() {
    return (Where);
}

inline void __cdecl operator delete[](void*, void*) throw() {
}
void __cdecl operator delete[](void*) throw();

[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void * __cdecl operator new[](size_t Size)
throw(...);

[returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] void * __cdecl operator new(size_t Size, const std::nothrow_t&)
throw();

[returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] void * __cdecl operator new[](size_t Size, const std::nothrow_t&)
throw();

void __cdecl operator delete(void*, const std::nothrow_t&)
throw();

void __cdecl operator delete[](void*, const std::nothrow_t&)
throw();
using std::new_handler;
#pragma warning(pop)
#pragma pack(pop)

#pragma pack(push,8)

extern "C" {
    typedef unsigned long _fsize_t;

    struct _wfinddata32_t {
        unsigned    attrib;
        __time32_t  time_create;
        __time32_t  time_access;
        __time32_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
    };

    struct _wfinddata32i64_t {
        unsigned    attrib;
        __time32_t  time_create;
        __time32_t  time_access;
        __time32_t  time_write;
        __int64     size;
        wchar_t     name[260];
    };

    struct _wfinddata64i32_t {
        unsigned    attrib;
        __time64_t  time_create;
        __time64_t  time_access;
        __time64_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
    };

    struct _wfinddata64_t {
        unsigned    attrib;
        __time64_t  time_create;
        __time64_t  time_access;
        __time64_t  time_write;
        __int64     size;
        wchar_t     name[260];
    };
    __declspec(dllimport) const unsigned short* __cdecl __pctype_func(void);

    __declspec(dllimport) extern const unsigned short* _pctype;
    __declspec(dllimport) extern const unsigned short _wctype[];
    __declspec(dllimport) const wctype_t* __cdecl __pwctype_func(void);

    __declspec(dllimport) extern const wctype_t* _pwctype;

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswalpha([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswalpha_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswlower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswlower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswdigit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswdigit_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswxdigit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswxdigit_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswspace([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswspace_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswpunct([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswpunct_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswalnum([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswalnum_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswprint([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswprint_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswgraph([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswgraph_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswcntrl([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswcntrl_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswascii([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl isleadbyte([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isleadbyte_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl towupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _towupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl towlower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _towlower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iswctype([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wctype_t Type);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswctype_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wctype_t Type, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __iswcsymf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswcsymf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __iswcsym([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iswcsym_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "iswctype" " instead. See online help for details.")) __declspec(dllimport) int __cdecl is_wctype([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wctype_t Type);

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wgetcwd([SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wgetdcwd([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive, [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords);
[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] wchar_t * __cdecl _wgetdcwd_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive, [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wchdir([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Path);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wmkdir([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Path);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wrmdir([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Path);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _waccess([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _AccessMode);
    __declspec(dllimport) errno_t __cdecl _waccess_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _AccessMode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wchmod([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wsopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _wcreat([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _PermissionMode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) intptr_t __cdecl _wfindfirst32([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata32_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wfindnext32([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] intptr_t _FindHandle, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata32_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wunlink([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wrename([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _OldFilename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _NewFilename);
    __declspec(dllimport) errno_t __cdecl _wmktemp_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _TemplateName, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wmktemp_s(wchar_t (&_TemplateName)[Size]) {
            return _wmktemp_s(_TemplateName, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wmktemp_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wmktemp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _TemplateName);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) intptr_t __cdecl _wfindfirst32i64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata32i64_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) intptr_t __cdecl _wfindfirst64i32([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata64i32_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) intptr_t __cdecl _wfindfirst64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata64_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wfindnext32i64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] intptr_t _FindHandle, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata32i64_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wfindnext64i32([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] intptr_t _FindHandle, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata64i32_t* _FindData);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wfindnext64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] intptr_t _FindHandle, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _wfinddata64_t* _FindData);

    __declspec(dllimport) errno_t __cdecl _wsopen_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _FileHandle, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _OpenFlag, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ShareFlag, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _PermissionFlag);

    extern "C++" __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wsopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _wopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _OpenFlag, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _PermissionMode = 0);
    extern "C++" __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wsopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _wsopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _OpenFlag, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ShareFlag, int _PermissionMode = 0);
    __declspec(dllimport) wchar_t* __cdecl _wsetlocale([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TCategory, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Locale);
    __declspec(dllimport) intptr_t __cdecl _wexecl([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wexecle([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wexeclp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wexeclpe([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wexecv([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList);
    __declspec(dllimport) intptr_t __cdecl _wexecve([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _Env);
    __declspec(dllimport) intptr_t __cdecl _wexecvp([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList);
    __declspec(dllimport) intptr_t __cdecl _wexecvpe([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _Env);
    __declspec(dllimport) intptr_t __cdecl _wspawnl([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wspawnle([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wspawnlp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wspawnlpe([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ArgList, ...);
    __declspec(dllimport) intptr_t __cdecl _wspawnv([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList);
    __declspec(dllimport) intptr_t __cdecl _wspawnve([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _Env);
    __declspec(dllimport) intptr_t __cdecl _wspawnvp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList);
    __declspec(dllimport) intptr_t __cdecl _wspawnvpe([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _ArgList,
            [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* const* _Env);
    typedef unsigned short _ino_t;
    typedef unsigned short ino_t;

    typedef unsigned int _dev_t;
    typedef unsigned int dev_t;

    typedef long _off_t;
    typedef long off_t;
    struct _stat32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
    };

    struct stat {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
    };

    struct _stat32i64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
    };

    struct _stat64i32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
    };

    struct _stat64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
    };

    __declspec(dllimport) int __cdecl _wstat32([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Name, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _stat32* _Stat);

    __declspec(dllimport) int __cdecl _wstat32i64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Name, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _stat32i64* _Stat);
    __declspec(dllimport) int __cdecl _wstat64i32([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Name, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _stat64i32* _Stat);
    __declspec(dllimport) int __cdecl _wstat64([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Name, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] struct _stat64* _Stat);

    __declspec(dllimport) errno_t __cdecl _cgetws_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="*_SizeRead")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer, size_t _SizeInWords, [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _SizeRead);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _cgetws_s([SA_Post(Deref=1, Null=SA_No, ValidElements="*_Buffer")] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size], size_t* Size) {
            return _cgetws_s(_Buffer, Size, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_cgetws_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _cgetws([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _getwch(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _getwche(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _putwch(wchar_t _WCh);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl _ungetwch(wint_t _WCh);
    __declspec(dllimport) int __cdecl _cputws([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _String);
    __declspec(dllimport) int __cdecl _cwprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _cwprintf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_cwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _cwscanf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_cwscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _cwscanf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _cwscanf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _cwscanf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vcwprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vcwprintf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _cwprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _vcwprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t*  _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _cwprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _cwprintf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vcwprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vcwprintf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _cwprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vcwprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    wint_t __cdecl _putwch_nolock(wchar_t _WCh);
[returnvalue:SA_Post(MustCheck=SA_Yes)] wint_t __cdecl _getwch_nolock(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] wint_t __cdecl _getwche_nolock(void);
    wint_t __cdecl _ungetwch_nolock(wint_t _WCh);

    struct tm {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
    };

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wasctime_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wasctime([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm* _Tm);
    __declspec(dllimport) errno_t __cdecl _wasctime_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElementsConst=26)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm* _Tm);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wasctime_s([SA_Post(Deref=1, Null=SA_No, ValidElementsConst=26)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm* _Time) {
            return _wasctime_s(_Buffer, Size, _Time);
        }
    }

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wctime32_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wctime32([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time32_t* _Time);
    __declspec(dllimport) errno_t __cdecl _wctime32_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElementsConst=26)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time32_t* _Time);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wctime32_s([SA_Post(Deref=1, Null=SA_No, ValidElementsConst=26)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time32_t* _Time) {
            return _wctime32_s(_Buffer, Size, _Time);
        }
    }

    __declspec(dllimport) size_t __cdecl wcsftime([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format,  [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm* _Tm);
    __declspec(dllimport) size_t __cdecl _wcsftime_l([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm* _Tm, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

    __declspec(dllimport) errno_t __cdecl _wstrdate_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElementsConst=9)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wstrdate_s([SA_Post(Deref=1, Null=SA_No, ValidElementsConst=9)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size]) {
            return _wstrdate_s(_Buffer, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wstrdate_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wstrdate([SA_Pre(Null=SA_No, WritableElementsConst=9)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer);

    __declspec(dllimport) errno_t __cdecl _wstrtime_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElementsConst=9)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wstrtime_s([SA_Post(Deref=1, Null=SA_No, ValidElementsConst=9)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size]) {
            return _wstrtime_s(_Buffer, Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wstrtime_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wstrtime([SA_Pre(Null=SA_No, WritableElementsConst=9)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wctime64_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wctime64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time64_t* _Time);
    __declspec(dllimport) errno_t __cdecl _wctime64_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElementsConst=26)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time64_t* _Time);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl _wctime64_s([SA_Post(Deref=1, Null=SA_No, ValidElementsConst=26)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const __time64_t* _Time) {
            return _wctime64_s(_Buffer, Size, _Time);
        }
    }
#pragma once
#pragma warning(push)
#pragma warning(disable:4996)

    static __inline wchar_t* __cdecl _wctime(const time_t* _Time) {
#pragma warning( push )
#pragma warning( disable : 4996 )
        return _wctime64(_Time);
#pragma warning( pop )
    }

    static __inline errno_t __cdecl _wctime_s(wchar_t* _Buffer, size_t _SizeInWords, const time_t* _Time) {
        return _wctime64_s(_Buffer, _SizeInWords, _Time);
    }
#pragma warning(pop)
    typedef int mbstate_t;
    typedef wchar_t _Wint_t;

    __declspec(dllimport) wint_t __cdecl btowc(int);
    __declspec(dllimport) size_t __cdecl mbrlen([SA_Pre(Null=SA_Maybe, ValidBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* chr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes,
            [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    __declspec(dllimport) size_t __cdecl mbrtowc([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstCh, [SA_Pre(Null=SA_Maybe, ValidBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _SrcCh,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    __declspec(dllimport) errno_t __cdecl mbsrtowcs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Pre(Null=SA_Maybe, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_Maybe)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] const char** _PSrc, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl mbsrtowcs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_Maybe)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] const char** _PSource, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State) {
            return mbsrtowcs_s(_Retval, Dest, Size, _PSource, count, _State);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "mbsrtowcs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl mbsrtowcs([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_Maybe)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] const char** _PSrc, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] mbstate_t* _State);

    __declspec(dllimport) errno_t __cdecl wcrtomb_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Pre(Null=SA_Maybe, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t chr, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcrtomb_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Source, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State) {
            return wcrtomb_s(_Retval, Dest, Size, _Source, _State);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcrtomb_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl wcrtomb([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Source, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    __declspec(dllimport) errno_t __cdecl wcsrtombs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Pre(Null=SA_Maybe, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="*_Retval")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst,
            [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] const wchar_t** Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    extern "C++" {
        template <size_t Size> inline errno_t __cdecl wcsrtombs_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _Retval, [SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char(&Dest)[Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] const wchar_t** _PSrc, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State) {
            return wcsrtombs_s(_Retval, Dest, Size, _PSrc, count, _State);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsrtombs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) size_t __cdecl wcsrtombs([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=2, Valid=SA_Yes)] [SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] const wchar_t** _PSource, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] mbstate_t* _State);
    __declspec(dllimport) int __cdecl wctob([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _WCh);

    __declspec(dllimport)  void*   __cdecl memmove([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="Size")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);

    void*   __cdecl memcpy([SA_Pre(Null=SA_Maybe, WritableBytes="Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="Size")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_Maybe, ValidBytes="Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size);

    __declspec(dllimport) errno_t __cdecl memcpy_s([SA_Pre(Null=SA_Maybe, WritableBytes="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);
    __declspec(dllimport) errno_t __cdecl memmove_s([SA_Pre(Null=SA_Maybe, WritableBytes="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _MaxCount);

    __inline int __cdecl fwide([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] FILE* _F, int _M) {
        (void)_F;
        return (_M);
    }
    __inline int __cdecl mbsinit([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const mbstate_t* _P) {
        return (_P == 0 || *_P == 0);
    }
    __inline const wchar_t* __cdecl wmemchr([SA_Pre(Null=SA_No, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* S, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
        for (; 0 < _N; ++S, --_N)
            if (*S == _C)
                return (const wchar_t*)(S);

        return (0);
    }
    __inline int __cdecl wmemcmp([SA_Pre(Null=SA_No, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S1, [SA_Pre(Null=SA_No, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
        for (; 0 < _N; ++_S1, ++_S2, --_N)
            if (*_S1 != *_S2)
                return (*_S1 < *_S2 ? -1 : +1);

        return (0);
    }

    __inline  wchar_t* __cdecl wmemcpy([SA_Pre(Null=SA_Maybe, WritableElements="_N")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _S1, [SA_Pre(Null=SA_Maybe, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
#pragma warning( push )
#pragma warning( disable : 4996 6386 )
        return (wchar_t*)memcpy(_S1, _S2, _N * sizeof(wchar_t));
#pragma warning( pop )
    }
    __inline errno_t __cdecl wmemcpy_s([SA_Pre(Null=SA_Maybe, WritableElements="_N1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _S1, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _N1, [SA_Pre(Null=SA_Maybe, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S2, rsize_t _N) {
#pragma warning( push )
#pragma warning( disable : 6386 )
        return memcpy_s(_S1, _N1 * sizeof(wchar_t), _S2, _N * sizeof(wchar_t));
    }
#pragma warning( pop )
    __inline  wchar_t* __cdecl wmemmove([SA_Pre(Null=SA_Maybe, WritableElements="_N")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _S1, [SA_Pre(Null=SA_Maybe, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
#pragma warning( push )
#pragma warning( disable : 4996 6386 )
#pragma warning( disable : 6387)
        return (wchar_t*)memmove(_S1, _S2, _N * sizeof(wchar_t));
#pragma warning( pop )
    }
    __inline errno_t __cdecl wmemmove_s([SA_Pre(Null=SA_Maybe, WritableElements="_N1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _S1, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _N1, [SA_Pre(Null=SA_Maybe, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _S2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _N) {
#pragma warning( push )
#pragma warning( disable : 6386 )
        return memmove_s(_S1, _N1 * sizeof(wchar_t), _S2, _N * sizeof(wchar_t));
#pragma warning( pop )
    }
    __inline wchar_t* __cdecl wmemset([SA_Pre(Null=SA_No, WritableElements="_N")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* S, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
        wchar_t* _Su = S;

        for (; 0 < _N; ++_Su, --_N) {
            *_Su = _C;
        }

        return (S);
    }
    extern "C++" {
        inline wchar_t* __cdecl wmemchr([SA_Pre(Null=SA_No, ValidElements="_N")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* S, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _N) {
            return (wchar_t*)wmemchr((const wchar_t*)S, _C, _N);
        }
    }
}

#pragma pack(pop)

typedef mbstate_t Mbstatet;
namespace std {
    using ::Mbstatet;

    using ::mbstate_t;
    using ::size_t;
    using ::tm;
    using ::wint_t;
    using ::btowc;
    using ::fgetwc;
    using ::fgetws;
    using ::fputwc;
    using ::fputws;
    using ::fwide;
    using ::fwprintf;
    using ::fwscanf;
    using ::getwc;
    using ::getwchar;
    using ::mbrlen;
    using ::mbrtowc;
    using ::mbsrtowcs;
    using ::mbsinit;
    using ::putwc;
    using ::putwchar;
    using ::swprintf;
    using ::swscanf;
    using ::ungetwc;
    using ::vfwprintf;
    using ::vswprintf;
    using ::vwprintf;
    using ::wcrtomb;
    using ::wprintf;
    using ::wscanf;
    using ::wcsrtombs;
    using ::wcstol;
    using ::wcscat;
    using ::wcschr;
    using ::wcscmp;
    using ::wcscoll;
    using ::wcscpy;
    using ::wcscspn;
    using ::wcslen;
    using ::wcsncat;
    using ::wcsncmp;
    using ::wcsncpy;
    using ::wcspbrk;
    using ::wcsrchr;
    using ::wcsspn;

    using ::wcstod;
    using ::wcstoul;
    using ::wcsstr;

    using ::wcstok;
    using ::wcsxfrm;
    using ::wctob;
    using ::wmemchr;
    using ::wmemcmp;
    using ::wmemcpy;
    using ::wmemmove;
    using ::wmemset;
    using ::wcsftime;
}

#pragma once
#pragma pack(push,8)

extern "C" {
    typedef void* _HFILE;
    typedef int (__cdecl* _CRT_REPORT_HOOK)(int, char*, int*);
    typedef int (__cdecl* _CRT_REPORT_HOOKW)(int, wchar_t*, int*);
    typedef int (__cdecl* _CRT_ALLOC_HOOK)(int, void*, size_t, int, long, const unsigned char*, int);
    typedef void (__cdecl* _CRT_DUMP_CLIENT)(void*, size_t);

    struct _CrtMemBlockHeader;
    typedef struct _CrtMemState {
        struct _CrtMemBlockHeader* pBlockHeader;
        size_t lCounts[5];
        size_t lSizes[5];
        size_t lHighWaterCount;
        size_t lTotalCount;
    } _CrtMemState;

    __declspec(dllimport) extern long _crtAssertBusy;

    __declspec(dllimport) _CRT_REPORT_HOOK __cdecl _CrtGetReportHook(
        void
    );

    __declspec(dllimport) _CRT_REPORT_HOOK __cdecl _CrtSetReportHook(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _CRT_REPORT_HOOK _PFnNewHook
    );

    __declspec(dllimport) int __cdecl _CrtSetReportHook2(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _CRT_REPORT_HOOK _PFnNewHook
    );

    __declspec(dllimport) int __cdecl _CrtSetReportHookW2(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _CRT_REPORT_HOOKW _PFnNewHook
    );

    __declspec(dllimport) int __cdecl _CrtSetReportMode(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReportType,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReportMode
    );

    __declspec(dllimport) _HFILE __cdecl _CrtSetReportFile(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReportType,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _HFILE _ReportFile
    );

    __declspec(dllimport) int __cdecl _CrtDbgReport(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReportType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Linenumber,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _ModuleName,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format,
        ...);

    __declspec(dllimport) size_t __cdecl _CrtSetDebugFillThreshold(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewDebugFillThreshold
    );
    __declspec(dllimport) int __cdecl _CrtDbgReportW(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReportType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ModuleName,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Format,
        ...);
    __declspec(dllimport) extern long _crtBreakAlloc;
    __declspec(dllimport) long __cdecl _CrtSetBreakAlloc(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _BreakAlloc
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport) void * __cdecl _malloc_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""count*Size")] __declspec(dllimport) void * __cdecl _calloc_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t count,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) void * __cdecl _realloc_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""_NumOfElements*_SizeOfElements")] __declspec(dllimport) void * __cdecl _recalloc_dbg
    (
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) void * __cdecl _expand_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

    __declspec(dllimport) void __cdecl _free_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType
    );

    __declspec(dllimport) size_t __cdecl _msize_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType
    );

    __declspec(dllimport) size_t __cdecl _aligned_msize_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport) void * __cdecl _aligned_malloc_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) void * __cdecl _aligned_realloc_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""_NumOfElements*_SizeOfElements")] __declspec(dllimport) void * __cdecl _aligned_recalloc_dbg
    (
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="Size")] __declspec(dllimport) void * __cdecl _aligned_offset_malloc_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t Size,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="_NewSize")] __declspec(dllimport) void * __cdecl _aligned_offset_realloc_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NewSize,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, WritableBytes="\n@""_NumOfElements*_SizeOfElements")] __declspec(dllimport) void * __cdecl _aligned_offset_recalloc_dbg
    (
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _NumOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeOfElements,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Alignment,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Offset,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

    __declspec(dllimport) void __cdecl _aligned_free_dbg(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] void* _Memory
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) char * __cdecl _strdup_dbg(
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* str_,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wcsdup_dbg(
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* str_,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) char * __cdecl _tempnam_dbg(
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _DirName,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _FilePrefix,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wtempnam_dbg(
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _DirName,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _FilePrefix,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) char * __cdecl _fullpath_dbg(
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _FullPath,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Path,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wfullpath_dbg(
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _FullPath,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Path,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) char * __cdecl _getcwd_dbg(
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInBytes,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wgetcwd_dbg(
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) char * __cdecl _getdcwd_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive,
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInBytes,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wgetdcwd_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive,
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] char * __cdecl _getdcwd_lk_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive,
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInBytes,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] [returnvalue:SA_Post(Null=SA_Maybe, NullTerminated=SA_Yes)] [returnvalue:SA_Post(Deref=1, Valid=SA_Yes)] wchar_t * __cdecl _wgetdcwd_lk_dbg(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Drive,
        [SA_Pre(Null=SA_Maybe, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

    __declspec(dllimport) errno_t __cdecl _dupenv_s_dbg(
        [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes, WritableElements="*_PBufferSizeInBytes")] [SA_Post(Deref=2, Valid=SA_Yes)] char** _PBuffer,
        [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PBufferSizeInBytes,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _VarName,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

    __declspec(dllimport) errno_t __cdecl _wdupenv_s_dbg(
        [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe, NullTerminated=SA_Yes, WritableElements="*_PBufferSizeInWords")] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t** _PBuffer,
        [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] size_t* _PBufferSizeInWords,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _VarName,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _BlockType,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _LineNumber
    );

    __declspec(dllimport) _CRT_ALLOC_HOOK __cdecl _CrtGetAllocHook
    (
        void
    );

    __declspec(dllimport) _CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook
    (
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _CRT_ALLOC_HOOK _PfnNewHook
    );
    __declspec(dllimport) extern int _crtDbgFlag;
    __declspec(dllimport) int __cdecl _CrtCheckMemory(
        void
    );

    __declspec(dllimport) int __cdecl _CrtSetDbgFlag(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _NewFlag
    );

    __declspec(dllimport) void __cdecl _CrtDoForAllClientObjects(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] void (__cdecl* _PFn)(void*, void*),
        void* _Context
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _CrtIsValidPointer(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* Ptr,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Bytes,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ReadWrite
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _CrtIsValidHeapPointer(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _HeapPtr
    );

    __declspec(dllimport) int __cdecl _CrtIsMemoryBlock(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Memory,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Bytes,
        [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] long* _RequestNumber,
        [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] char** _Filename,
        [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] int* _LineNumber
    );

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _CrtReportBlockType(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Memory
    );

    __declspec(dllimport) _CRT_DUMP_CLIENT __cdecl _CrtGetDumpClient
    (
        void
    );

    __declspec(dllimport) _CRT_DUMP_CLIENT __cdecl _CrtSetDumpClient
    (
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _CRT_DUMP_CLIENT _PFnNewDump
    );

    __declspec(dllimport)  void __cdecl _CrtMemCheckpoint(
        [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CrtMemState* _State
    );

    __declspec(dllimport)  int __cdecl _CrtMemDifference(
        [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] _CrtMemState* _State,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const _CrtMemState* _OldState,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const _CrtMemState* _NewState
    );

    __declspec(dllimport) void __cdecl _CrtMemDumpAllObjectsSince(
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const _CrtMemState* _State
    );

    __declspec(dllimport) void __cdecl _CrtMemDumpStatistics(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const _CrtMemState* _State
    );

    __declspec(dllimport) int __cdecl _CrtDumpMemoryLeaks(
        void
    );

    __declspec(dllimport) int __cdecl _CrtSetCheckCount(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _CheckCount
    );

    __declspec(dllimport) int __cdecl _CrtGetCheckCount(
        void
    );
}

extern "C++" {

#pragma warning(suppress: 4985)
[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void * __cdecl operator new[](size_t Size);

[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void * __cdecl operator new(
        size_t Size,
        int,
        const char*,
        int
    );

#pragma warning(suppress: 4985)
[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] void * __cdecl operator new[](
        size_t Size,
        int,
        const char*,
        int
    );
    void __cdecl operator delete[](void*);

    inline void __cdecl operator delete(void* _P, int, const char*, int) {
        ::operator delete(_P);
    }

    inline void __cdecl operator delete[](void* _P, int, const char*, int) {
        ::operator delete[](_P);
    }
}
#pragma pack(pop)

#pragma pack(push,8)
#pragma warning(push,3)

namespace std {

    typedef long streamoff;
    typedef int streamsize;

    extern __declspec(dllimport)  fpos_t _Fpz;
    extern __declspec(dllimport)  const streamoff _BADOFF;

    template<class TState>
    class fpos {
        typedef fpos<TState> MyType;

    public:
        fpos(streamoff Offset = 0)
            : _Myoff(Offset), _Fpos(0), _Mystate(_Stz) {
        }

        fpos(TState _State, fpos_t _Fileposition)
            : _Myoff(0), _Fpos(_Fileposition), _Mystate(_State) {
        }

        TState  state() const {
            return (_Mystate);
        }

        void  state(TState _State) {
            _Mystate = _State;
        }

        fpos_t  seekpos() const {
            return (_Fpos);
        }

        operator streamoff() const {
            return ((streamoff)(_Myoff + ((long)(_Fpos))));
        }

        streamoff  operator-(const MyType& right) const {
            return ((streamoff) * this - (streamoff)right);
        }

        MyType&  operator+=(streamoff Offset) {
            _Myoff += Offset;
            return (*this);
        }

        MyType&  operator-=(streamoff Offset) {
            _Myoff -= Offset;
            return (*this);
        }

        MyType  operator+(streamoff Offset) const {
            MyType temp = *this;
            return (temp += Offset);
        }

        MyType  operator-(streamoff Offset) const {
            MyType temp = *this;
            return (temp -= Offset);
        }

        bool  operator==(const MyType& right) const {
            return ((streamoff) * this == (streamoff)right);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }

    private:
        static const TState _Stz;
        streamoff _Myoff;
        fpos_t _Fpos;
        TState _Mystate;
    };

    template<class TState>
    const TState fpos<TState>::_Stz = TState();
    typedef fpos<_Mbstatet> streampos;
    typedef streampos wstreampos;
    struct Unsecure_char_traits_tag {
    };
    struct Secure_charTTraits_tag {
    };
    struct Char_traits_base {
        typedef Unsecure_char_traits_tag Secure_char_traits;
    };
    template<class TElement>
    struct char_traits:
        public Char_traits_base {
        typedef TElement char_type;
        typedef long int_type;
        typedef streampos posTpe;
        typedef streamoff offTpe;
        typedef _Mbstatet stateTpe;

        static void __cdecl assign(TElement& left, const TElement& right) {
            left = right;
        }

        static bool __cdecl eq(const TElement& left, const TElement& right) {
            return (left == right);
        }

        static bool __cdecl lt(const TElement& left, const TElement& right) {
            return (left < right);
        }

        static int __cdecl compare([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first1,
                                   [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2, size_t count) {
            for (; 0 < count; --count, ++first1, ++first2)
                if (!eq(*first1, *first2))
                    return (lt(*first1, *first2) ? -1 : +1);

            return (0);
        }

        static size_t __cdecl length([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first) {
            size_t count;

            for (count = 0; !eq(*first, TElement()); ++first)
                ++count;

            return (count);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl copy([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1,
                                      [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2, size_t count) {
            return Copy_s(first1, count, first2, count);
        }

        static TElement* __cdecl Copy_s([SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t destSize,
                                        [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2, size_t count) {
            {
                if (!(destSize >= count)) {
                    (void)((!!((("destSize >= count", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\iosfwd", 219, 0, L"(\"destSize >= count\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"destSize >= count", __LPREFIX(__FUNCTION__), L"..\\include\\iosfwd", 219, 0);
                    return (0);
                }
            };
            TElement* Next = first1;

            for (; 0 < count; --count, ++Next, ++first2)
                assign(*Next, *first2);

            return (first1);
        }

        static const TElement* __cdecl find([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first,
                                            size_t count, const TElement& chr) {
            for (; 0 < count; --count, ++first)
                if (eq(*first, chr))
                    return (first);

            return (0);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl move([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1,
                                      [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2, size_t count) {
            return Move_s(first1, count, first2, count);
        }

        static TElement* __cdecl Move_s([SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t destSize,
                                        [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2, size_t count) {
            {
                if (!(destSize >= count)) {
                    (void)((!!((("destSize >= count", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\iosfwd", 249, 0, L"(\"destSize >= count\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"destSize >= count", __LPREFIX(__FUNCTION__), L"..\\include\\iosfwd", 249, 0);
                    return (0);
                }
            };
            TElement* Next = first1;

            if (first2 < Next && Next < first2 + count)
                for (Next += count, first2 += count; 0 < count; --count)
                    assign(*--Next, *--first2);
            else
                for (; 0 < count; --count, ++Next, ++first2)
                    assign(*Next, *first2);

            return (first1);
        }

        static TElement* __cdecl assign([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first,
                                        size_t count, TElement chr) {
            TElement* Next = first;

            for (; 0 < count; --count, ++Next)
                assign(*Next, chr);

            return (first);
        }

        static TElement __cdecl to_char_type(const int_type& meta) {
            return ((TElement)meta);
        }

        static int_type __cdecl to_int_type(const TElement& chr) {
            return ((int_type)chr);
        }

        static bool __cdecl eq_int_type(const int_type& left,
                                        const int_type& right) {
            return (left == right);
        }

        static int_type __cdecl eof() {
            return ((int_type)(-1));
        }

        static int_type __cdecl not_eof(const int_type& meta) {
            return (meta != eof() ? (int_type)meta : (int_type)!eof());
        }
    };
template<> struct __declspec(dllimport) char_traits<wchar_t>:
    public Char_traits_base {
        typedef wchar_t TElement;
        typedef TElement char_type;
        typedef wint_t int_type;
        typedef streampos posTpe;
        typedef streamoff offTpe;
        typedef _Mbstatet stateTpe;

    static void __cdecl assign(TElement& left, const TElement& right) {
        left = right;
    }

    static bool __cdecl eq(const TElement& left, const TElement& right) {
        return (left == right);
    }

    static bool __cdecl lt(const TElement& left, const TElement& right) {
        return (left < right);
    }

    static int __cdecl compare([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return (::wmemcmp(first1, first2, count));
    }

    static size_t __cdecl length([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first) {
        return (::wcslen(first));
    }

    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    static TElement* __cdecl copy([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return Copy_s(first1, count, first2, count);
    }

    static TElement* __cdecl Copy_s([SA_Pre(Null=SA_No, WritableElements="_Size_in_words")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t _Size_in_words, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        ::wmemcpy_s((first1), (_Size_in_words), (first2), (count));
        return first1;
    }

    static const TElement* __cdecl find([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first, size_t count,
    const TElement& chr) {
        return ((const TElement*)::wmemchr(first, chr, count));
    }

    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    static TElement* __cdecl move([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return Move_s(first1, count, first2, count);
    }

    static TElement* __cdecl Move_s([SA_Pre(Null=SA_No, WritableElements="_Size_in_words")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t _Size_in_words, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        ::wmemmove_s((first1), (_Size_in_words), (first2), (count));
        return (TElement*)first1;
    }

    static TElement* __cdecl assign([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first, size_t count, TElement chr) {
        return ((TElement*)::wmemset(first, chr, count));
    }

    static TElement __cdecl to_char_type(const int_type& meta) {
        return (meta);
    }

    static int_type __cdecl to_int_type(const TElement& chr) {
        return (chr);
    }

    static bool __cdecl eq_int_type(const int_type& left,
    const int_type& right) {
        return (left == right);
    }

    static int_type __cdecl eof() {
        return ((wint_t)(0xFFFF));
    }

    static int_type __cdecl not_eof(const int_type& meta) {
        return (meta != eof() ? meta : !eof());
    }
    };

template<> struct __declspec(dllimport) char_traits<char>:
    public Char_traits_base {
        typedef char TElement;
        typedef TElement char_type;
        typedef int int_type;
        typedef streampos posTpe;
        typedef streamoff offTpe;
        typedef _Mbstatet stateTpe;

    static void __cdecl assign(TElement& left, const TElement& right) {
        left = right;
    }

    static bool __cdecl eq(const TElement& left, const TElement& right) {
        return (left == right);
    }

    static bool __cdecl lt(const TElement& left, const TElement& right) {
        return (left < right);
    }

    static int __cdecl compare([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return (::memcmp(first1, first2, count));
    }

    static size_t __cdecl length([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first) {
        return (::strlen(first));
    }

    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    static TElement* __cdecl copy([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return Copy_s(first1, count, first2, count);
    }

    static TElement* __cdecl Copy_s([SA_Pre(Null=SA_No, WritableElements="size_in_bytes")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t size_in_bytes, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        ::memcpy_s((first1), (size_in_bytes), (first2), (count));
        return first1;
    }

    static const TElement* __cdecl find([SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first, size_t count,
    const TElement& chr) {
        return ((const TElement*)::memchr(first, chr, count));
    }

    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    static TElement* __cdecl move([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        return Move_s(first1, count, first2, count);
    }

    static TElement* __cdecl Move_s([SA_Pre(Null=SA_No, WritableElements="size_in_bytes")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first1, size_t size_in_bytes, [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const TElement* first2,
    size_t count) {
        ::memmove_s((first1), (size_in_bytes), (first2), (count));
        return first1;
    }

    static TElement* __cdecl assign([SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] TElement* first, size_t count, TElement chr) {
        return ((TElement*)::memset(first, chr, count));
    }

    static TElement __cdecl to_char_type(const int_type& meta) {
        return ((TElement)meta);
    }

    static int_type __cdecl to_int_type(const TElement& chr) {
        return ((unsigned char)chr);
    }

    static bool __cdecl eq_int_type(const int_type& left,
    const int_type& right) {
        return (left == right);
    }

    static int_type __cdecl eof() {
        return ((-1));
    }

    static int_type __cdecl not_eof(const int_type& meta) {
        return (meta != eof() ? meta : !eof());
    }
    };
    template <class TTraits>
    class Inherits_from_char_traits_base {
        typedef char True;
        class False {
            char Dummy[2];
        };
        static True Inherits(Char_traits_base);
        static False Inherits(...);
        static TTraits Make_traits();

    public:
        enum { Exists = sizeof(Inherits(Make_traits())) == sizeof(True) };
    };

    template <class TTraits, bool Inherits_from_char_traits_base>
    class Char_traits_category_helper {
    public:
        typedef Unsecure_char_traits_tag Secure_char_traits;
    };

    template <class TElement>
    class Char_traits_category_helper< char_traits<TElement> , true> {
    public:
        typedef Secure_charTTraits_tag Secure_char_traits;
    };

    template <class TTraits>
    class Char_traits_category_helper<TTraits, true> {
    public:
        typedef typename TTraits::Secure_char_traits Secure_char_traits;
    };

    template <class TTraits>
    class Char_traits_category {
    public:
        typedef typename Char_traits_category_helper<TTraits, Inherits_from_char_traits_base<TTraits>::Exists>::Secure_char_traits Secure_char_traits;
    };

    template<class TTraits>
    inline
    typename Char_traits_category<TTraits>::Secure_char_traits Char_traits_cat() {
        typename Char_traits_category<TTraits>::Secure_char_traits Secure;
        return (Secure);
    }

    namespace Traits_helper {
        template<class TTraits>
        inline
        typename TTraits::char_type* copy_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count) {
            return copy_s<TTraits>(first1, Size, first2, count, Char_traits_cat<TTraits>());
        }

        template<class TTraits>
        inline
        typename TTraits::char_type* copy_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count, Secure_charTTraits_tag) {
            return TTraits::Copy_s(first1, Size, first2, count);
        }
        template<class TTraits>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename TTraits::char_type* copy_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count, Unsecure_char_traits_tag) {
            return TTraits::copy(first1, first2, count);
        }
        template<class TTraits>
        inline
        typename TTraits::char_type* move_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count) {
            return move_s<TTraits>(first1, Size, first2, count, Char_traits_cat<TTraits>());
        }

        template<class TTraits>
        inline
        typename TTraits::char_type* move_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count, Secure_charTTraits_tag) {
            return TTraits::Move_s(first1, Size, first2, count);
        }
        template<class TTraits>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename TTraits::char_type* move_s([SA_Pre(Null=SA_No, WritableElements="Size")] [SA_Pre(Deref=1, Valid=SA_No)] typename TTraits::char_type* first1, size_t Size,
                                            [SA_Pre(Null=SA_No, ValidElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const typename TTraits::char_type* first2, size_t count, Unsecure_char_traits_tag) {
            return TTraits::move(first1, first2, count);
        }

    }
    template<class T1>
    class allocator;
    class ios_base;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_ios;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class istreambufTIteratorator;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class ostreambufTIteratorator;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_streambuf;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_istream;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_ostream;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_iostream;
    template < class TElement,
             class TTraits = char_traits<TElement>,
             class TAllocator = allocator<TElement> >
    class basic_stringbuf;
    template < class TElement,
             class TTraits = char_traits<TElement>,
             class TAllocator = allocator<TElement> >
    class basic_istringstream;
    template < class TElement,
             class TTraits = char_traits<TElement>,
             class TAllocator = allocator<TElement> >
    class basic_ostringstream;
    template < class TElement,
             class TTraits = char_traits<TElement>,
             class TAllocator = allocator<TElement> >
    class basic_stringstream;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_filebuf;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_ifstream;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_ofstream;
    template < class TElement,
             class TTraits = char_traits<TElement> >
    class basic_fstream;
    template < class TElement,
             class TInputIterator >
    class num_get;
    template < class TElement,
             class TOutputIterator >
    class num_put;
    template<class TElement>
    class collate;

    typedef basic_ios<char, char_traits<char> > ios;
    typedef basic_streambuf<char, char_traits<char> > streambuf;
    typedef basic_istream<char, char_traits<char> > istream;
    typedef basic_ostream<char, char_traits<char> > ostream;
    typedef basic_iostream<char, char_traits<char> > iostream;
    typedef basic_stringbuf < char, char_traits<char>,
            allocator<char> > stringbuf;
    typedef basic_istringstream < char, char_traits<char>,
            allocator<char> > istringstream;
    typedef basic_ostringstream < char, char_traits<char>,
            allocator<char> > ostringstream;
    typedef basic_stringstream < char, char_traits<char>,
            allocator<char> > stringstream;
    typedef basic_filebuf<char, char_traits<char> > filebuf;
    typedef basic_ifstream<char, char_traits<char> > ifstream;
    typedef basic_ofstream<char, char_traits<char> > ofstream;
    typedef basic_fstream<char, char_traits<char> > fstream;
    typedef basic_ios<wchar_t, char_traits<wchar_t> > wios;
    typedef basic_streambuf<wchar_t, char_traits<wchar_t> >
    wstreambuf;
    typedef basic_istream<wchar_t, char_traits<wchar_t> > wistream;
    typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;
    typedef basic_iostream<wchar_t, char_traits<wchar_t> > wiostream;
    typedef basic_stringbuf < wchar_t, char_traits<wchar_t>,
            allocator<wchar_t> > wstringbuf;
    typedef basic_istringstream < wchar_t, char_traits<wchar_t>,
            allocator<wchar_t> > wistringstream;
    typedef basic_ostringstream < wchar_t, char_traits<wchar_t>,
            allocator<wchar_t> > wostringstream;
    typedef basic_stringstream < wchar_t, char_traits<wchar_t>,
            allocator<wchar_t> > wstringstream;
    typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;
    typedef basic_ifstream<wchar_t, char_traits<wchar_t> > wifstream;
    typedef basic_ofstream<wchar_t, char_traits<wchar_t> > wofstream;
    typedef basic_fstream<wchar_t, char_traits<wchar_t> > wfstream;
    typedef num_get<char, istreambufTIteratorator<char, char_traits<char> > >
    numget;
    typedef num_get<wchar_t, istreambufTIteratorator<wchar_t, char_traits<wchar_t> > >
    wnumget;
    typedef num_put<char, ostreambufTIteratorator<char, char_traits<char> > >
    numput;
    typedef num_put<wchar_t, ostreambufTIteratorator<wchar_t, char_traits<wchar_t> > >
    wnumput;
    typedef collate<char> ncollate;
    typedef collate<wchar_t> wcollate;
}
#pragma warning(pop)
#pragma pack(pop)
#pragma pack(push,8)
#pragma warning(push,3)
namespace std {

    template<class T1> inline
    void swap(T1& left, T1& right) {
        if (&left != &right) {
            T1 temp = left;
            left = right;
            right = temp;
        }
    }
    template<class T1> inline
    void Swap_adl(T1& left, T1& right) {
        swap(left, right);
    }
    template < class T1,
             class T2 >
    struct pair {
        typedef pair<T1, T2> MyType;
        typedef T1 first_type;
        typedef T2 second_type;

        pair()
            : first(T1()), second(T2()) {
        }

        pair(const T1& value1, const T2& value2)
            : first(value1), second(value2) {
        }

        template < class TOther1,
                 class TOther2 >
        pair(const pair<TOther1, TOther2>& right)
            : first(right.first), second(right.second) {
        }

        void swap(MyType& right) {
            if (this != &right) {
                ::std:: Swap_adl(first, right.first);
                ::std:: Swap_adl(second, right.second);
            }
        }

        T1 first;
        T2 second;
    };
    template < class T1,
             class T2 > inline
    void swap(pair<T1, T2>& left, pair<T1, T2>& right) {
        left.swap(right);
    }

    template < class T1,
             class T2 > inline
    bool operator==(const pair<T1, T2>& left,
                    const pair<T1, T2>& right) {
        return (left.first == right.first && left.second == right.second);
    }

    template < class T1,
             class T2 > inline
    bool operator!=(const pair<T1, T2>& left,
                    const pair<T1, T2>& right) {
        return (!(left == right));
    }

    template < class T1,
             class T2 > inline
    bool operator<(const pair<T1, T2>& left,
                   const pair<T1, T2>& right) {
        return (left.first < right.first ||
                !(right.first < left.first) && left.second < right.second);
    }

    template < class T1,
             class T2 > inline
    bool operator>(const pair<T1, T2>& left,
                   const pair<T1, T2>& right) {
        return (right < left);
    }

    template < class T1,
             class T2 > inline
    bool operator<=(const pair<T1, T2>& left,
                    const pair<T1, T2>& right) {
        return (!(right < left));
    }

    template < class T1,
             class T2 > inline
    bool operator>=(const pair<T1, T2>& left,
                    const pair<T1, T2>& right) {
        return (!(left < right));
    }

    template < class T1,
             class T2 > inline
    pair<T1, T2> make_pair(T1 value1, T2 value2) {
        return (pair<T1, T2>(value1, value2));
    }
    namespace rel_ops {
        template<class T1> inline
        bool  operator!=(const T1& left, const T1& right) {
            return (!(left == right));
        }

        template<class T1> inline
        bool operator>(const T1& left, const T1& right) {
            return (right < left);
        }

        template<class T1> inline
        bool operator<=(const T1& left, const T1& right) {
            return (!(right < left));
        }

        template<class T1> inline
        bool operator>=(const T1& left, const T1& right) {
            return (!(left < right));
        }
    }
}
namespace std {
    namespace tr1 {
        template<class TTuple>
        struct tuple_size;
        template < int Index,
                 class TTuple >
        struct tupleTElementent;
        template < class T1,
                 class T2 >
        struct tuple_size<std::pair<T1, T2> > {
            static const int value = 2;
        };

        template < int Index,
                 class T1 >
        struct Pair_data;
        template < class T1,
                 class T2 >
        struct Pair_data<0, std::pair<T1, T2> > {
            typedef T1& Type;
            typedef const T1& CType;

            static Type Value(std::pair<T1, T2>& pr) {
                return (pr.first);
            }

            static CType Value(const std::pair<T1, T2>& pr) {
                return (pr.first);
            }
        };

        template < class T1,
                 class T2 >
        struct Pair_data<1, std::pair<T1, T2> > {
            typedef T2& Type;
            typedef const T2& CType;

            static Type Value(std::pair<T1, T2>& pr) {
                return (pr.second);
            }

            static CType Value(const std::pair<T1, T2>& pr) {
                return (pr.second);
            }
        };

        template < class T1,
                 class T2 >
        struct tupleTElementent<0, std::pair<T1, T2> > {
            typedef T1 Type;
        };

        template < class T1,
                 class T2 >
        struct tupleTElementent<1, std::pair<T1, T2> > {
            typedef T2 Type;
        };

        template < int Index,
                 class T1,
                 class T2 >
        typename Pair_data<Index, std::pair<T1, T2> >::Type
        get(std::pair<T1, T2>& pr) {
            return (Pair_data<Index, std::pair<T1, T2> >::Value(pr));
        }

        template < int Index,
                 class T1,
                 class T2 >
        typename Pair_data<Index, std::pair<T1, T2> >::CType
        get(const std::pair<T1, T2>& pr) {
            return (Pair_data<Index, std::pair<T1, T2> >::Value(pr));
        }

    }
}

#pragma warning(pop)
#pragma pack(pop)

#pragma pack(push,8)
#pragma warning(push,3)
namespace std {
    struct Unchecked_iterator_tag {
    };
    struct Range_checked_iterator_tag {
    };
    struct Unchanged_checked_iterator_base_type_tag {
    };

    struct Difference_checked_iterator_base_type_tag {
    };
    struct Undefined_inner_type_tag {
    };
    struct Undefined_move_tag {
    };

    struct Swap_move_tag {
    };
    class Iterator_base_universal {
    public:
        typedef Unchecked_iterator_tag Checked_iterator_category;
        typedef Unchanged_checked_iterator_base_type_tag Checked_iterator_base_type;
        typedef Undefined_inner_type_tag Inner_type;
    };

    __declspec(noreturn) __declspec(deprecated) __declspec(dllimport) void __cdecl Debug_message(const char*, const char*);
    __declspec(dllimport) void __cdecl Debug_message(const wchar_t*, const wchar_t*, unsigned int line);

    class Iterator_base;

    class __declspec(dllimport) Container_base_secure {
    public:
        friend class Iterator_base;

        Container_base_secure()
            : myFirstIterator_(0) {
        }

        Container_base_secure(const Container_base_secure&)
            : myFirstIterator_(0) {
        }

        Container_base_secure&  operator=(const Container_base_secure&) {
            return (*this);
        }

        ~Container_base_secure() {
            Orphan_all();
        }

        void  Orphan_all() const;
        void  Swap_all(Container_base_secure&) const;

        void  Swap_aux(Container_base_secure&) {
        }

        Iterator_base* myFirstIterator_;
    };

    typedef Container_base_secure Container_base;
    class Iterator_base : public Iterator_base_universal {
    public:
        friend class Container_base_secure;

        Iterator_base()
            : myContainer_(0), myNextIterator_(0) {
        }

        Iterator_base(const Iterator_base& right)
            : myContainer_(0), myNextIterator_(0) {
            *this = right;
        }

        Iterator_base&  operator=(const Iterator_base& right) {
            if (myContainer_ != right.myContainer_) {
                Lockit lock(3);
                Orphan_me();
                Adopt(right.myContainer_);
            }

            return (*this);
        }

        ~Iterator_base() {
            Lockit lock(3);
            Orphan_me();
        }

        void  Adopt(const Container_base_secure* parent) {
            if (myContainer_ != parent) {
                Lockit lock(3);
                Orphan_me();

                if (parent != 0 && parent->myFirstIterator_ != ((Iterator_base*) - 3)) {
                    myNextIterator_ = parent->myFirstIterator_;
                    ((Container_base_secure*)parent)->myFirstIterator_ = this;
                }

                myContainer_ = parent;
            }
        }

        void  Orphan_me() {
            if (myContainer_ != 0 && myContainer_->myFirstIterator_ != ((Iterator_base*) - 3)) {
                Iterator_base** pNext =
                    (Iterator_base**)&myContainer_->myFirstIterator_;

                while (*pNext != 0 && *pNext != this)
                    pNext = &(*pNext)->myNextIterator_;

                if (*pNext == 0)
                    Debug_message(L"ITERATOR LIST CORRUPTED!", L"..\\include\\xutility", 189);

                *pNext = myNextIterator_;
                myContainer_ = 0;
            }
        }

        const Container_base_secure*   Getmycontainer() const {
            return myContainer_;
        }

        bool  Same_container(const Iterator_base& Other) const {
            return myContainer_ == Other.myContainer_;
        }

        bool  Has_container() const {
            return myContainer_ != 0;
        }

        const Container_base_secure* myContainer_;
        Iterator_base* myNextIterator_;
    };

    typedef Iterator_base Iterator_base_secure;

    inline void  Container_base_secure::Orphan_all() const {
        Lockit lock(3);

        if (myFirstIterator_ != ((Iterator_base*) - 3)) {
            for (Iterator_base** pNext = (Iterator_base**)&myFirstIterator_;
                    *pNext != 0; *pNext = (*pNext)->myNextIterator_)
                (*pNext)->myContainer_ = 0;

            *(Iterator_base**)&myFirstIterator_ = 0;
        }
    }

    inline void  Container_base_secure::Swap_all(Container_base_secure& right) const {
        Lockit lock(3);
        Iterator_base* pNext;
        Iterator_base* temp = (Iterator_base*)myFirstIterator_;
        *(Iterator_base**)&myFirstIterator_ = (Iterator_base*)right.myFirstIterator_;
        *(Iterator_base**)&right.myFirstIterator_ = temp;

        if (myFirstIterator_ != ((Iterator_base*) - 3)) {
            for (pNext = (Iterator_base*)myFirstIterator_;
                    pNext != 0; pNext = pNext->myNextIterator_)
                pNext->myContainer_ = this;
        }

        if (right.myFirstIterator_ != ((Iterator_base*) - 3)) {
            for (pNext = (Iterator_base*)right.myFirstIterator_;
                    pNext != 0; pNext = pNext->myNextIterator_)
                pNext->myContainer_ = &right;
        }
    }
    template<class T1, class T2> inline
    bool __cdecl Debug_lt(const T1& left, const T2& right,
                          const wchar_t* Where, unsigned int Line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class T1, class T2> inline
    bool __cdecl Debug_lt(const T1& left, T2& right,
                          const wchar_t* Where, unsigned int Line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class T1, class T2> inline
    bool __cdecl Debug_lt(T1& left, const T2& right,
                          const wchar_t* Where, unsigned int Line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class T1, class T2> inline
    bool __cdecl Debug_lt(T1& left, T2& right,
                          const wchar_t* Where, unsigned int Line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class pr, class T1, class T2> inline
    bool __cdecl Debug_lt_pred(pr Pred, const T1& left, const T2& right,
                               const wchar_t* Where, unsigned int Line) {
        if (!Pred(left, right))
            return (false);
        else if (Pred(right, left))
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class pr, class T1, class T2> inline
    bool __cdecl Debug_lt_pred(pr Pred, const T1& left, T2& right,
                               const wchar_t* Where, unsigned int Line) {
        if (!Pred(left, right))
            return (false);
        else if (Pred(right, left))
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class pr, class T1, class T2> inline
    bool __cdecl Debug_lt_pred(pr Pred, T1& left, const T2& right,
                               const wchar_t* Where, unsigned int Line) {
        if (!Pred(left, right))
            return (false);
        else if (Pred(right, left))
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class pr, class T1, class T2> inline
    bool __cdecl Debug_lt_pred(pr Pred, T1& left, T2& right,
                               const wchar_t* Where, unsigned int Line) {
        if (!Pred(left, right))
            return (false);
        else if (Pred(right, left))
            Debug_message(L"invalid operator<", Where, Line);

        return (true);
    }

    template<class TAllocator>
    class Container_base_aux_alloc_empty
            : public Container_base {
    protected:
        explicit Container_base_aux_alloc_empty(TAllocator) { }

        Container_base_aux_alloc_empty(const Container_base_aux_alloc_empty&) { }

        Container_base_aux_alloc_empty& operator=(const Container_base_aux_alloc_empty&) {
            return *this;
        }

        ~Container_base_aux_alloc_empty() { }
    };

    struct input_iterator_tag {
    };

    struct output_iterator_tag {
    };

    struct forward_iterator_tag
            : public input_iterator_tag {
    };

    struct bidirectional_iterator_tag
            : public forward_iterator_tag {
    };

    struct random_access_iterator_tag
            : public bidirectional_iterator_tag {
    };

    struct Int_iterator_tag {
    };

    struct Float_iterator_tag {
    };
    struct Nonscalar_ptr_iterator_tag {
    };
    struct Scalar_ptr_iterator_tag {
    };
    template < class TCategory,
             class T1,
             class TDifference = ptrdiff_t,
             class TPointer = T1*,
             class TReference = T1& >
    struct iterator
            : public Iterator_base_universal

    {
        typedef TCategory iterator_category;
        typedef T1 value_type;
        typedef TDifference difference_type;
        typedef TDifference distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    template < class TCategory,
             class T1,
             class TDifference = ptrdiff_t,
             class TPointer = T1*,
             class TReference = T1&,
             class TBase_class = Iterator_base >
    struct Iterator_with_base
            : public TBase_class

    {
        typedef TCategory iterator_category;
        typedef T1 value_type;
        typedef TDifference difference_type;
        typedef TDifference distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    template < class T1,
             class TDifference,
             class TPointer,
             class TReference >
    struct BidirectionIterator
            : public Iterator_with_base < bidirectional_iterator_tag,
              T1, TDifference, TPointer, TReference > {
    };

    template < class T1,
             class TDifference,
             class TPointer,
             class TReference >
    struct RandomIterator
            : public Iterator_with_base < random_access_iterator_tag,
              T1, TDifference, TPointer, TReference > {
    };

    template < class T1,
             class TDifference,
             class TPointer,
             class TReference,
             class TBase_class >
    struct RandomIterator_base
            : public TBase_class

    {
        typedef random_access_iterator_tag iterator_category;
        typedef T1 value_type;
        typedef TDifference difference_type;
        typedef TDifference distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    struct OutputIterator
            : public iterator < output_iterator_tag, void, void,
              void, void > {
    };
    template<class TIterator>
    struct iterator_traits {
        typedef typename TIterator::iterator_category iterator_category;
        typedef typename TIterator::value_type value_type;
        typedef typename TIterator::difference_type difference_type;
        typedef difference_type distance_type;
        typedef typename TIterator::pointer pointer;
        typedef typename TIterator::reference reference;
    };

    template<class T1>
    struct iterator_traits<T1*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T1 value_type;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
        typedef T1* pointer;
        typedef T1& reference;
    };

    template<class T1>
    struct iterator_traits<const T1*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T1 value_type;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
        typedef const T1* pointer;
        typedef const T1& reference;
    };

    template<> struct iterator_traits<_Bool> {
        typedef Int_iterator_tag iterator_category;
        typedef _Bool value_type;
        typedef _Bool difference_type;
        typedef _Bool distance_type;
        typedef _Bool* pointer;
        typedef _Bool& reference;
    };

    template<> struct iterator_traits<char> {
        typedef Int_iterator_tag iterator_category;
        typedef char value_type;
        typedef char difference_type;
        typedef char distance_type;
        typedef char* pointer;
        typedef char& reference;
    };

    template<> struct iterator_traits<signed char> {
        typedef Int_iterator_tag iterator_category;
        typedef signed char value_type;
        typedef signed char difference_type;
        typedef signed char distance_type;
        typedef signed char* pointer;
        typedef signed char& reference;
    };

    template<> struct iterator_traits<unsigned char> {
        typedef Int_iterator_tag iterator_category;
        typedef unsigned char value_type;
        typedef unsigned char difference_type;
        typedef unsigned char distance_type;
        typedef unsigned char* pointer;
        typedef unsigned char& reference;
    };
    template<> struct iterator_traits<wchar_t> {
        typedef Int_iterator_tag iterator_category;
        typedef wchar_t value_type;
        typedef wchar_t difference_type;
        typedef wchar_t distance_type;
        typedef wchar_t* pointer;
        typedef wchar_t& reference;
    };
    template<> struct iterator_traits<short> {
        typedef Int_iterator_tag iterator_category;
        typedef short value_type;
        typedef short difference_type;
        typedef short distance_type;
        typedef short* pointer;
        typedef short& reference;
    };

    template<> struct iterator_traits<unsigned short> {
        typedef Int_iterator_tag iterator_category;
        typedef unsigned short value_type;
        typedef unsigned short difference_type;
        typedef unsigned short distance_type;
        typedef unsigned short* pointer;
        typedef unsigned short& reference;
    };

    template<> struct iterator_traits<int> {
        typedef Int_iterator_tag iterator_category;
        typedef int value_type;
        typedef int difference_type;
        typedef int distance_type;
        typedef int* pointer;
        typedef int& reference;
    };

    template<> struct iterator_traits<unsigned int> {
        typedef Int_iterator_tag iterator_category;
        typedef unsigned int value_type;
        typedef unsigned int difference_type;
        typedef unsigned int distance_type;
        typedef unsigned int* pointer;
        typedef unsigned int& reference;
    };

    template<> struct iterator_traits<long> {
        typedef Int_iterator_tag iterator_category;
        typedef long value_type;
        typedef long difference_type;
        typedef long distance_type;
        typedef long* pointer;
        typedef long& reference;
    };

    template<> struct iterator_traits<unsigned long> {
        typedef Int_iterator_tag iterator_category;
        typedef unsigned long value_type;
        typedef unsigned long difference_type;
        typedef unsigned long distance_type;
        typedef unsigned long* pointer;
        typedef unsigned long& reference;
    };

    template<> struct iterator_traits<double> {
        typedef Float_iterator_tag iterator_category;
        typedef double value_type;
        typedef double difference_type;
        typedef double distance_type;
        typedef double* pointer;
        typedef double& reference;
    };

    template<> struct iterator_traits<float> {
        typedef Float_iterator_tag iterator_category;
        typedef float value_type;
        typedef float difference_type;
        typedef float distance_type;
        typedef float* pointer;
        typedef float& reference;
    };
    template<> struct iterator_traits<__int64> {
        typedef Int_iterator_tag iterator_category;
        typedef __int64 value_type;
        typedef __int64 difference_type;
        typedef __int64 distance_type;
        typedef __int64* pointer;
        typedef __int64& reference;
    };

    template<> struct iterator_traits<unsigned __int64> {
        typedef Int_iterator_tag iterator_category;
        typedef unsigned __int64 value_type;
        typedef unsigned __int64 difference_type;
        typedef unsigned __int64 distance_type;
        typedef unsigned __int64* pointer;
        typedef unsigned __int64& reference;
    };

    template<class TIterator> inline
    typename iterator_traits<TIterator>::iterator_category
    __cdecl Iterator_cat(const TIterator&) {
        typename iterator_traits<TIterator>::iterator_category Cat;
        return (Cat);
    }
    template<class TCategory1, class TCategory2>
    class Iterator_random_helper {
    public:
        typedef forward_iterator_tag Iterator_random_cat;
    };

    template<>
    class Iterator_random_helper<random_access_iterator_tag, random_access_iterator_tag> {
    public:
        typedef random_access_iterator_tag Iterator_random_cat;
    };

    template<class TCategory1, class TCategory2, class TCategory3>
    class TIterator_random_helper3 {
    public:
        typedef forward_iterator_tag Iterator_random_cat;
    };

    template<>
    class TIterator_random_helper3<random_access_iterator_tag, random_access_iterator_tag, random_access_iterator_tag> {
    public:
        typedef random_access_iterator_tag Iterator_random_cat;
    };

    template<class TIterator1, class TIterator2> inline
    typename Iterator_random_helper <
    typename iterator_traits<TIterator1>::iterator_category,
             typename iterator_traits<TIterator2>::iterator_category >::Iterator_random_cat
    __cdecl Iterator_random(const TIterator1&, const TIterator2&) {
        typename Iterator_random_helper <
        iterator_traits<TIterator1>::iterator_category,
                        iterator_traits<TIterator2>::iterator_category >::Iterator_random_cat Cat;
        return (Cat);
    }

    template<class TIterator1, class TIterator2, class TIterator3> inline
    typename TIterator_random_helper3 <
    typename iterator_traits<TIterator1>::iterator_category,
             typename iterator_traits<TIterator2>::iterator_category,
             typename iterator_traits<TIterator3>::iterator_category >::Iterator_random_cat
    __cdecl Iterator_random(const TIterator1&, const TIterator2&, const TIterator3&) {
        typename TIterator_random_helper3 <
        iterator_traits<TIterator1>::iterator_category,
                        iterator_traits<TIterator2>::iterator_category,
                        iterator_traits<TIterator3>::iterator_category >::Iterator_random_cat Cat;
        return (Cat);
    }

    template <bool TCondition, class T1, class T2>
    class If {
    public:
        typedef T2 Result;
    };

    template <class T1, class T2>
    class If<true, T1, T2> {
    public:
        typedef T1 Result;
    };
    template <bool Secure_validation>
    class Secure_validation_helper {
    public:
        typedef Unchecked_iterator_tag Checked_iterator_category;
    };

    template <>
    class Secure_validation_helper<true> {
    public:
        typedef Range_checked_iterator_tag Checked_iterator_category;
    };

    template <class TIterator, bool Inherits_from_iterator_base>
    class Checked_iterator_category_helper {
    public:
        typedef Unchecked_iterator_tag Checked_cat;
    };

    template <class TIterator>
    class Checked_iterator_category_helper<TIterator, true> {
    public:
        typedef typename TIterator::Checked_iterator_category Checked_cat;
    };

    template <class TIterator>
    class Checked_iterator_category {
    public:
        typedef typename Checked_iterator_category_helper<TIterator, __is_base_of(Iterator_base_universal, TIterator)>::Checked_cat Checked_cat;
    };

    template<class TIterator>
    inline
    typename Checked_iterator_category<TIterator>::Checked_cat Checked_cat(const TIterator&) {
        typename Checked_iterator_category<TIterator>::Checked_cat Cat;
        return (Cat);
    }
    template <class TIterator, bool Inherits_from_iterator_base>
    class Checked_iterator_base_helper2 {
    public:
        typedef Unchanged_checked_iterator_base_type_tag Checked_iterator_base_type;
    };

    template <class TIterator>
    class Checked_iterator_base_helper2<TIterator, true> {
    public:
        typedef typename TIterator::Checked_iterator_base_type Checked_iterator_base_type;
    };

    template <class TIterator, class TBase>
    class Checked_iterator_base_helper1 {
    public:
        typedef Difference_checked_iterator_base_type_tag Base_type_tag;
        typedef TBase Checked_iterator_base_type;
    };

    template <class TIterator>
    class Checked_iterator_base_helper1<TIterator, Unchanged_checked_iterator_base_type_tag> {
    public:
        typedef Unchanged_checked_iterator_base_type_tag Base_type_tag;
        typedef TIterator Checked_iterator_base_type;
    };

    template <class TIterator>
    class Checked_iterator_base_helper {
    public:
        typedef Checked_iterator_base_helper2<TIterator, __is_base_of(Iterator_base_universal, TIterator)> Base_helper2;
        typedef Checked_iterator_base_helper1<TIterator, typename  Base_helper2::Checked_iterator_base_type > Base_helper1;

        typedef typename Base_helper1::Base_type_tag Checked_iterator_base_type_tag;
        typedef typename Base_helper1::Checked_iterator_base_type Checked_iterator_base_type;
    };

    template<class TIterator, class TBase_tag>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(const TIterator& TIterator, TBase_tag) {
        return TIterator.Checked_iterator_base();
    }

    template<class TIterator>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(const TIterator& TIterator, Unchanged_checked_iterator_base_type_tag) {
        return TIterator;
    }

    template<class TIterator, class TBase_tag>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(TIterator& TIterator, TBase_tag) {
        return TIterator.Checked_iterator_base();
    }

    template<class TIterator>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(TIterator& TIterator, Unchanged_checked_iterator_base_type_tag) {
        return TIterator;
    }

    template<class TIterator>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(const TIterator& TIterator) {
        typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type_tag TBase_tag;
        return Checked_base(TIterator, TBase_tag);
    }

    template<class TIterator>
    inline
    typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type
    __cdecl Checked_base(TIterator& TIterator) {
        typename Checked_iterator_base_helper<TIterator>::Checked_iterator_base_type_tag TBase_tag;
        return Checked_base(TIterator, TBase_tag);
    }

    template<class TDstIterator, class TBaseIterator>
    inline
    void __cdecl Checked_assign_from_base(TDstIterator& Dest, const TBaseIterator& Src) {
        Dest.Checked_iterator_assign_from_base(Src);
    }

    template<class TIterator>
    inline
    void __cdecl Checked_assign_from_base(TIterator& Dest, const TIterator& Src) {
        Dest = Src;
    }

    template <class TValue>
    class Move_operation_category {
    public:
        typedef Undefined_move_tag Move_cat;
    };

    template<class TIterator>
    inline
    typename Move_operation_category<typename iterator_traits<TIterator>::value_type>::Move_cat Move_cat(const TIterator&) {
        typename Move_operation_category<typename iterator_traits<TIterator>::value_type>::Move_cat Cat;
        return (Cat);
    }

    template<class T1>
    struct Is_swap_move {
        static const bool TValue = false;
    };

    template<>
    struct Is_swap_move<Swap_move_tag> {
        static const bool TValue = true;
    };

    template < class T1,
             class T2 >
    class Move_operation_category<pair<T1, T2> > {
    public:
        typedef typename If <
        Is_swap_move < typename Move_operation_category <
        T1 >::Move_cat >::TValue ||
        Is_swap_move < typename Move_operation_category <
        T2 >::Move_cat >::TValue,
           Swap_move_tag,
           Undefined_move_tag >::Result Move_cat;
    };
    template<class T1, class T2, class TCheckedCat1, class TCheckedCat2>
    struct Ptr_cat_with_checked_cat_helper {
        typedef Nonscalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T1,  class T2>
    struct Ptr_cat_helper {
        typedef typename Ptr_cat_with_checked_cat_helper < T1, T2,
                typename Checked_iterator_category<T1>::Checked_cat,
                typename Checked_iterator_category<T2>::Checked_cat >::Ptr_cat Ptr_cat;
    };

    template<class T1,  class T2, class TCheckedCat1>
    struct Ptr_cat_with_checked_cat_helper<T1, T2, TCheckedCat1, Range_checked_iterator_tag> {
        typedef typename Ptr_cat_helper<T1, typename T2::Inner_type>::Ptr_cat Ptr_cat;
    };

    template<class T1,  class T2, class TCheckedCat2>
    struct Ptr_cat_with_checked_cat_helper<T1, T2, Range_checked_iterator_tag, TCheckedCat2> {
        typedef typename Ptr_cat_helper<typename T1::Inner_type, T2>::Ptr_cat Ptr_cat;
    };

    template<class T1,  class T2>
    struct Ptr_cat_with_checked_cat_helper<T1, T2, Range_checked_iterator_tag, Range_checked_iterator_tag> {
        typedef typename Ptr_cat_helper<typename T1::Inner_type, typename T2::Inner_type>::Ptr_cat Ptr_cat;
    };

    template<class T1>
    struct Ptr_cat_helper<T1, Undefined_inner_type_tag> {
        typedef Nonscalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T2>
    struct Ptr_cat_helper<Undefined_inner_type_tag, T2> {
        typedef Nonscalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<Undefined_inner_type_tag, Undefined_inner_type_tag> {
        typedef Nonscalar_ptr_iterator_tag Ptr_cat;
    };
    template<>
    struct Ptr_cat_helper<_Bool*, _Bool*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const _Bool*, _Bool*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<char*, char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const char*, char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<signed char*, signed char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const signed char*, signed char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<unsigned char*, unsigned char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const unsigned char*, unsigned char*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<wchar_t*, wchar_t*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const wchar_t*, wchar_t*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };
    template<>
    struct Ptr_cat_helper<short*, short*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const short*, short*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<unsigned short*, unsigned short*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const unsigned short*, unsigned short*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<int*, int*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const int*, int*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<unsigned int*, unsigned int*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const unsigned int*, unsigned int*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<long*, long*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const long*, long*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<unsigned long*, unsigned long*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const unsigned long*, unsigned long*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<float*, float*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const float*, float*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<double*, double*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const double*, double*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<long double*, long double*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const long double*, long double*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<__int64*, __int64*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const __int64*, __int64*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<unsigned __int64*, unsigned __int64*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<>
    struct Ptr_cat_helper<const unsigned __int64*, unsigned __int64*> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T1>
    struct Ptr_cat_helper<T1**, T1**> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T1>
    struct Ptr_cat_helper<T1**, const T1**> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T1>
    struct Ptr_cat_helper<T1* const*, T1**> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };

    template<class T1>
    struct Ptr_cat_helper<T1* const*, const T1**> {
        typedef Scalar_ptr_iterator_tag Ptr_cat;
    };
    template<class T1, class T2> inline
    typename Ptr_cat_helper<T1, T2>::Ptr_cat __cdecl Ptr_cat(T1&, T2&) {
        typename Ptr_cat_helper<T1, T2>::Ptr_cat Cat;
        return (Cat);
    }

    template<class TInputIterator> inline
    void __cdecl Debug_pointer(TInputIterator&, const wchar_t*, unsigned int) {
    }

    template<class T1> inline
    void __cdecl Debug_pointer(const T1* first, const wchar_t* File, unsigned int Line) {
        if (first == 0)
            Debug_message(L"invalid null pointer", File, Line);
    }

    template<class T1> inline
    void __cdecl Debug_pointer(T1* first, const wchar_t* File, unsigned int Line) {
        if (first == 0)
            Debug_message(L"invalid null pointer", File, Line);
    }
    template<class TInputIterator> inline
    void __cdecl Debug_range2(TInputIterator, TInputIterator, const wchar_t*, unsigned int ,
                              input_iterator_tag) {
    }

    template<class TRandomIterator> inline
    void __cdecl Debug_range2(TRandomIterator first, TRandomIterator Last, const wchar_t* File, unsigned int Line,
                              random_access_iterator_tag) {
        if (first != Last) {
            Debug_pointer(first, File, Line);
            Debug_pointer(Last, File, Line);

            if (Last < first)
                Debug_message(L"invalid iterator range", File, Line);
        }
    }

    template<class TInputIterator> inline
    void __cdecl Debug_range(TInputIterator first, TInputIterator Last, const wchar_t* File, unsigned int Line) {
        Debug_range2(first, Last, File, Line, Iterator_cat(first));
    }
    template<class TInputIterator> inline
    void __cdecl Debug_order2(TInputIterator first, TInputIterator Last,
                              const wchar_t* File, unsigned int Line, input_iterator_tag) {
    }

    template<class TForwardIterator> inline
    void __cdecl Debug_order2(TForwardIterator first, TForwardIterator Last,
                              const wchar_t* File, unsigned int Line, forward_iterator_tag) {
        if (first != Last)
            for (TForwardIterator Next = first; ++Next != Last; ++first)
                if (Debug_lt(*Next, *first, L"..\\include\\xutility", 1614))
                    Debug_message(L"sequence not ordered", File, Line);
    }

    template<class TInputIterator> inline
    void __cdecl Debug_order(TInputIterator first, TInputIterator Last,
                             const wchar_t* File, unsigned int Line) {
        Debug_range(first, Last, File, Line);
        Debug_order2(first, Last, File, Line, Iterator_cat(first));
    }
    template < class TInputIterator,
             class pr > inline
    void __cdecl Debug_order2(TInputIterator first, TInputIterator Last, pr Pred,
                              const wchar_t* File, unsigned int Line, input_iterator_tag) {
    }

    template < class TForwardIterator,
             class pr > inline
    void __cdecl Debug_order2(TForwardIterator first, TForwardIterator Last, pr Pred,
                              const wchar_t* File, unsigned int Line, forward_iterator_tag) {
        if (first != Last)
            for (TForwardIterator Next = first; ++Next != Last; ++first)
                if (Debug_lt_pred(Pred, *Next, *first, L"..\\include\\xutility", 1641))
                    Debug_message(L"sequence not ordered", File, Line);
    }

    template < class TInputIterator,
             class pr > inline
    void __cdecl Debug_order(TInputIterator first, TInputIterator Last, pr Pred,
                             const wchar_t* File, unsigned int Line) {
        Debug_range(first, Last, File, Line);
        Debug_pointer(Pred, File, Line);
        Debug_order2(first, Last, Pred, File, Line, Iterator_cat(first));
    }
    template<class TInputIterator> inline
    void __cdecl Debug_order_single2(TInputIterator first, TInputIterator Last, bool IsFirstIteration,
                                     const wchar_t* File, unsigned int Line, input_iterator_tag) {
    }

    template<class TForwardIterator> inline
    void __cdecl Debug_order_single2(TForwardIterator first, TForwardIterator Last, bool ,
                                     const wchar_t* File, unsigned int Line, forward_iterator_tag) {
        if (first != Last) {
            TForwardIterator Next = first;

            if (++Next != Last)
                if (Debug_lt(*Next, *first, L"..\\include\\xutility", 1670))
                    Debug_message(L"sequence not ordered", File, Line);
        }
    }

    template<class TInputIterator> inline
    void __cdecl Debug_order_single(TInputIterator first, TInputIterator Last, bool IsFirstIteration,
                                    const wchar_t* File, unsigned int Line) {
        Debug_order_single2(first, Last, IsFirstIteration, File, Line, Iterator_cat(first));
    }
    template < class TInputIterator,
             class pr > inline
    void __cdecl Debug_order_single2(TInputIterator first, TInputIterator Last, pr Pred, bool IsFirstIteration,
                                     const wchar_t* File, unsigned int Line, input_iterator_tag) {
    }

    template < class TForwardIterator,
             class pr > inline
    void __cdecl Debug_order_single2(TForwardIterator first, TForwardIterator Last, pr Pred, bool IsFirstIteration,
                                     const wchar_t* File, unsigned int Line, forward_iterator_tag) {
        if (first != Last) {
            TForwardIterator Next = first;

            if (++Next != Last)
                if (Debug_lt_pred(Pred, *Next, *first, L"..\\include\\xutility", 1699))
                    Debug_message(L"sequence not ordered", File, Line);
        }
    }

    template < class TInputIterator,
             class pr > inline
    void __cdecl Debug_order_single(TInputIterator first, TInputIterator Last, pr Pred, bool IsFirstIteration,
                                    const wchar_t* File, unsigned int Line) {
        Debug_order_single2(first, Last, Pred, IsFirstIteration, File, Line, Iterator_cat(first));
    }
    template<class TIterator> inline
    typename iterator_traits<TIterator>::value_type* __cdecl Value_type(TIterator) {
        return (0);
    }
    template < class TInputIterator,
             class TDifference > inline
    void __cdecl advance(TInputIterator& Where, TDifference Offset) {
        Advance(Where, Offset, Iterator_cat(Where));
    }

    template < class TInputIterator,
             class TDifference > inline
    void __cdecl Advance(TInputIterator& Where, TDifference Offset, input_iterator_tag) {
        for (; 0 < Offset; --Offset)
            ++Where;
    }

    template < class TForwardIterator,
             class TDifference > inline
    void __cdecl Advance(TForwardIterator& Where, TDifference Offset, forward_iterator_tag) {
        for (; 0 < Offset; --Offset)
            ++Where;
    }

#pragma warning(push)
#pragma warning(disable: 6295)
    template < class TBidirectionalIterator,
             class TDifference > inline
    void __cdecl Advance(TBidirectionalIterator& Where, TDifference Offset, bidirectional_iterator_tag) {
        for (; 0 < Offset; --Offset)
            ++Where;

        for (; Offset < 0; ++Offset)
            --Where;
    }
#pragma warning(pop)

    template < class TRandomIterator,
             class TDifference > inline
    void __cdecl Advance(TRandomIterator& Where, TDifference Offset, random_access_iterator_tag) {
        Where += Offset;
    }

    template<class TIterator> inline
    typename iterator_traits<TIterator>::difference_type
    * __cdecl Dist_type(TIterator) {
        return (0);
    }
    template < class TInputIterator,
             class TDifference > inline
    void __cdecl Distance2(TInputIterator first, TInputIterator Last, TDifference& Offset,
                           input_iterator_tag) {
        for (; first != Last; ++first)
            ++Offset;
    }

    template < class TForwardIterator,
             class TDifference > inline
    void __cdecl Distance2(TForwardIterator first, TForwardIterator Last, TDifference& Offset,
                           forward_iterator_tag) {
        for (; first != Last; ++first)
            ++Offset;
    }

    template < class BidirectionIterator,
             class TDifference > inline
    void __cdecl Distance2(BidirectionIterator first, BidirectionIterator Last, TDifference& Offset,
                           bidirectional_iterator_tag) {
        for (; first != Last; ++first)
            ++Offset;
    }

    template < class TRandomIterator,
             class TDifference > inline
    void __cdecl Distance2(TRandomIterator first, TRandomIterator Last, TDifference& Offset,
                           random_access_iterator_tag) {
        if (first != Last) {
            Debug_pointer(first, L"..\\include\\xutility", 1871);
            Debug_pointer(Last, L"..\\include\\xutility", 1872);
        }

        Offset += Last - first;
    }

    template<class TInputIterator> inline
    typename iterator_traits<TInputIterator>::difference_type
    __cdecl distance(TInputIterator first, TInputIterator Last) {
        typename iterator_traits<TInputIterator>::difference_type Offset = 0;
        Distance2(first, Last, Offset, Iterator_cat(first));
        return (Offset);
    }
    template < class TInputIterator,
             class TDifference > inline
    void __cdecl Distance(TInputIterator first, TInputIterator Last, TDifference& Offset) {
        Distance2(first, Last, Offset, Iterator_cat(first));
    }
    template < class TRandomIterator,
             class TBase >
    class TRevRandomIterator
            : public TBase {
    public:
        typedef TRevRandomIterator<TRandomIterator, TBase> MyType;
        typedef typename iterator_traits<TRandomIterator>::iterator_category iterator_category;
        typedef typename iterator_traits<TRandomIterator>::value_type value_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type difference_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type distance_type;
        typedef typename iterator_traits<TRandomIterator>::pointer pointer;
        typedef typename iterator_traits<TRandomIterator>::reference reference;
        typedef TRandomIterator iterator_type;

        TRevRandomIterator() {
        }

        explicit  TRevRandomIterator(TRandomIterator right)
            : current(right) {
        }

        template < class TRandomIterator2,
                 class TBase2 >
        TRevRandomIterator(const TRevRandomIterator<TRandomIterator2, TBase2>& right)
            : current(right.base()) {
        }

        TRandomIterator  base() const {
            return (current);
        }

        reference  operator*() const {
            TRandomIterator temp = current;
            return (*--temp);
        }

        pointer  operator->() const {
            return (&** this);
        }

        MyType&  operator++() {
            --current;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            --current;
            return (temp);
        }

        MyType&  operator--() {
            ++current;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current;
            return (temp);
        }

        template < class TRandomIterator2,
                 class TBase2 >
        bool  Equal(const TRevRandomIterator<TRandomIterator2, TBase2>& right) const {
            return (current == right.base());
        }

        MyType&  operator+=(difference_type Offset) {
            current -= Offset;
            return (*this);
        }

        MyType  operator+(difference_type Offset) const {
            return (MyType(current - Offset));
        }

        MyType&  operator-=(difference_type Offset) {
            current += Offset;
            return (*this);
        }

        MyType  operator-(difference_type Offset) const {
            return (MyType(current + Offset));
        }

        reference  operator[](difference_type Offset) const {
            return (*(*this + Offset));
        }

        template < class TRandomIterator2,
                 class TBase2 >
        bool  Less(const TRevRandomIterator<TRandomIterator2, TBase2>& right) const {
            return (right.base() < current);
        }

        template < class TRandomIterator2,
                 class TBase2 >
        difference_type  Minus(const TRevRandomIterator<TRandomIterator2, TBase2>& right) const {
            return (right.base() - current);
        }

    protected:
        TRandomIterator current;
    };
    template < class TRandomIterator,
             class TBase,
             class TDifference > inline
    TRevRandomIterator<TRandomIterator, TBase>
    operator+(TDifference Offset,
              const TRevRandomIterator<TRandomIterator, TBase>& right) {
        return (right + Offset);
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    typename TBase1::difference_type operator-(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (left.Minus(right));
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator==(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (left.Equal(right));
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator!=(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (!(left == right));
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator<(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (left.Less(right));
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator>(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (right < left);
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator<=(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (!(right < left));
    }

    template < class TRandomIterator1,
             class TBase1,
             class TRandomIterator2,
             class TBase2 > inline
    bool operator>=(
        const TRevRandomIterator<TRandomIterator1, TBase1>& left,
        const TRevRandomIterator<TRandomIterator2, TBase2>& right) {
        return (!(left < right));
    }
    template<class TRandomIterator>
    class reverse_iterator
        : public TRevRandomIterator < TRandomIterator, iterator <
        typename iterator_traits<TRandomIterator>::iterator_category,
        typename iterator_traits<TRandomIterator>::value_type,
        typename iterator_traits<TRandomIterator>::difference_type,
        typename iterator_traits<TRandomIterator>::pointer,
            typename iterator_traits<TRandomIterator>::reference > > {
        typedef reverse_iterator<TRandomIterator> MyType;
        typedef TRevRandomIterator < TRandomIterator, iterator <
        typename iterator_traits<TRandomIterator>::iterator_category,
                 typename iterator_traits<TRandomIterator>::value_type,
                 typename iterator_traits<TRandomIterator>::difference_type,
                 typename iterator_traits<TRandomIterator>::pointer,
                 typename iterator_traits<TRandomIterator>::reference > > MyBaseType;

    public:
        typedef typename iterator_traits<TRandomIterator>::iterator_category iterator_category;
        typedef typename iterator_traits<TRandomIterator>::value_type value_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type difference_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type distance_type;
        typedef typename iterator_traits<TRandomIterator>::pointer pointer;
        typedef typename iterator_traits<TRandomIterator>::reference reference;
        typedef TRandomIterator iterator_type;

        reverse_iterator() {
        }

        explicit  reverse_iterator(TRandomIterator right)
            : MyBaseType(right) {
        }

        template<class Other>
        reverse_iterator(const reverse_iterator<Other>& right)
            : MyBaseType(right.base()) {
        }

        reverse_iterator(MyBaseType right)
            : MyBaseType(right) {
        }
        typedef typename ::std:: Checked_iterator_category<TRandomIterator>::Checked_cat Checked_iterator_category;
        typedef reverse_iterator<typename Checked_iterator_base_helper<TRandomIterator>::Checked_iterator_base_type> Checked_iterator_base_type;

        Checked_iterator_base_type Checked_iterator_base() const {
            typename Checked_iterator_base_type TBase(::std:: Checked_base(this->current));
            return TBase;
        }

        void Checked_iterator_assign_from_base(Checked_iterator_base_type TBase) {
            ::std:: Checked_assign_from_base(this->current, TBase.base());
        }
        MyType& operator++() {
            ++*((MyBaseType*)this);
            return (*this);
        }

        MyType operator++(int) {
            MyType temp = *this;
            ++*this;
            return (temp);
        }

        MyType& operator--() {
            --*((MyBaseType*)this);
            return (*this);
        }

        MyType operator--(int) {
            MyType temp = *this;
            --*this;
            return (temp);
        }

        MyType& operator+=(difference_type Offset) {
            *((MyBaseType*)this) += Offset;
            return (*this);
        }

        MyType operator+(difference_type Offset) const {
            MyType temp = *this;
            return (temp += Offset);
        }

        MyType& operator-=(difference_type Offset) {
            *((MyBaseType*)this) -= Offset;
            return (*this);
        }

        MyType operator-(difference_type Offset) const {
            MyType temp = *this;
            return (temp -= Offset);
        }
    };
    template < class TRandomIterator,
             class TDifference > inline
    reverse_iterator<TRandomIterator> __cdecl operator+(TDifference Offset,
            const reverse_iterator<TRandomIterator>& right) {
        return (right + Offset);
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    typename reverse_iterator<TRandomIterator1>::difference_type
    __cdecl operator-(const reverse_iterator<TRandomIterator1>& left,
                      const reverse_iterator<TRandomIterator2>& right) {
        return (left.Minus(right));
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator==(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (left.Equal(right));
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator!=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(left == right));
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator<(const reverse_iterator<TRandomIterator1>& left,
                           const reverse_iterator<TRandomIterator2>& right) {
        return (left.Less(right));
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator>(const reverse_iterator<TRandomIterator1>& left,
                           const reverse_iterator<TRandomIterator2>& right) {
        return (right < left);
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator<=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(right < left));
    }

    template < class TRandomIterator1,
             class TRandomIterator2 > inline
    bool __cdecl operator>=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(left < right));
    }
    template < class BidirectionIterator,
             class T1,
             class TReference = T1&,
             class TPointer = T1*,
             class TDifference = ptrdiff_t >
    class reverse_bidirectional_iterator
        : public iterator < bidirectional_iterator_tag, T1, TDifference,
              TPointer, TReference > {
    public:
        typedef reverse_bidirectional_iterator < BidirectionIterator, T1, TReference,
                TPointer, TDifference > MyType;
        typedef BidirectionIterator iterator_type;

        reverse_bidirectional_iterator() {
        }

        explicit  reverse_bidirectional_iterator(BidirectionIterator right)
            : current(right) {
        }

        BidirectionIterator  base() const {
            return (current);
        }

        TReference  operator*() const {
            BidirectionIterator temp = current;
            return (*--temp);
        }

        TPointer  operator->() const {
            TReference temp = **this;
            return (&temp);
        }

        MyType&  operator++() {
            --current;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            --current;
            return (temp);
        }

        MyType&  operator--() {
            ++current;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current;
            return (temp);
        }

        bool  operator==(const MyType& right) const {
            return (current == right.current);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }

    protected:
        BidirectionIterator current;
    };
    template < class BidirectionIterator,
             class BidirectionIterator2 = BidirectionIterator >
    class RevBidirectionalIterator
        : public iterator <
        typename iterator_traits<BidirectionIterator>::iterator_category,
        typename iterator_traits<BidirectionIterator>::value_type,
        typename iterator_traits<BidirectionIterator>::difference_type,
        typename iterator_traits<BidirectionIterator>::pointer,
            typename iterator_traits<BidirectionIterator>::reference > {
    public:
        typedef RevBidirectionalIterator<BidirectionIterator, BidirectionIterator2> MyType;
        typedef typename iterator_traits<BidirectionIterator>::difference_type TDifference;
        typedef typename iterator_traits<BidirectionIterator>::pointer TPointer;
        typedef typename iterator_traits<BidirectionIterator>::reference TReference;
        typedef BidirectionIterator iterator_type;

        RevBidirectionalIterator() {
        }

        explicit  RevBidirectionalIterator(BidirectionIterator right)
            : current(right) {
        }

        RevBidirectionalIterator(const RevBidirectionalIterator<BidirectionIterator2>& Other)
            : current(Other.base()) {
        }

        BidirectionIterator  base() const {
            return (current);
        }

        TReference  operator*() const {
            BidirectionIterator temp = current;
            return (*--temp);
        }

        TPointer  operator->() const {
            TReference temp = **this;
            return (&temp);
        }

        MyType&  operator++() {
            --current;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            --current;
            return (temp);
        }

        MyType&  operator--() {
            ++current;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current;
            return (temp);
        }

        bool  operator==(const MyType& right) const {
            return (current == right.current);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }

    protected:
        BidirectionIterator current;
    };
    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory>
    inline
    TOutputIterator __cdecl Copy_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     TInOutIteratorCategory, Nonscalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2470);

        for (; first != Last; ++Dest, ++first)
            *Dest = *first;

        return (Dest);
    }
    template<class TInputIterator, class TOutputIterator>
    inline
    TOutputIterator __cdecl Copy_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     random_access_iterator_tag, Nonscalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        TOutputIterator Result = Dest + (Last - first);
        Copy_opt(first, Last, ::std:: Checked_base(Dest),
                 forward_iterator_tag(), Nonscalar_ptr_iterator_tag(), Range_checked_iterator_tag());
        return Result;
    }
    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory>
    inline
    TOutputIterator __cdecl Copy_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     TInOutIteratorCategory, Scalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2497);

        if (first != Last)
            Debug_pointer(Dest, L"..\\include\\xutility", 2499);

        ptrdiff_t Offset = Last - first;
        TOutputIterator Result = Dest + Offset;

        if (Offset > 0)
            ::memmove_s((&*Dest), (Offset * sizeof(*first)), (&*first), (Offset * sizeof(*first)));

        return Result;
    }

    template <bool TCondition, class TResult>
    struct Enable_if;

    template <class TResult>
    struct Enable_if<true, TResult> {
        typedef TResult Result;
    };

    template <class TCheckedIteratorCategory>
    struct Is_checked_iterator_helper {
        enum { Result = false };
    };

    template <>
    struct Is_checked_iterator_helper<Range_checked_iterator_tag> {
        enum { Result = true };
    };

    template <class TIterator>
    struct Is_checked_iterator {
        enum { Result =
                   Is_checked_iterator_helper<typename Checked_iterator_category<TIterator>::Checked_cat>::Result
             };
    };

    template<class TInputIterator, class TOutputIterator>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl copy(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (Copy_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                         Iterator_random(first, Dest), Ptr_cat(first, Dest), Range_checked_iterator_tag()));
    }

    template<class TInputIterator, class TOutElement, size_t Size>
    inline
    TOutElement* __cdecl copy(TInputIterator first, TInputIterator Last, TOutElement(&Dest)[Size]) {
        return copy(::std:: Checked_base(first), ::std:: Checked_base(Last), ::stdext:: make_checked_arrayTIteratorator(Dest, Size)).base();
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl copy(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (Copy_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                         Iterator_random(first, Dest), Ptr_cat(first, Dest), Range_checked_iterator_tag()));
    }
    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory, class TMoveCategory>
    inline
    TOutputIterator __cdecl Move_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     TInOutIteratorCategory First_dest_cat, TMoveCategory, Range_checked_iterator_tag) {
        return Copy_opt(first, Last, Dest,
                        First_dest_cat, Ptr_cat(first, Dest), Range_checked_iterator_tag());
    }

    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory>
    inline
    TOutputIterator __cdecl Move_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     TInOutIteratorCategory, Swap_move_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2609);

        for (; first != Last; ++Dest, ++first)
            ::std:: Swap_adl(*Dest, *first);

        return (Dest);
    }
    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory>
    inline
    TOutputIterator __cdecl Move_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
                                     random_access_iterator_tag, Swap_move_tag Move_cat, Range_checked_iterator_tag) {
        TOutputIterator Result = Dest + (Last - first);
        Move_opt(first, Last, ::std:: Checked_base(Dest),
                 forward_iterator_tag(), Move_cat, Range_checked_iterator_tag());
        return Result;
    }
    template<class TInputIterator, class TOutputIterator>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl Move(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return Move_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                        Iterator_random(first, Dest), Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator, class TOutElement, size_t Size>
    inline
    TOutElement* __cdecl Move(TInputIterator first, TInputIterator Last, TOutElement(&Dest)[Size]) {
        return Move(::std:: Checked_base(first), ::std:: Checked_base(Last),
                    ::stdext:: make_checked_arrayTIteratorator(Dest, Size)).base();
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl Move(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return Move_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                        Iterator_random(first, Dest), Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }
    template<class BidirectionIterator1, class BidirectionIterator2, class TInOutIteratorCategory>
    inline
    BidirectionIterator2 __cdecl Copy_backward_opt(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest,
            TInOutIteratorCategory, Nonscalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2674);

        while (first != Last)
            *--Dest = *--Last;

        return (Dest);
    }
    template<class TInputIterator, class TOutputIterator>
    inline
    TOutputIterator __cdecl Copy_backward_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
            random_access_iterator_tag, Nonscalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        TOutputIterator Result = Dest - (Last - first);
        Copy_backward_opt(first, Last, ::std:: Checked_base(Dest),
                          forward_iterator_tag(), Nonscalar_ptr_iterator_tag(), Range_checked_iterator_tag());
        return Result;
    }
    template<class TInputIterator, class TOutputIterator, class TInOutIteratorCategory>
    inline
    TOutputIterator __cdecl Copy_backward_opt(TInputIterator first, TInputIterator Last, TOutputIterator Dest,
            TInOutIteratorCategory, Scalar_ptr_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2701);

        if (first != Last)
            Debug_pointer(Dest, L"..\\include\\xutility", 2703);

        ptrdiff_t Offset = Last - first;
        TOutputIterator Result = Dest - Offset;

        if (Offset > 0)
            ::memmove_s((&*Result), (Offset * sizeof(*first)), (&*first), (Offset * sizeof(*first)));

        return Result;
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl copy_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return Copy_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 Iterator_random(first, Dest), Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag());
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl copy_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return Copy_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 Iterator_random(first, Dest), Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag());
    }
    template<class BidirectionIterator1, class BidirectionIterator2, class TInOutIteratorCategory, class TMoveCategory>
    inline
    BidirectionIterator2 __cdecl Move_backward_opt(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest,
            TInOutIteratorCategory First_dest_cat, TMoveCategory, Range_checked_iterator_tag) {
        return Copy_backward_opt(first, Last, Dest,
                                 First_dest_cat, Ptr_cat(first, Dest), Range_checked_iterator_tag());
    }

    template<class BidirectionIterator1, class BidirectionIterator2, class TInOutIteratorCategory>
    inline
    BidirectionIterator2 __cdecl Move_backward_opt(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest,
            TInOutIteratorCategory, Swap_move_tag, Range_checked_iterator_tag) {
        Debug_range(first, Last, L"..\\include\\xutility", 2762);

        while (first != Last)
            ::std:: Swap_adl(*--Dest, *--Last);

        return (Dest);
    }
    template<class BidirectionIterator1, class BidirectionIterator2>
    inline
    BidirectionIterator2 __cdecl Move_backward_opt(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest,
            random_access_iterator_tag, Swap_move_tag Move_cat, Range_checked_iterator_tag) {
        BidirectionIterator2 Result = Dest - (Last - first);
        Move_backward_opt(first, Last, ::std:: Checked_base(Dest),
                          forward_iterator_tag(), Move_cat, Range_checked_iterator_tag());
        return Result;
    }
    template<class BidirectionIterator1, class BidirectionIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl _Move_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return Move_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 Iterator_random(first, Dest), Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template<class BidirectionIterator1, class BidirectionIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl _Move_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return Move_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 Iterator_random(first, Dest), Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }
    template<class TInputIterator1, class TInputIterator2, class TInputIteratorCategory>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl Mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2,
                     TInputIteratorCategory, Range_checked_iterator_tag) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 2822);

        if (first1 != Last1)
            Debug_pointer(first2, L"..\\include\\xutility", 2824);

        for (; first1 != Last1 && *first1 == *first2;)
            ++first1, ++first2;

        return (pair<TInputIterator1, TInputIterator2>(first1, first2));
    }
    template<class TInputIterator1, class TInputIterator2>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl Mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2,
                     random_access_iterator_tag, Range_checked_iterator_tag) {
        TInputIterator2 Last2 = first2 + (Last1 - first1);
        (Last2);
        pair<TInputIterator1, typename ::std:: Checked_iterator_base_helper<TInputIterator2>::Checked_iterator_base_type> Result =
            Mismatch(first1, Last1, ::std:: Checked_base(first2),
                     forward_iterator_tag(), Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first2, Result.second);
        return (pair<TInputIterator1, TInputIterator2>(Result.first, first2));
    }
    template<class TInputIterator1, class TInputIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInElement2, size_t Size>
    inline
    pair<TInputIterator1, TInElement2*>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size]) {
        pair<TInputIterator1, ::stdext:: checked_array_iterator<TInElement2*> > Result =
            mismatch(first1, Last1, ::stdext:: make_checked_arrayTIteratorator(first2, Size));
        return (pair<TInputIterator1, TInElement2*>(Result.first, Result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class pr, class TInputIteratorCategory>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl Mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred,
                     TInputIteratorCategory, Range_checked_iterator_tag) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 2908);

        if (first1 != Last1)
            Debug_pointer(first2, L"..\\include\\xutility", 2910);

        Debug_pointer(Pred, L"..\\include\\xutility", 2911);

        for (; first1 != Last1 && Pred(*first1, *first2);)
            ++first1, ++first2;

        return (pair<TInputIterator1, TInputIterator2>(first1, first2));
    }
    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl Mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred,
                     random_access_iterator_tag, Range_checked_iterator_tag) {
        TInputIterator2 Last2 = first2 + (Last1 - first1);
        (Last2);
        pair<TInputIterator1, typename ::std:: Checked_iterator_base_helper<TInputIterator2>::Checked_iterator_base_type> Result =
            Mismatch(first1, Last1, ::std:: Checked_base(first2), Pred,
                     forward_iterator_tag(), Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first2, Result.second);
        return (pair<TInputIterator1, TInputIterator2>(Result.first, first2));
    }
    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInElement2, class pr, size_t Size>
    inline
    pair<TInputIterator1, TInElement2*>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size], pr Pred) {
        pair<TInputIterator1, ::stdext:: checked_array_iterator<TInElement2*> > Result =
            mismatch(first1, Last1, ::stdext:: make_checked_arrayTIteratorator(first2, Size), Pred);
        return (pair<TInputIterator1, TInElement2*>(Result.first, Result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class TInputIteratorCategory>
    inline
    bool __cdecl Equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2,
                       TInputIteratorCategory, Range_checked_iterator_tag) {
        for (; first1 != Last1; ++first1, ++first2)
            if (!(*first1 == *first2))
                return (false);

        return (true);
    }

    inline bool __cdecl Equal(const char* first1,
                              const char* Last1, const char* first2,
                              random_access_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3003);

        if (first1 != Last1)
            Debug_pointer(first2, L"..\\include\\xutility", 3005);

        return (::memcmp(first1, first2, Last1 - first1) == 0);
    }

    inline bool __cdecl Equal(const signed char* first1,
                              const signed char* Last1, const signed char* first2,
                              random_access_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3016);

        if (first1 != Last1)
            Debug_pointer(first2, L"..\\include\\xutility", 3018);

        return (::memcmp(first1, first2, Last1 - first1) == 0);
    }

    inline bool __cdecl Equal(const unsigned char* first1,
                              const unsigned char* Last1, const unsigned char* first2,
                              random_access_iterator_tag, Range_checked_iterator_tag) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3029);

        if (first1 != Last1)
            Debug_pointer(first2, L"..\\include\\xutility", 3031);

        return (::memcmp(first1, first2, Last1 - first1) == 0);
    }
    template<class TInputIterator1, class TInputIterator2>
    inline
    bool __cdecl Equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2,
                       random_access_iterator_tag, Range_checked_iterator_tag) {
        TInputIterator2 Last2 = first2 + (Last1 - first1);
        (Last2);
        return Equal(first1, Last1, ::std:: Checked_base(first2),
                     forward_iterator_tag(), Range_checked_iterator_tag());
    }
    template<class TInputIterator1, class TInputIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        return Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, size_t Size>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size]) {
        return equal(first1, Last1,
                     ::stdext:: make_checked_arrayTIteratorator(first2, Size));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        return Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }
    template<class TInputIterator1, class TInputIterator2, class pr, class TInputIteratorCategory>
    inline
    bool __cdecl Equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred,
                       TInputIteratorCategory, Range_checked_iterator_tag) {
        for (; first1 != Last1; ++first1, ++first2)
            if (!Pred(*first1, *first2))
                return (false);

        return (true);
    }
    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    bool __cdecl Equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred,
                       random_access_iterator_tag, Range_checked_iterator_tag) {
        TInputIterator2 Last2 = first2 + (Last1 - first1);
        (Last2);
        return Equal(first1, Last1, ::std:: Checked_base(first2), Pred,
                     forward_iterator_tag(), Range_checked_iterator_tag());
    }
    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        return Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, class pr, size_t Size>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size], pr Pred) {
        return equal(first1, Last1,
                     ::stdext:: make_checked_arrayTIteratorator(first2, Size), Pred);
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        return Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                     Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }
    template<class TForwardIterator, class T1> inline
    void __cdecl Fill(TForwardIterator first, TForwardIterator Last, const T1& Value) {
        Debug_range(first, Last, L"..\\include\\xutility", 3157);

        for (; first != Last; ++first)
            *first = Value;
    }

    inline void __cdecl Fill([SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* first,
                             [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* Last, int Value) {
        Debug_range(first, Last, L"..\\include\\xutility", 3165);
        ::memset(first, Value, Last - first);
    }

    inline void __cdecl Fill([SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] signed char* first,
                             [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] signed char* Last, int Value) {
        Debug_range(first, Last, L"..\\include\\xutility", 3172);
        ::memset(first, Value, Last - first);
    }

    inline void __cdecl Fill(
        [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] unsigned char* first,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] unsigned char* Last, int Value) {
        Debug_range(first, Last, L"..\\include\\xutility", 3180);
        ::memset(first, Value, Last - first);
    }

    template<class TForwardIterator, class T1> inline
    void __cdecl fill(TForwardIterator first, TForwardIterator Last, const T1& Value) {
        Fill(::std:: Checked_base(first), ::std:: Checked_base(Last), Value);
    }
    template < class TOutputIterator,
             class TDifference,
             class T1 > inline
    void __cdecl Fill_n(TOutputIterator first, TDifference count, const T1& Value,
                        Range_checked_iterator_tag) {
        for (; 0 < count; --count, ++first)
            *first = Value;
    }

    inline void __cdecl Fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] char* first,
                               size_t count, int Value, Range_checked_iterator_tag) {
        if (0 < count)
            Debug_pointer(first, L"..\\include\\xutility", 3207);

        ::memset(first, Value, count);
    }

    inline void __cdecl Fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] signed char* first,
                               size_t count, int Value, Range_checked_iterator_tag) {
        if (0 < count)
            Debug_pointer(first, L"..\\include\\xutility", 3219);

        ::memset(first, Value, count);
    }

    inline void __cdecl Fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] unsigned char* first,
                               size_t count, int Value, Range_checked_iterator_tag) {
        if (0 < count)
            Debug_pointer(first, L"..\\include\\xutility", 3231);

        ::memset(first, Value, count);
    }

    template<class TOutputIterator, class TDifference, class T1, class TOutCategory>
    inline
    void __cdecl Fill_n(TOutputIterator first, TDifference count, const T1& Value,
                        TOutCategory, Range_checked_iterator_tag) {
        Fill_n(first, count, Value, Range_checked_iterator_tag());
    }
    template<class TOutputIterator, class TDifference, class T1>
    inline
    void __cdecl Fill_n(TOutputIterator first, TDifference count, const T1& Value,
                        random_access_iterator_tag, Range_checked_iterator_tag) {
        TOutputIterator Last = first + count;
        (Last);
        Fill_n(::std:: Checked_base(first), count, Value,
               Range_checked_iterator_tag());
    }
    template < class TOutputIterator,
             class TDifference,
             class T1 >
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, void >::Result __cdecl fill_n(TOutputIterator first, TDifference count, const T1& Value) {
        Fill_n(first, count, Value, Iterator_cat(first), ::std:: Range_checked_iterator_tag());
    }

    template < class TOutElement,
             class TDifference,
             class T1, size_t Size >
    inline
    void __cdecl fill_n(TOutElement(&first)[Size], TDifference count, const T1& Value) {
        fill_n(::stdext:: make_checked_arrayTIteratorator(first, Size), count, Value);
    }

    template < class TOutputIterator,
             class TDifference,
             class T1 >
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, void >::Result __cdecl fill_n(TOutputIterator first, TDifference count, const T1& Value) {
        Fill_n(first, count, Value, Iterator_cat(first), ::std:: Range_checked_iterator_tag());
    }
    template<class TInputIterator1, class TInputIterator2> inline
    bool __cdecl Lexicographical_compare(TInputIterator1 first1, TInputIterator1 Last1,
                                         TInputIterator2 first2, TInputIterator2 Last2) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3305);
        Debug_range(first2, Last2, L"..\\include\\xutility", 3306);

        for (; first1 != Last1 && first2 != Last2; ++first1, ++first2)
            if (Debug_lt(*first1, *first2, L"..\\include\\xutility", 3308))
                return (true);
            else if (*first2 < *first1)
                return (false);

        return (first1 == Last1 && first2 != Last2);
    }

    template<class TInputIterator1, class TInputIterator2> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 Last1,
                                         TInputIterator2 first2, TInputIterator2 Last2) {
        return Lexicographical_compare(::std:: Checked_base(first1), ::std:: Checked_base(Last1),
                                       ::std:: Checked_base(first2), ::std:: Checked_base(Last2));
    }

    inline bool __cdecl lexicographical_compare(
        const unsigned char* first1, const unsigned char* Last1,
        const unsigned char* first2, const unsigned char* Last2) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3327);
        Debug_range(first2, Last2, L"..\\include\\xutility", 3328);
        ptrdiff_t Num1 = Last1 - first1;
        ptrdiff_t Num2 = Last2 - first2;
        int Ans = ::memcmp(first1, first2, Num1 < Num2 ? Num1 : Num2);
        return (Ans < 0 || Ans == 0 && Num1 < Num2);
    }
    template < class TInputIterator1,
             class TInputIterator2,
             class pr > inline
    bool __cdecl Lexicographical_compare(TInputIterator1 first1, TInputIterator1 Last1,
                                         TInputIterator2 first2, TInputIterator2 Last2, pr Pred) {
        Debug_range(first1, Last1, L"..\\include\\xutility", 3356);
        Debug_range(first2, Last2, L"..\\include\\xutility", 3357);
        Debug_pointer(Pred, L"..\\include\\xutility", 3358);

        for (; first1 != Last1 && first2 != Last2; ++first1, ++first2)
            if (Debug_lt_pred(Pred, *first1, *first2, L"..\\include\\xutility", 3360))
                return (true);
            else if (Pred(*first2, *first1))
                return (false);

        return (first1 == Last1 && first2 != Last2);
    }

    template<class TInputIterator1, class TInputIterator2, class pr> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 Last1,
                                         TInputIterator2 first2, TInputIterator2 Last2, pr Pred) {
        return Lexicographical_compare(::std:: Checked_base(first1), ::std:: Checked_base(Last1),
                                       ::std:: Checked_base(first2), ::std:: Checked_base(Last2), Pred);
    }

    template<class T1> inline
    const T1& (__cdecl max)(const T1& left, const T1& right) {
        return (Debug_lt(left, right, L"..\\include\\xutility", 3384) ? right : left);
    }
    template < class T1,
             class pr > inline
    const T1& (__cdecl max)(const T1& left, const T1& right, pr Pred) {
        return (Debug_lt_pred(Pred, left, right, L"..\\include\\xutility", 3392) ? right : left);
    }
    template<class T1> inline
    const T1& (__cdecl min)(const T1& left, const T1& right) {
        return (Debug_lt(right, left, L"..\\include\\xutility", 3399) ? right : left);
    }
    template < class T1,
             class pr > inline
    const T1& (__cdecl min)(const T1& left, const T1& right, pr Pred) {
        return (Debug_lt_pred(Pred, right, left, L"..\\include\\xutility", 3407) ? right : left);
    }

}

namespace stdext {
    template <class TIterator>
    class checked_array_iterator
        : public ::std:: iterator <
        typename ::std:: iterator_traits<TIterator>::iterator_category,
        typename ::std:: iterator_traits<TIterator>::value_type,
        typename ::std:: iterator_traits<TIterator>::difference_type,
        typename ::std:: iterator_traits<TIterator>::pointer,
            typename ::std:: iterator_traits<TIterator>::reference > {
    public:
        typedef checked_array_iterator<TIterator> MyType;
        typedef typename ::std:: iterator_traits<TIterator>::difference_type difference_type;
        typedef typename ::std:: iterator_traits<TIterator>::pointer pointer;
        typedef typename ::std:: iterator_traits<TIterator>::reference reference;

        typedef ::std:: Range_checked_iterator_tag Checked_iterator_category;
        typedef TIterator Inner_type;

        typedef TIterator Checked_iterator_base_type;

        Checked_iterator_base_type Checked_iterator_base() const {
            return myContainer_ + Current_;
        }

        void Checked_iterator_assign_from_base(Checked_iterator_base_type TBase) {
            Current_ = TBase - myContainer_;
        }

        checked_array_iterator():
            Size(0), Current_(0) {
        }

        checked_array_iterator(TIterator Cont, size_t S, size_t Index = 0) {
            {
                if (!(Index <= S)) {
                    (void)((!!((("Index <= S", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3454, 0, L"(\"Index <= S\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3454, 0);
                } ;
            };
            myContainer_ = Cont;
            Size = S;
            Current_ = Index;
        }

        TIterator base() const {
            return myContainer_ + Current_;
        }

        size_t __Size() const {
            return Size;
        }

        bool operator==(const MyType& right) const {
            {
                if (!(myContainer_ == right.myContainer_)) {
                    (void)((!!((("myContainer_ == right.myContainer_", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3472, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3472, 0);
                } ;
            };
            return Current_ == right.Current_;
        }

        bool operator!=(const MyType& right) const {
            return !(*this == right);
        }

        bool operator<(const MyType& right) const {
            {
                if (!(myContainer_ == right.myContainer_)) {
                    (void)((!!((("myContainer_ == right.myContainer_", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3483, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3483, 0);
                } ;
            };
            return Current_ < right.Current_;
        }

        bool operator>(const MyType& right) const {
            return right < *this;
        }

        bool operator<=(const MyType& right) const {
            return !(right < *this);
        }

        bool operator>=(const MyType& right) const {
            return !(*this < right);
        }

        reference operator*() const {
            {
                if (!(Current_ < Size)) {
                    (void)((!!((("Current_ < Size", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3504, 0, L"(\"Current_ < Size\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3504, 0);
                } ;
            };
            return *(myContainer_ + Current_);
        }

        pointer operator->() const {
            return (&** this);
        }

        checked_array_iterator& operator++() {
            {
                if (!(Current_ < Size)) {
                    (void)((!!((("Current_ < Size", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3515, 0, L"(\"Current_ < Size\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3515, 0);
                } ;
            };
            ++Current_;
            return *this;
        }

        MyType operator++(int) {
            checked_array_iterator temp = *this;
            ++*this;
            return temp;
        }

        MyType& operator--() {
            {
                if (!(Current_ > 0)) {
                    (void)((!!((("Current_ > 0", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3529, 0, L"(\"Current_ > 0\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3529, 0);
                } ;
            };
            --Current_;
            return *this;
        }

        MyType operator--(int) {
            checked_array_iterator temp = *this;
            --*this;
            return temp;
        }

        MyType& operator+=(difference_type Offset) {
            {
                if (!(Current_ + Offset <= Size && Current_ + Offset >= 0)) {
                    (void)((!!((("Current_ + Offset <= Size && Current_ + Offset >= 0", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3545, 0, L"(\"Current_ + Offset <= Size && Current_ + Offset >= 0\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3545, 0);
                } ;
            };
            Current_ += Offset;
            return *this;
        }

        MyType operator+(difference_type Offset) const {
            checked_array_iterator temp = *this;
            return (temp += Offset);
        }

        MyType& operator-=(difference_type Offset) {
            return (*this += -Offset);
        }

        MyType operator-(difference_type Offset) const {
            checked_array_iterator temp = *this;
            return (temp -= Offset);
        }

        difference_type operator-(const checked_array_iterator& right) const {
            {
                if (!(myContainer_ == right.myContainer_)) {
                    (void)((!!((("myContainer_ == right.myContainer_", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3569, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3569, 0);
                } ;
            };
            return Current_ - right.Current_;
        }

        reference operator[](difference_type Offset) const {
            {
                if (!(Current_ + Offset < Size && Current_ + Offset >= 0)) {
                    (void)((!!((("Current_ + Offset < Size && Current_ + Offset >= 0", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xutility", 3575, 0, L"(\"Current_ + Offset < Size && Current_ + Offset >= 0\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xutility", 3575, 0);
                } ;
            };
            return *(myContainer_ + Current_ + Offset);
        }

    protected:
        void Xran() const {
            throw ::std:: out_of_range("invalid checked_array_iterator<T1> subscript");
        }

        void Xinvarg() const {
            throw ::std:: invalid_argument("invalid checked_array_iterator<T1> argument");
        }

        TIterator myContainer_;
        size_t Current_;
        size_t Size;
    };

    template <class TIterator>
    checked_array_iterator<TIterator> make_checked_arrayTIteratorator(TIterator Ptr, size_t Size) {
        return checked_array_iterator<TIterator>(Ptr, Size);
    }

    template < class TInputIterator,
             class TOutputIterator > inline
    TOutputIterator __cdecl unchecked_copy(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (::std:: Copy_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag()));
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl checked_copy(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (::std:: Copy_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag()));
    }

    template<class TInputIterator, class TOutElement, size_t Size>
    inline
    TOutElement* __cdecl checked_copy(TInputIterator first, TInputIterator Last, TOutElement(&Dest)[Size]) {
        return checked_copy(::std:: Checked_base(first), ::std:: Checked_base(Last), ::stdext:: make_checked_arrayTIteratorator(Dest, Size)).base();
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl checked_copy(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (::std:: Copy_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag()));
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    BidirectionIterator2 __cdecl unchecked_copy_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return (::std:: Copy_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                          ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag()));
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl checked_copy_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return ::std:: Copy_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                         ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag());
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl checked_copy_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return ::std:: Copy_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                         ::std:: Iterator_random(first, Dest), ::std:: Ptr_cat(first, Dest), ::std:: Range_checked_iterator_tag());
    }

    template < class TInputIterator,
             class TOutputIterator > inline
    TOutputIterator __cdecl _Unchecked_move(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return (::std:: Move_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                 ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag()));
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl Checked_move(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return ::std:: Move_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator, class TOutElement, size_t Size>
    inline
    TOutElement* __cdecl Checked_move(TInputIterator first, TInputIterator Last, TOutElement(&Dest)[Size]) {
        return Checked_move(::std:: Checked_base(first), ::std:: Checked_base(Last),
                            ::stdext:: make_checked_arrayTIteratorator(Dest, Size)).base();
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, TOutputIterator >::Result __cdecl Checked_move(TInputIterator first, TInputIterator Last, TOutputIterator Dest) {
        return ::std:: Move_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template < class BidirectionIterator1,
             class BidirectionIterator2 > inline
    BidirectionIterator2 __cdecl Unchecked_move_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return (::std:: Move_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                          ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag()));
    }

    template<class BidirectionIterator1, class BidirectionIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl _Checked_move_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return ::std:: Move_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                         ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template<class BidirectionIterator1, class BidirectionIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< BidirectionIterator2 >::Result, BidirectionIterator2 >::Result __cdecl _Checked_move_backward(BidirectionIterator1 first, BidirectionIterator1 Last, BidirectionIterator2 Dest) {
        return ::std:: Move_backward_opt(::std:: Checked_base(first), ::std:: Checked_base(Last), Dest,
                                         ::std:: Iterator_random(first, Dest), ::std:: Move_cat(Dest), ::std:: Range_checked_iterator_tag());
    }

    template < class TOutputIterator,
             class TDifference,
             class T1 > inline
    void __cdecl unchecked_fill_n(TOutputIterator first, TDifference count, const T1& Value) {
        ::std:: Fill_n(first, count, Value, ::std:: Iterator_cat(first), ::std:: Range_checked_iterator_tag());
    }

    template < class TOutputIterator,
             class TDifference,
             class T1 >
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TOutputIterator >::Result, void >::Result __cdecl checked_fill_n(TOutputIterator first, TDifference count, const T1& Value) {
        Fill_n(first, count, Value, ::std:: Iterator_cat(first), ::std:: Range_checked_iterator_tag());
    }

    template < class TOutElement,
             class TDifference,
             class T1, size_t Size >
    inline
    void __cdecl checked_fill_n(TOutElement(&first)[Size], TDifference count, const T1& Value) {
        checked_fill_n(::stdext:: make_checked_arrayTIteratorator(first, Size), count, Value);
    }

    template < class TOutputIterator,
             class TDifference,
             class T1 >
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TOutputIterator >::Result, void >::Result __cdecl checked_fill_n(TOutputIterator first, TDifference count, const T1& Value) {
        Fill_n(first, count, Value, ::std:: Iterator_cat(first), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    ::std:: pair<TInputIterator1, TInputIterator2>
    __cdecl unchecked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInElement2, size_t Size>
    inline
    ::std:: pair<TInputIterator1, TInElement2*>
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size]) {
        ::std:: pair<TInputIterator1, ::stdext:: checked_array_iterator<TInElement2*> > Result =
            checked_mismatch(first1, Last1, ::stdext:: make_checked_arrayTIteratorator(first2, Size));
        return (::std:: pair<TInputIterator1, TInElement2*>(Result.first, Result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    ::std:: pair<TInputIterator1, TInputIterator2>
    __cdecl unchecked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInElement2, class pr, size_t Size>
    inline
    ::std:: pair<TInputIterator1, TInElement2*>
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size], pr Pred) {
        ::std:: pair<TInputIterator1, ::stdext:: checked_array_iterator<TInElement2*> > Result =
            checked_mismatch(first1, Last1, ::stdext:: make_checked_arrayTIteratorator(first2, Size), Pred);
        return (::std:: pair<TInputIterator1, TInElement2*>(Result.first, Result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, ::std:: pair< TInputIterator1, TInputIterator2 > >::Result
    __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        ::std:: pair<typename ::std:: Checked_iterator_base_helper<TInputIterator1>::Checked_iterator_base_type, TInputIterator2> Result =
            ::std:: Mismatch(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
        ::std:: Checked_assign_from_base(first1, Result.first);
        return (::std:: pair<TInputIterator1, TInputIterator2>(first1, Result.second));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    bool __cdecl unchecked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, size_t Size>
    inline
    bool __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size]) {
        return checked_equal(first1, Last1,
                             ::stdext:: make_checked_arrayTIteratorator(first2, Size));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    bool __cdecl unchecked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    typename ::std:: Enable_if< ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, class pr, size_t Size>
    inline
    bool __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInElement2(&first2)[Size], pr Pred) {
        return checked_equal(first1, Last1,
                             ::stdext:: make_checked_arrayTIteratorator(first2, Size), Pred);
    }

    template<class TInputIterator1, class TInputIterator2, class pr>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std:: Enable_if < ! ::std:: Is_checked_iterator< TInputIterator2 >::Result, bool >::Result __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 Last1, TInputIterator2 first2, pr Pred) {
        return ::std:: Equal(::std:: Checked_base(first1), ::std:: Checked_base(Last1), first2, Pred,
                             ::std:: Iterator_random(first1, first2), ::std:: Range_checked_iterator_tag());
    }

}

extern "C"

__declspec(dllimport) void __cdecl _invalid_parameter([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, unsigned int, uintptr_t);
#pragma warning(pop)
#pragma pack(pop)
#pragma pack(push,8)
#pragma warning(push,3)
#pragma warning(disable: 4100)

namespace std {

    template<class T1> inline
    T1*  _Allocate(size_t count, T1*) {
        if (count <= 0)
            count = 0;
        else if (((size_t)(-1) / count) < sizeof(T1))
            throw std::bad_alloc(0);

        return ((T1*)::operator new(count * sizeof(T1)));
    }
    template < class T1,
             class T2 > inline
    void Construct_(T1*  Ptr, const T2& Value) {
        void*  vPtr = Ptr;
        ::new(vPtr) T1(Value);
    }
    template<class T1> inline
    void _Destroy(T1*  Ptr) {
        (Ptr)->~T1();
    }

    template<> inline
    void _Destroy(char*) {
    }

    template<> inline
    void _Destroy(wchar_t*) {
    }

    template<class T1>
    struct Allocator_base {
        typedef T1 value_type;
    };
    template<class T1>
    struct Allocator_base<const T1> {
        typedef T1 value_type;
    };
    template<class T1>
    class allocator
            : public Allocator_base<T1> {
    public:
        typedef Allocator_base<T1> MyBaseType;
        typedef typename MyBaseType::value_type value_type;
        typedef value_type*  pointer;
        typedef value_type& reference;
        typedef const value_type*  const_pointer;
        typedef const value_type& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        template<class Other>
        struct rebind {
            typedef allocator<Other> other;
        };

        pointer address(reference Value) const {
            return (&Value);
        }

        const_pointer address(const_reference Value) const {
            return (&Value);
        }

        allocator() throw() {
        }

        allocator(const allocator<T1>&) throw() {
        }

        template<class Other>
        allocator(const allocator<Other>&) throw() {
        }

        template<class Other>
        allocator<T1>& operator=(const allocator<Other>&) {
            return (*this);
        }

        void deallocate(pointer Ptr, size_type) {
            ::operator delete(Ptr);
        }

        pointer allocate(size_type count) {
            return (_Allocate(count, (pointer)0));
        }

        pointer allocate(size_type count, const void*) {
            return (allocate(count));
        }

        void construct(pointer Ptr, const T1& Value) {
            Construct_(Ptr, Value);
        }

        void destroy(pointer Ptr) {
            _Destroy(Ptr);
        }

        size_t max_size() const throw() {
            size_t count = (size_t)(-1) / sizeof(T1);
            return (0 < count ? count : 1);
        }
    };
    template < class T1,
             class Other > inline
    bool operator==(const allocator<T1>&, const allocator<Other>&) throw() {
        return (true);
    }

    template < class T1,
             class Other > inline
    bool operator!=(const allocator<T1>&, const allocator<Other>&) throw() {
        return (false);
    }
    template<> class __declspec(dllimport) allocator<void> {
    public:
        typedef void T1;
        typedef T1*  pointer;
        typedef const T1*  const_pointer;
        typedef T1 value_type;

        template<class Other>
        struct rebind {
            typedef allocator<Other> other;
        };

        allocator() throw() {
        }

        allocator(const allocator<T1>&) throw() {
        }

        template<class Other>
        allocator(const allocator<Other>&) throw() {
        }

        template<class Other>
        allocator<T1>& operator=(const allocator<Other>&) {
            return (*this);
        }
    };
    template<class TAllocator> inline
    void _Destroy_range(typename TAllocator::pointer first,
                        typename TAllocator::pointer Last, TAllocator& alt) {
        _Destroy_range(first, Last, alt, Ptr_cat(first, Last));
    }

    template<class TAllocator> inline
    void _Destroy_range(typename TAllocator::pointer first,
                        typename TAllocator::pointer Last, TAllocator& alt,
                        Nonscalar_ptr_iterator_tag) {
        for (; first != Last; ++first)
            alt.destroy(first);
    }

    template<class TAllocator> inline
    void _Destroy_range(typename TAllocator::pointer first,
                        typename TAllocator::pointer Last, TAllocator& alt,
                        Scalar_ptr_iterator_tag) {
    }
}
#pragma warning(default: 4100)
#pragma warning(pop)
#pragma pack(pop)

#pragma pack(push,8)
#pragma warning(push,3)
#pragma warning(disable:4412)

namespace std {

#pragma warning(disable:4251)

    template < class TElement,
             class TTraits = char_traits<TElement>,
             class TAllocator = allocator<TElement> >
    class basic_string;
    template < class TElement,
             class TTraits,
             class TAllocator >
    class String_const_iterator
        : public RandomIterator_base < TElement, typename TAllocator::difference_type,
              typename TAllocator::const_pointer, typename TAllocator::const_reference, Iterator_base_secure > {
    public:
        typedef String_const_iterator<TElement, TTraits, TAllocator> MyType;
        typedef basic_string<TElement, TTraits, TAllocator> MyString;

        typedef random_access_iterator_tag iterator_category;
        typedef TElement value_type;
        typedef typename TAllocator::difference_type difference_type;
        typedef typename TAllocator::const_pointer pointer;
        typedef typename TAllocator::const_reference reference;
        typedef TElement* Inner_type;
        typedef Range_checked_iterator_tag Checked_iterator_category;
        String_const_iterator() {
            myPtr_ = 0;
        }
        String_const_iterator(pointer Ptr, const Container_base_secure* pString) {
            {
                if (!(pString == 0 || Ptr != 0 && ((MyString*)pString)->myPtr_() <= Ptr && Ptr <= (((MyString*)pString)->myPtr_() + ((MyString*)pString)->mySize_))) {
                    (void)((!!((("pString == NULL || Ptr != NULL && ((MyString *)pString)->myPtr_() <= Ptr && Ptr <= (((MyString *)pString)->myPtr_() + ((MyString *)pString)->mySize_)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 76, 0, L"(\"pString == NULL || Ptr != NULL && ((MyString *)pString)->myPtr_() <= Ptr && Ptr <= (((MyString *)pString)->myPtr_() + ((MyString *)pString)->mySize_)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 76, 0);
                } ;
            };
            this->Adopt(pString);
            myPtr_ = Ptr;
        }

        reference  operator*() const {
            if (this->myContainer_ == 0 || myPtr_ == 0
                    || myPtr_ < ((MyString*)this->myContainer_)->myPtr_()
                    || ((MyString*)this->myContainer_)->myPtr_()
                    + ((MyString*)this->myContainer_)->mySize_ <= myPtr_) {
                Debug_message(L"string iterator not dereferencable", L"..\\include\\xstring", 112);
                {
                    (void)((!!((("Standard C++ Libraries Out of Range", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 113, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 113, 0);
                };
            }

            ;

            return (*myPtr_);
        }

        pointer  operator->() const {
            return (&** this);
        }

        MyType&  operator++() {
            if (this->myContainer_ != ((const Container_base_secure*) - 4)) {
                {
                    if (!(this->Has_container())) {
                        (void)((!!((("this->Has_container()", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 136, 0, L"(\"this->Has_container()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 136, 0);
                    } ;
                };
                {
                    if (!(myPtr_ < (((MyString*)this->myContainer_)->myPtr_() + ((MyString*)this->myContainer_)->mySize_))) {
                        (void)((!!((("myPtr_ < (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 137, 0, L"(\"myPtr_ < (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_)\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 137, 0);
                    } ;
                };
            }

            ++myPtr_;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            ++*this;
            return (temp);
        }

        MyType&  operator--() {
            if (this->myContainer_ != ((const Container_base_secure*) - 4)) {
                {
                    if (!(this->Has_container())) {
                        (void)((!!((("this->Has_container()", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 154, 0, L"(\"this->Has_container()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 154, 0);
                    } ;
                };
                {
                    if (!(myPtr_ > ((MyString*)this->myContainer_)->myPtr_())) {
                        (void)((!!((("myPtr_ > ((MyString *)this->myContainer_)->myPtr_()", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 155, 0, L"(\"myPtr_ > ((MyString *)this->myContainer_)->myPtr_()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 155, 0);
                    } ;
                };
            }

            --myPtr_;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            --*this;
            return (temp);
        }

        MyType&  operator+=(difference_type Offset) {
            if (this->myContainer_ != ((const Container_base_secure*) - 4)) {
                {
                    if (!(this->Has_container())) {
                        (void)((!!((("this->Has_container()", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 172, 0, L"(\"this->Has_container()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 172, 0);
                    } ;
                };
                {
                    if (!(myPtr_ + Offset <= (((MyString*)this->myContainer_)->myPtr_() + ((MyString*)this->myContainer_)->mySize_) && myPtr_ + Offset >= ((MyString*)this->myContainer_)->myPtr_())) {
                        (void)((!!((("myPtr_ + Offset <= (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_) && myPtr_ + Offset >= ((MyString *)this->myContainer_)->myPtr_()", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 175, 0, L"(\"myPtr_ + Offset <= (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_) && myPtr_ + Offset >= ((MyString *)this->myContainer_)->myPtr_()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 175, 0);
                    } ;
                };
            }

            myPtr_ += Offset;
            return (*this);
        }

        MyType  operator+(difference_type Offset) const {
            MyType temp = *this;
            return (temp += Offset);
        }

        MyType&  operator-=(difference_type Offset) {
            return (*this += -Offset);
        }

        MyType  operator-(difference_type Offset) const {
            MyType temp = *this;
            return (temp -= Offset);
        }

        difference_type  operator-(const MyType& right) const {
            _Compat(right);
            return (myPtr_ - right.myPtr_);
        }

        reference  operator[](difference_type Offset) const {
            return (*(*this + Offset));
        }

        bool  operator==(const MyType& right) const {
            _Compat(right);
            return (myPtr_ == right.myPtr_);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }

        bool  operator<(const MyType& right) const {
            _Compat(right);
            return (myPtr_ < right.myPtr_);
        }

        bool  operator>(const MyType& right) const {
            return (right < *this);
        }

        bool  operator<=(const MyType& right) const {
            return (!(right < *this));
        }

        bool  operator>=(const MyType& right) const {
            return (!(*this < right));
        }
        void  _Compat(const MyType& right) const {
            if (this->myContainer_ == 0 || this->myContainer_ != right.myContainer_) {
                Debug_message(L"string iterators incompatible", L"..\\include\\xstring", 273);
                {
                    (void)((!!((("Standard C++ Libraries Invalid Argument", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 274, 0, L"(\"Standard C++ Libraries Invalid Argument\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 274, 0);
                };
            }
        }
        static void __cdecl Xlen() {
            MyString::Xlen();
        }

        static void __cdecl Xran() {
            MyString::Xran();
        }

        static void __cdecl Xinvarg() {
            MyString::Xinvarg();
        }

        pointer myPtr_;
    };

    template < class TElement,
             class TTraits,
             class TAllocator > inline
    String_const_iterator<TElement, TTraits, TAllocator> __cdecl operator+(
        typename String_const_iterator<TElement, TTraits, TAllocator>
        ::difference_type Offset,
        String_const_iterator<TElement, TTraits, TAllocator> Next) {
        return (Next += Offset);
    }
    template < class TElement,
             class TTraits,
             class TAllocator >
    class String_iterator
            : public String_const_iterator<TElement, TTraits, TAllocator> {
    public:
        typedef String_iterator<TElement, TTraits, TAllocator> MyType;
        typedef String_const_iterator<TElement, TTraits, TAllocator> MyBaseType;

        typedef random_access_iterator_tag iterator_category;
        typedef TElement value_type;
        typedef typename TAllocator::difference_type difference_type;
        typedef typename TAllocator::pointer pointer;
        typedef typename TAllocator::reference reference;

        String_iterator() {
        }
        String_iterator(pointer Ptr, const Container_base_secure* pString)
            : MyBaseType(Ptr, pString) {
        }

        reference  operator*() const {
            return ((reference)** (MyBaseType*)this);
        }

        pointer  operator->() const {
            return (&** this);
        }

        MyType&  operator++() {
            ++(*(MyBaseType*)this);
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            ++*this;
            return (temp);
        }

        MyType&  operator--() {
            --(*(MyBaseType*)this);
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            --*this;
            return (temp);
        }

        MyType&  operator+=(difference_type Offset) {
            (*(MyBaseType*)this) += Offset;
            return (*this);
        }

        MyType  operator+(difference_type Offset) const {
            MyType temp = *this;
            return (temp += Offset);
        }

        MyType&  operator-=(difference_type Offset) {
            return (*this += -Offset);
        }

        MyType  operator-(difference_type Offset) const {
            MyType temp = *this;
            return (temp -= Offset);
        }

        difference_type  operator-(const MyBaseType& right) const {
            return ((MyBaseType) * this - right);
        }

        reference  operator[](difference_type Offset) const {
            return (*(*this + Offset));
        }
    };

    template < class TElement,
             class TTraits,
             class TAllocator > inline
    String_iterator<TElement, TTraits, TAllocator>  operator+(
        typename String_iterator<TElement, TTraits, TAllocator>
        ::difference_type Offset,
        String_iterator<TElement, TTraits, TAllocator> Next) {
        return (Next += Offset);
    }
    class __declspec(dllimport) String_base
            : public Container_base_secure {
    public:
        static void __cdecl Xlen();

        static void __cdecl Xran();

        static void __cdecl Xinvarg();
    };
    template < class T1,
             class TAllocator >
    class String_val
            : public String_base {
    protected:
        typedef typename TAllocator::template
        rebind<T1>::other Alty;

        String_val(Alty alt = Alty())
            : altVal_(alt) {
        }

    public:
        String_val(const String_val& right)
            : altVal_(right.altVal_) {
            if (right.myFirstIterator_ == ((Iterator_base*) - 3)) {
                this->myFirstIterator_ = ((Iterator_base*) - 3);
            }
        }

    protected:
        Alty altVal_;
    };
    template < class TElement,
             class TTraits,
             class TAllocator >
    class basic_string
            : public String_val<TElement, TAllocator> {
    public:
        typedef basic_string<TElement, TTraits, TAllocator> MyType;
        typedef String_val<TElement, TAllocator> MyBaseType;
        typedef typename MyBaseType::Alty TAllocator;
        typedef typename TAllocator::size_type size_type;
        typedef typename TAllocator::difference_type Difference;
        typedef Difference difference_type;
        typedef typename TAllocator::pointer Pointer;
        typedef typename TAllocator::const_pointer ConstPointer;
        typedef Pointer pointer;
        typedef ConstPointer const_pointer;
        typedef typename TAllocator::reference Reference;
        typedef Reference reference;
        typedef typename TAllocator::const_reference const_reference;
        typedef typename TAllocator::value_type value_type;

        typedef String_iterator<TElement, TTraits, TAllocator> iterator;
        typedef String_const_iterator<TElement, TTraits, TAllocator> const_iterator;
        friend class String_const_iterator<TElement, TTraits, TAllocator>;

        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        struct Has_debug_it {
            bool Value;
            explicit Has_debug_it(bool value): Value(value) { }
        };
        basic_string(Has_debug_it hasDebugIt = Has_debug_it(true))
            : MyBaseType() {
            if (!hasDebugIt.Value) {
                this->myFirstIterator_ = ((Iterator_base*) - 3);
            }

            Tidy_();
        }

        explicit  basic_string(const TAllocator& alt)
            : MyBaseType(alt) {
            Tidy_();
        }

        basic_string(const MyType& right, size_type rightOffset,
                     size_type count = npos)
            : MyBaseType() {
            if (right.myFirstIterator_ == ((Iterator_base*) - 3)) {
                this->myFirstIterator_ = ((Iterator_base*) - 3);
            }

            Tidy_();
            assign(right, rightOffset, count);
        }

        basic_string(const MyType& right, size_type rightOffset, size_type count,
                     const TAllocator& alt)
            : MyBaseType(alt) {
            if (right.myFirstIterator_ == ((Iterator_base*) - 3)) {
                this->myFirstIterator_ = ((Iterator_base*) - 3);
            }

            Tidy_();
            assign(right, rightOffset, count);
        }

        basic_string(const TElement* Ptr, size_type count)
            : MyBaseType() {
            Tidy_();
            assign(Ptr, count);
        }

        basic_string(const TElement* Ptr, size_type count, const TAllocator& alt)
            : MyBaseType(alt) {
            Tidy_();
            assign(Ptr, count);
        }

        basic_string(const TElement* Ptr)
            : MyBaseType() {
            Tidy_();
            assign(Ptr);
        }

        basic_string(const TElement* Ptr, const TAllocator& alt)
            : MyBaseType(alt) {
            Tidy_();
            assign(Ptr);
        }

        basic_string(size_type count, TElement chr)
            : MyBaseType() {
            Tidy_();
            assign(count, chr);
        }

        basic_string(size_type count, TElement chr, const TAllocator& alt)
            : MyBaseType(alt) {
            Tidy_();
            assign(count, chr);
        }

        template<class TIterator>
        basic_string(TIterator first, TIterator Last)
            : MyBaseType() {
            Tidy_();
            Construct_(first, Last, Iterator_cat(first));
        }

        template<class TIterator>
        basic_string(TIterator first, TIterator Last, const TAllocator& alt)
            : MyBaseType(alt) {
            Tidy_();
            Construct_(first, Last, Iterator_cat(first));
        }

        basic_string(const_pointer first, const_pointer Last)
            : MyBaseType() {
            Debug_range(first, Last, L"..\\include\\xstring", 697);
            Tidy_();

            if (first != Last)
                assign(&*first, Last - first);
        }

        basic_string(const_iterator first, const_iterator Last)
            : MyBaseType() {
            Debug_range(first, Last, L"..\\include\\xstring", 708);
            Tidy_();

            if (first != Last)
                assign(&*first, Last - first);
        }

        basic_string(const MyType& right)
            : MyBaseType(right) {
            Tidy_();
            assign(right, 0, npos);
        }

        template<class TIterator>
        void  Construct_(TIterator count,
                         TIterator chr, Int_iterator_tag) {
            assign((size_type)count, (TElement)chr);
        }

        template<class TIterator>
        void  Construct_(TIterator first,
                         TIterator Last, input_iterator_tag) {
            try {
                for (; first != Last; ++first)
                    append((size_type)1, (TElement)*first);
            } catch (...) {
                Tidy_(true);
                throw;
            }
        }

        template<class TIterator>
        void  Construct_(TIterator first,
                         TIterator Last, forward_iterator_tag) {
            if (this->myFirstIterator_ != ((Iterator_base*) - 3)) {
                Debug_range(first, Last, L"..\\include\\xstring", 888);
            }

            size_type count = 0;
            Distance(first, Last, count);
            reserve(count);

            try {
                for (; first != Last; ++first)
                    append((size_type)1, (TElement)*first);
            } catch (...) {
                Tidy_(true);
                throw;
            }
        }

        ~basic_string() {
            Tidy_(true);
        }

        typedef TTraits traitsTpe;
        typedef TAllocator allocatorTpe;

        static const size_type npos;

        MyType&  operator=(const MyType& right) {
            return (assign(right));
        }

        MyType&  operator=(const TElement* Ptr) {
            return (assign(Ptr));
        }

        MyType&  operator=(TElement chr) {
            return (assign(1, chr));
        }

        MyType&  operator+=(const MyType& right) {
            return (append(right));
        }

        MyType&  operator+=(const TElement* Ptr) {
            return (append(Ptr));
        }

        MyType&  operator+=(TElement chr) {
            return (append((size_type)1, chr));
        }

        MyType&  append(const MyType& right) {
            return (append(right, 0, npos));
        }

        MyType&  append(const MyType& right,
                        size_type rightOffset, size_type count) {
            if (right.size() < rightOffset)
                String_base::Xran();

            size_type num = right.size() - rightOffset;

            if (num < count)
                count = num;

            if (npos - mySize_ <= count || mySize_ + count < mySize_)
                String_base::Xlen();

            if (0 < count && Grow(num = mySize_ + count)) {
                Traits_helper::copy_s<TTraits>(myPtr_() + mySize_, myRes_ - mySize_,
                                               right.myPtr_() + rightOffset, count);
                Eos(num);
            }

            return (*this);
        }

        MyType&  append(const TElement* Ptr, size_type count) {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 974);

            if (Inside(Ptr))
                return (append(*this, Ptr - myPtr_(), count));

            if (npos - mySize_ <= count || mySize_ + count < mySize_)
                String_base::Xlen();

            size_type num;

            if (0 < count && Grow(num = mySize_ + count)) {
                Traits_helper::copy_s<TTraits>(myPtr_() + mySize_, myRes_ - mySize_, Ptr, count);
                Eos(num);
            }

            return (*this);
        }

        MyType&  append(const TElement* Ptr) {
            Debug_pointer(Ptr, L"..\\include\\xstring", 993);
            return (append(Ptr, TTraits::length(Ptr)));
        }

        MyType&  append(size_type count, TElement chr) {
            if (npos - mySize_ <= count)
                String_base::Xlen();

            size_type num;

            if (0 < count && Grow(num = mySize_ + count)) {
                ChAssign(mySize_, count, chr);
                Eos(num);
            }

            return (*this);
        }

        template<class TIterator>
        MyType&  append(TIterator first, TIterator Last) {
            return (Append(first, Last, Iterator_cat(first)));
        }

        template<class TIterator>
        MyType&  Append(TIterator count, TIterator chr, Int_iterator_tag) {
            return (append((size_type)count, (TElement)chr));
        }

        template<class TIterator>
        MyType&  Append(TIterator first, TIterator Last, input_iterator_tag) {
            return (replace(end(), end(), first, Last));
        }

        MyType&  append(const_pointer first, const_pointer Last) {
            return (replace(end(), end(), first, Last));
        }

        MyType&  append(const_iterator first, const_iterator Last) {
            return (replace(end(), end(), first, Last));
        }

        MyType&  assign(const MyType& right) {
            return (assign(right, 0, npos));
        }

        MyType&  assign(const MyType& right,
                        size_type rightOffset, size_type count) {
            if (right.size() < rightOffset)
                String_base::Xran();

            size_type num = right.size() - rightOffset;

            if (count < num)
                num = count;

            if (this == &right)
                erase((size_type)(rightOffset + num)), erase(0, rightOffset);
            else if (Grow(num)) {
                Traits_helper::copy_s<TTraits>(myPtr_(), myRes_, right.myPtr_() + rightOffset, num);
                Eos(num);
            }

            return (*this);
        }

        MyType&  assign(const TElement* Ptr, size_type count) {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1068);

            if (Inside(Ptr))
                return (assign(*this, Ptr - myPtr_(), count));

            if (Grow(count)) {
                Traits_helper::copy_s<TTraits>(myPtr_(), myRes_, Ptr, count);
                Eos(count);
            }

            return (*this);
        }

        MyType&  assign(const TElement* Ptr) {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1084);
            return (assign(Ptr, TTraits::length(Ptr)));
        }

        MyType&  assign(size_type count, TElement chr) {
            if (count == npos)
                String_base::Xlen();

            if (Grow(count)) {
                ChAssign(0, count, chr);
                Eos(count);
            }

            return (*this);
        }

        template<class TIterator>
        MyType&  assign(TIterator first, TIterator Last) {
            return (Assign(first, Last, Iterator_cat(first)));
        }

        template<class TIterator>
        MyType&  Assign(TIterator count, TIterator chr, Int_iterator_tag) {
            return (assign((size_type)count, (TElement)chr));
        }

        template<class TIterator>
        MyType&  Assign(TIterator first, TIterator Last, input_iterator_tag) {
            return (replace(begin(), end(), first, Last));
        }

        MyType&  assign(const_pointer first, const_pointer Last) {
            return (replace(begin(), end(), first, Last));
        }

        MyType&  assign(const_iterator first, const_iterator Last) {
            return (replace(begin(), end(), first, Last));
        }

        MyType&  insert(size_type Offset, const MyType& right) {
            return (insert(Offset, right, 0, npos));
        }

        MyType&  insert(size_type Offset,
                        const MyType& right, size_type rightOffset, size_type count) {
            if (mySize_ < Offset || right.size() < rightOffset)
                String_base::Xran();

            size_type num = right.size() - rightOffset;

            if (num < count)
                count = num;

            if (npos - mySize_ <= count)
                String_base::Xlen();

            if (0 < count && Grow(num = mySize_ + count)) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset, mySize_ - Offset);

                if (this == &right)
                    Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                                   myPtr_() + (Offset < rightOffset ? rightOffset + count : rightOffset),
                                                   count);
                else
                    Traits_helper::copy_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                                   right.myPtr_() + rightOffset, count);

                Eos(num);
            }

            return (*this);
        }

        MyType&  insert(size_type Offset,
                        const TElement* Ptr, size_type count) {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1167);

            if (Inside(Ptr))
                return (insert(Offset, *this,
                               Ptr - myPtr_(), count));

            if (mySize_ < Offset)
                String_base::Xran();

            if (npos - mySize_ <= count)
                String_base::Xlen();

            size_type num;

            if (0 < count && Grow(num = mySize_ + count)) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset, mySize_ - Offset);
                Traits_helper::copy_s<TTraits>(myPtr_() + Offset, myRes_ - Offset, Ptr, count);
                Eos(num);
            }

            return (*this);
        }

        MyType&  insert(size_type Offset, const TElement* Ptr) {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1190);
            return (insert(Offset, Ptr, TTraits::length(Ptr)));
        }

        MyType&  insert(size_type Offset,
                        size_type count, TElement chr) {
            if (mySize_ < Offset)
                String_base::Xran();

            if (npos - mySize_ <= count)
                String_base::Xlen();

            size_type num;

            if (0 < count && Grow(num = mySize_ + count)) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset, mySize_ - Offset);
                ChAssign(Offset, count, chr);
                Eos(num);
            }

            return (*this);
        }

        iterator  insert(const_iterator Where) {
            return (insert(Where, TElement()));
        }

        iterator  insert(const_iterator Where, TElement chr) {
            size_type Offset = Pdif(Where, begin());
            insert(Offset, 1, chr);
            return (begin() + Offset);
        }

        void  insert(const_iterator Where, size_type count, TElement chr) {
            size_type Offset = Pdif(Where, begin());
            insert(Offset, count, chr);
        }

        template<class TIterator>
        void  insert(const_iterator Where, TIterator first, TIterator Last) {
            Insert(Where, first, Last, Iterator_cat(first));
        }

        template<class TIterator>
        void  Insert(const_iterator Where, TIterator count, TIterator chr,
                     Int_iterator_tag) {
            insert(Where, (size_type)count, (TElement)chr);
        }

        template<class TIterator>
        void  Insert(const_iterator Where, TIterator first, TIterator Last,
                     input_iterator_tag) {
            replace(Where, Where, first, Last);
        }

        void  insert(const_iterator Where, const_pointer first, const_pointer Last) {
            replace(Where, Where, first, Last);
        }

        void  insert(const_iterator Where, const_iterator first, const_iterator Last) {
            replace(Where, Where, first, Last);
        }

        MyType&  erase(size_type Offset = 0,
                       size_type count = npos) {
            if (mySize_ < Offset)
                String_base::Xran();

            if (mySize_ - Offset < count)
                count = mySize_ - Offset;

            if (0 < count) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset, myPtr_() + Offset + count,
                                               mySize_ - Offset - count);
                size_type newSize = mySize_ - count;
                Eos(newSize);
            }

            return (*this);
        }

        iterator  erase(const_iterator Where) {
            size_type count = Pdif(Where, begin());
            erase(count, 1);
            return (iterator(myPtr_() + count, this));
        }

        iterator  erase(const_iterator first, const_iterator Last) {
            size_type count = Pdif(first, begin());
            erase(count, Pdif(Last, first));
            return (iterator(myPtr_() + count, this));
        }

        void  clear() {
            erase(begin(), end());
        }

        MyType&  replace(size_type Offset, size_type N0, const MyType& right) {
            return (replace(Offset, N0, right, 0, npos));
        }

        MyType&  replace(size_type Offset,
                         size_type N0, const MyType& right, size_type rightOffset, size_type count) {
            if (mySize_ < Offset || right.size() < rightOffset)
                String_base::Xran();

            if (mySize_ - Offset < N0)
                N0 = mySize_ - Offset;

            size_type num = right.size() - rightOffset;

            if (num < count)
                count = num;

            if (npos - count <= mySize_ - N0)
                String_base::Xlen();

            size_type Nm = mySize_ - N0 - Offset;
            size_type newSize = mySize_ + count - N0;

            if (mySize_ < newSize)
                Grow(newSize);

            if (this != &right) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);
                Traits_helper::copy_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                               right.myPtr_() + rightOffset, count);
            } else if (count <= N0) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                               myPtr_() + rightOffset, count);
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);
            } else if (rightOffset <= Offset) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);
                Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                               myPtr_() + rightOffset, count);
            } else if (Offset + N0 <= rightOffset) {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);
                Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                               myPtr_() + (rightOffset + count - N0), count);
            } else {
                Traits_helper::move_s<TTraits>(myPtr_() + Offset, myRes_ - Offset,
                                               myPtr_() + rightOffset, N0);
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + N0, myRes_ - Offset - N0, myPtr_() + rightOffset + count,
                                               count - N0);
            }

            Eos(newSize);
            return (*this);
        }

        MyType&  replace(size_type Offset,
                         size_type N0, const TElement* Ptr, size_type count) {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1367);

            if (Inside(Ptr))
                return (replace(Offset, N0, *this,
                                Ptr - myPtr_(), count));

            if (mySize_ < Offset)
                String_base::Xran();

            if (mySize_ - Offset < N0)
                N0 = mySize_ - Offset;

            if (npos - count <= mySize_ - N0)
                String_base::Xlen();

            size_type Nm = mySize_ - N0 - Offset;

            if (count < N0)
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);

            size_type num;

            if ((0 < count || 0 < N0) && Grow(num = mySize_ + count - N0)) {
                if (N0 < count)
                    Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                                   myPtr_() + Offset + N0, Nm);

                Traits_helper::copy_s<TTraits>(myPtr_() + Offset, myRes_ - Offset, Ptr, count);
                Eos(num);
            }

            return (*this);
        }

        MyType&  replace(size_type Offset, size_type N0, const TElement* Ptr) {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1398);
            return (replace(Offset, N0, Ptr, TTraits::length(Ptr)));
        }

        MyType&  replace(size_type Offset,
                         size_type N0, size_type count, TElement chr) {
            if (mySize_ < Offset)
                String_base::Xran();

            if (mySize_ - Offset < N0)
                N0 = mySize_ - Offset;

            if (npos - count <= mySize_ - N0)
                String_base::Xlen();

            size_type Nm = mySize_ - N0 - Offset;

            if (count < N0)
                Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                               myPtr_() + Offset + N0, Nm);

            size_type num;

            if ((0 < count || 0 < N0) && Grow(num = mySize_ + count - N0)) {
                if (N0 < count)
                    Traits_helper::move_s<TTraits>(myPtr_() + Offset + count, myRes_ - Offset - count,
                                                   myPtr_() + Offset + N0, Nm);

                ChAssign(Offset, count, chr);
                Eos(num);
            }

            return (*this);
        }

        MyType&  replace(const_iterator first, const_iterator Last, const MyType& right) {
            return (replace(
                        Pdif(first, begin()), Pdif(Last, first), right));
        }

        MyType&  replace(const_iterator first, const_iterator Last, const TElement* Ptr,
                         size_type count) {
            return (replace(
                        Pdif(first, begin()), Pdif(Last, first), Ptr, count));
        }

        MyType&  replace(const_iterator first, const_iterator Last, const TElement* Ptr) {
            return (replace(
                        Pdif(first, begin()), Pdif(Last, first), Ptr));
        }

        MyType&  replace(const_iterator first, const_iterator Last,
                         size_type count, TElement chr) {
            return (replace(
                        Pdif(first, begin()), Pdif(Last, first), count, chr));
        }

        template<class TIterator>
        MyType&  replace(const_iterator first, const_iterator Last,
                         TIterator first2, TIterator Last2) {
            return (Replace(first, Last,
                            first2, Last2, Iterator_cat(first2)));
        }

        template<class TIterator>
        MyType&  Replace(const_iterator first, const_iterator Last,
                         TIterator count, TIterator chr, Int_iterator_tag) {
            return (replace(first, Last, (size_type)count, (TElement)chr));
        }

        template<class TIterator>
        MyType&  Replace(const_iterator first, const_iterator Last,
                         TIterator first2, TIterator Last2, input_iterator_tag) {
            MyType right(first2, Last2);
            replace(first, Last, right);
            return (*this);
        }

        MyType&  replace(const_iterator first, const_iterator Last,
                         const_pointer first2, const_pointer Last2) {
            if (first2 == Last2)
                erase(Pdif(first, begin()), Pdif(Last, first));
            else
                replace(Pdif(first, begin()), Pdif(Last, first),
                        &*first2, Last2 - first2);

            return (*this);
        }

        MyType&  replace(const_iterator first, const_iterator Last,
                         const_iterator first2, const_iterator Last2) {
            if (first2 == Last2)
                erase(Pdif(first, begin()), Pdif(Last, first));
            else
                replace(Pdif(first, begin()), Pdif(Last, first),
                        &*first2, Last2 - first2);

            return (*this);
        }

        iterator  begin() {
            return (iterator(myPtr_(), this));
        }

        const_iterator  begin() const {
            return (const_iterator(myPtr_(), this));
        }

        iterator  end() {
            return (iterator(myPtr_() + mySize_, this));
        }

        const_iterator  end() const {
            return (const_iterator(myPtr_() + mySize_, this));
        }

        reverse_iterator  rbegin() {
            return (reverse_iterator(end()));
        }

        const_reverse_iterator  rbegin() const {
            return (const_reverse_iterator(end()));
        }

        reverse_iterator  rend() {
            return (reverse_iterator(begin()));
        }

        const_reverse_iterator  rend() const {
            return (const_reverse_iterator(begin()));
        }

        reference  at(size_type Offset) {
            if (mySize_ <= Offset)
                String_base::Xran();

            return (myPtr_()[Offset]);
        }

        const_reference  at(size_type Offset) const {
            if (mySize_ <= Offset)
                String_base::Xran();

            return (myPtr_()[Offset]);
        }

        reference  operator[](size_type Offset) {
            if (this->myFirstIterator_ != ((Iterator_base*) - 3)) {
                if (mySize_ < Offset) {
                    Debug_message(L"string subscript out of range", L"..\\include\\xstring", 1565);
                    {
                        (void)((!!((("Standard C++ Libraries Out of Range", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 1566, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 1566, 0);
                    };
                }
            }

            return (myPtr_()[Offset]);
        }

        const_reference  operator[](size_type Offset) const {
            if (this->myFirstIterator_ != ((Iterator_base*) - 3)) {
                if (mySize_ < Offset) {
                    Debug_message(L"string subscript out of range", L"..\\include\\xstring", 1585);
                    {
                        (void)((!!((("Standard C++ Libraries Out of Range", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xstring", 1586, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xstring", 1586, 0);
                    };
                }
            }

            return (myPtr_()[Offset]);
        }

        void  push_back(TElement chr) {
            insert(end(), chr);
        }

        const TElement* c_str() const {
            return (myPtr_());
        }

        const TElement* data() const {
            return (c_str());
        }

        size_type  length() const {
            return (mySize_);
        }

        size_type  size() const {
            return (mySize_);
        }

        size_type  max_size() const {
            size_type num = MyBaseType::altVal_.max_size();
            return (num <= 1 ? 1 : num - 1);
        }

        void  resize(size_type newSize) {
            resize(newSize, TElement());
        }

        void  resize(size_type newSize, TElement chr) {
            if (newSize <= mySize_)
                erase(newSize);
            else
                append(newSize - mySize_, chr);
        }

        size_type  capacity() const {
            return (myRes_);
        }

        void  reserve(size_type _Newcap = 0) {
            if (mySize_ <= _Newcap && myRes_ != _Newcap) {
                size_type Size = mySize_;

                if (Grow(_Newcap, true))
                    Eos(Size);
            }
        }

        bool  empty() const {
            return (mySize_ == 0);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        size_type  copy(TElement* Dest,
                        size_type count, size_type Offset = 0) const {
            if (count != 0)
                Debug_pointer(Dest, L"..\\include\\xstring", 1715);

            return Copy_s(Dest, count, count, Offset);
        }

        size_type  Copy_s(TElement* Dest, size_type destSize,
                          size_type count, size_type Offset = 0) const {
            Debug_pointer(Dest, L"..\\include\\xstring", 1725);

            if (mySize_ < Offset)
                String_base::Xran();

            if (mySize_ - Offset < count)
                count = mySize_ - Offset;

            Traits_helper::copy_s<TTraits>(Dest, destSize, myPtr_() + Offset, count);
            return (count);
        }

        void  swap(MyType& right) {
            if (this == &right)
                ;
            else if (MyBaseType::altVal_ == right.altVal_) {
                this->Swap_all(right);
                BoxType _Tbx = Box;
                Box = right.Box, right.Box = _Tbx;
                size_type tempLen = mySize_;
                mySize_ = right.mySize_, right.mySize_ = tempLen;
                size_type tempRes = myRes_;
                myRes_ = right.myRes_, right.myRes_ = tempRes;
            } else {
                MyType temp = *this;
                *this = right;
                right = temp;
            }
        }

        size_type  find(const MyType& right, size_type Offset = 0) const {
            return (find(right.myPtr_(), Offset, right.size()));
        }

        size_type  find(const TElement* Ptr,
                        size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1774);

            if (count == 0 && Offset <= mySize_)
                return (Offset);

            size_type Nm;

            if (Offset < mySize_ && count <= (Nm = mySize_ - Offset)) {
                const TElement* uPtr, *vPtr;

                for (Nm -= count - 1, vPtr = myPtr_() + Offset;
                        (uPtr = TTraits::find(vPtr, Nm, *Ptr)) != 0;
                        Nm -= uPtr - vPtr + 1, vPtr = uPtr + 1)
                    if (TTraits::compare(uPtr, Ptr, count) == 0)
                        return (uPtr - myPtr_());
            }

            return (npos);
        }

        size_type  find(const TElement* Ptr, size_type Offset = 0) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1796);
            return (find(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  find(TElement chr, size_type Offset = 0) const {
            return (find((const TElement*)&chr, Offset, 1));
        }

        size_type  rfind(const MyType& right, size_type Offset = npos) const {
            return (rfind(right.myPtr_(), Offset, right.size()));
        }

        size_type  rfind(const TElement* Ptr,
                         size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1816);

            if (count == 0)
                return (Offset < mySize_ ? Offset : mySize_);

            if (count <= mySize_) {
                const TElement* uPtr = myPtr_() +
                                       (Offset < mySize_ - count ? Offset : mySize_ - count);

                for (; ; --uPtr)
                    if (TTraits::eq(*uPtr, *Ptr)
                            && TTraits::compare(uPtr, Ptr, count) == 0)
                        return (uPtr - myPtr_());
                    else if (uPtr == myPtr_())
                        break;
            }

            return (npos);
        }

        size_type  rfind(const TElement* Ptr, size_type Offset = npos) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1838);
            return (rfind(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  rfind(TElement chr, size_type Offset = npos) const {
            return (rfind((const TElement*)&chr, Offset, 1));
        }

        size_type  find_first_of(const MyType& right,
                                 size_type Offset = 0) const {
            return (find_first_of(right.myPtr_(), Offset, right.size()));
        }

        size_type  find_first_of(const TElement* Ptr,
                                 size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1859);

            if (0 < count && Offset < mySize_) {
                const TElement* const vPtr = myPtr_() + mySize_;

                for (const TElement* uPtr = myPtr_() + Offset; uPtr < vPtr; ++uPtr)
                    if (TTraits::find(Ptr, count, *uPtr) != 0)
                        return (uPtr - myPtr_());
            }

            return (npos);
        }

        size_type  find_first_of(const TElement* Ptr, size_type Offset = 0) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1875);
            return (find_first_of(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  find_first_of(TElement chr, size_type Offset = 0) const {
            return (find((const TElement*)&chr, Offset, 1));
        }

        size_type  find_last_of(const MyType& right,
                                size_type Offset = npos) const {
            return (find_last_of(right.myPtr_(), Offset, right.size()));
        }

        size_type  find_last_of(const TElement* Ptr,
                                size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1896);

            if (0 < count && 0 < mySize_)
                for (const TElement* uPtr = myPtr_()
                                            + (Offset < mySize_ ? Offset : mySize_ - 1); ; --uPtr)
                    if (TTraits::find(Ptr, count, *uPtr) != 0)
                        return (uPtr - myPtr_());
                    else if (uPtr == myPtr_())
                        break;

            return (npos);
        }

        size_type  find_last_of(const TElement* Ptr,
                                size_type Offset = npos) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1913);
            return (find_last_of(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  find_last_of(TElement chr, size_type Offset = npos) const {
            return (rfind((const TElement*)&chr, Offset, 1));
        }

        size_type  find_first_not_of(const MyType& right,
                                     size_type Offset = 0) const {
            return (find_first_not_of(right.myPtr_(), Offset,
                                      right.size()));
        }

        size_type  find_first_not_of(const TElement* Ptr,
                                     size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1935);

            if (Offset < mySize_) {
                const TElement* const vPtr = myPtr_() + mySize_;

                for (const TElement* uPtr = myPtr_() + Offset; uPtr < vPtr; ++uPtr)
                    if (TTraits::find(Ptr, count, *uPtr) == 0)
                        return (uPtr - myPtr_());
            }

            return (npos);
        }

        size_type  find_first_not_of(const TElement* Ptr,
                                     size_type Offset = 0) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1951);
            return (find_first_not_of(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  find_first_not_of(TElement chr, size_type Offset = 0) const {
            return (find_first_not_of((const TElement*)&chr, Offset, 1));
        }

        size_type  find_last_not_of(const MyType& right,
                                    size_type Offset = npos) const {
            return (find_last_not_of(right.myPtr_(), Offset, right.size()));
        }

        size_type  find_last_not_of(const TElement* Ptr,
                                    size_type Offset, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 1972);

            if (0 < mySize_)
                for (const TElement* uPtr = myPtr_()
                                            + (Offset < mySize_ ? Offset : mySize_ - 1); ; --uPtr)
                    if (TTraits::find(Ptr, count, *uPtr) == 0)
                        return (uPtr - myPtr_());
                    else if (uPtr == myPtr_())
                        break;

            return (npos);
        }

        size_type  find_last_not_of(const TElement* Ptr,
                                    size_type Offset = npos) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 1988);
            return (find_last_not_of(Ptr, Offset, TTraits::length(Ptr)));
        }

        size_type  find_last_not_of(TElement chr, size_type Offset = npos) const {
            return (find_last_not_of((const TElement*)&chr, Offset, 1));
        }

        MyType  substr(size_type Offset = 0, size_type count = npos) const {
            return (MyType(*this, Offset, count, get_allocator()));
        }

        int  compare(const MyType& right) const {
            return (compare(0, mySize_, right.myPtr_(), right.size()));
        }

        int  compare(size_type Offset, size_type N0,
                     const MyType& right) const {
            return (compare(Offset, N0, right, 0, npos));
        }

        int  compare(size_type Offset,
                     size_type N0, const MyType& right,
                     size_type rightOffset, size_type count) const {
            if (right.size() < rightOffset)
                String_base::Xran();

            if (right.mySize_ - rightOffset < count)
                count = right.mySize_ - rightOffset;

            return (compare(Offset, N0, right.myPtr_() + rightOffset, count));
        }

        int  compare(const TElement* Ptr) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 2026);
            return (compare(0, mySize_, Ptr, TTraits::length(Ptr)));
        }

        int  compare(size_type Offset, size_type N0, const TElement* Ptr) const {
            Debug_pointer(Ptr, L"..\\include\\xstring", 2032);
            return (compare(Offset, N0, Ptr, TTraits::length(Ptr)));
        }

        int  compare(size_type Offset,
                     size_type N0, const TElement* Ptr, size_type count) const {
            if (count != 0)
                Debug_pointer(Ptr, L"..\\include\\xstring", 2042);

            if (mySize_ < Offset)
                String_base::Xran();

            if (mySize_ - Offset < N0)
                N0 = mySize_ - Offset;

            size_type Ans = TTraits::compare(myPtr_() + Offset, Ptr,
                                             N0 < count ? N0 : count);
            return (Ans != 0 ? (int)Ans : N0 < count ? -1
                    : N0 == count ? 0 : +1);
        }

        allocatorTpe  get_allocator() const {
            return (MyBaseType::altVal_);
        }

        enum {
            BUFFER_SIZE = 16 / sizeof(TElement) < 1 ? 1
            : 16 / sizeof(TElement)
        };

    protected:
        enum {
            ALLOC_MASK = sizeof(TElement) <= 1 ? 15
            : sizeof(TElement) <= 2 ? 7
            : sizeof(TElement) <= 4 ? 3
            : sizeof(TElement) <= 8 ? 1 : 0
        };

        void  ChAssign(size_type Offset, size_type count, TElement chr) {
            if (count == 1)
                TTraits::assign(*(myPtr_() + Offset), chr);
            else
                TTraits::assign(myPtr_() + Offset, count, chr);
        }

        void  Copy(size_type newSize, size_type oldLen) {
            size_type newRes = newSize | ALLOC_MASK;

            if (max_size() < newRes)
                newRes = newSize;
            else if (newRes / 3 < myRes_ / 2
                     && myRes_ <= max_size() - myRes_ / 2)
                newRes = myRes_ + myRes_ / 2;

            TElement* Ptr = 0;

            try {
                Ptr = MyBaseType::altVal_.allocate(newRes + 1);
            } catch (...) {
                newRes = newSize;

                try {
                    Ptr = MyBaseType::altVal_.allocate(newRes + 1);
                } catch (...) {
                    Tidy_(true);
                    throw;
                }
            }

            if (0 < oldLen)
                Traits_helper::copy_s<TTraits>(Ptr, newRes + 1, myPtr_(), oldLen);

            Tidy_(true);
            Box.Ptr = Ptr;
            myRes_ = newRes;
            Eos(oldLen);
        }

        void  Eos(size_type newSize) {
            TTraits::assign(myPtr_()[mySize_ = newSize], TElement());
        }

        bool  Grow(size_type newSize,
                   bool _Trim = false) {
            if (max_size() < newSize)
                String_base::Xlen();

            if (myRes_ < newSize)
                Copy(newSize, mySize_);
            else if (_Trim && newSize < BUFFER_SIZE)
                Tidy_(true,
                      newSize < mySize_ ? newSize : mySize_);
            else if (newSize == 0)
                Eos(0);

            return (0 < newSize);
        }

        bool  Inside(const TElement* Ptr) {
            if (Ptr == 0 || Ptr < myPtr_() || myPtr_() + mySize_ <= Ptr)
                return (false);
            else
                return (true);
        }

        static size_type __cdecl Pdif(const_iterator _P2,
                                      const_iterator _P1) {
            return ((_P2).myPtr_ == 0 ? 0 : _P2 - _P1);
        }

        void  Tidy_(bool isBuilt = false,
                    size_type newSize = 0) {
            if (!isBuilt)
                ;
            else if (BUFFER_SIZE <= myRes_) {
                TElement* Ptr = Box.Ptr;

                if (0 < newSize)
                    Traits_helper::copy_s<TTraits>(Box.Buffer, BUFFER_SIZE, Ptr, newSize);

                MyBaseType::altVal_.deallocate(Ptr, myRes_ + 1);
            }

            myRes_ = BUFFER_SIZE - 1;
            Eos(newSize);
        }

        union BoxType {
            TElement Buffer[BUFFER_SIZE];
            TElement* Ptr;
        } Box;

        TElement* myPtr_() {
            return (BUFFER_SIZE <= myRes_ ? Box.Ptr : Box.Buffer);
        }

        const TElement* myPtr_() const {
            return (BUFFER_SIZE <= myRes_ ? Box.Ptr : Box.Buffer);
        }

        size_type mySize_;
        size_type myRes_;
    };
    template<class TElement, class TTraits, class TAllocator>
    class Move_operation_category<basic_string<TElement, TTraits, TAllocator> > {
    public:
        typedef Swap_move_tag Move_cat;
    };
    template < class TElement,
             class TTraits,
             class TAllocator >
    const typename basic_string<TElement, TTraits, TAllocator>::size_type
    basic_string<TElement, TTraits, TAllocator>::npos =
        (typename basic_string<TElement, TTraits, TAllocator>::size_type)(-1);

    template < class TElement,
             class TTraits,
             class TAllocator > inline
    void __cdecl swap(basic_string<TElement, TTraits, TAllocator>& left,
                      basic_string<TElement, TTraits, TAllocator>& right) {
        left.swap(right);
    }

    typedef basic_string<char, char_traits<char>, allocator<char> >
    string;
    typedef basic_string < wchar_t, char_traits<wchar_t>,
            allocator<wchar_t> > wstring;

    template class __declspec(dllimport) allocator<char>;
    template class __declspec(dllimport) allocator<wchar_t>;
    template class __declspec(dllimport) String_val<char, allocator<char> >;
    template class __declspec(dllimport) String_val<wchar_t, allocator<wchar_t> >;
    template class __declspec(dllimport) basic_string < char, char_traits<char>,
             allocator<char> >;
    template class __declspec(dllimport) basic_string < wchar_t, char_traits<wchar_t>,
             allocator<wchar_t> >;

}

#pragma warning(default: 4251)
#pragma warning(pop)
#pragma pack(pop)

#pragma pack(push,8)
#pragma warning(push,3)

namespace std {
    class logic_error
            : public std:: exception {
    public:
        explicit  logic_error(const string& message)
            : str_(message) {
        }

        virtual  ~logic_error() throw() {
        }

        virtual const char* what() const throw() {
            return (str_.c_str());
        }

    private:
        string str_;
    };
    class domain_error
            : public logic_error {
    public:
        explicit  domain_error(const string& message)
            : logic_error(message) {
        }

        virtual  ~domain_error() throw() {
        }

    };
    class invalid_argument
            : public logic_error {
    public:
        explicit  invalid_argument(const string& message)
            : logic_error(message) {
        }

        virtual  ~invalid_argument() throw() {
        }

    };
    class length_error
            : public logic_error {
    public:
        explicit  length_error(const string& message)
            : logic_error(message) {
        }

        virtual  ~length_error() throw() {
        }

    };
    class out_of_range
            : public logic_error {
    public:
        explicit  out_of_range(const string& message)
            : logic_error(message) {
        }

        virtual  ~out_of_range() throw() {
        }

    };
    class runtime_error
            : public std:: exception {
    public:
        explicit  runtime_error(const string& message)
            : str_(message) {
        }

        virtual  ~runtime_error() throw() {
        }

        virtual const char* what() const throw() {
            return (str_.c_str());
        }

    private:
        string str_;
    };
    class overflow_error
            : public runtime_error {
    public:
        explicit  overflow_error(const string& message)
            : runtime_error(message) {
        }

        virtual  ~overflow_error() throw() {
        }

    };
    class underflow_error
            : public runtime_error {
    public:
        explicit  underflow_error(const string& message)
            : runtime_error(message) {
        }

        virtual  ~underflow_error() throw() {
        }

    };
    class range_error
            : public runtime_error {
    public:
        explicit  range_error(const string& message)
            : runtime_error(message) {
        }

        virtual  ~range_error() throw() {
        }

    };

}

#pragma warning(pop)
#pragma pack(pop)
#pragma once
#pragma pack(push,8)
struct Type_info_node {
    void* memPtr;
    Type_info_node* next;
};

extern Type_info_node Type_info_root_node;

class type_info {
public:
    virtual ~type_info();
    __declspec(dllimport) bool  operator==(const type_info& rhs) const;
    __declspec(dllimport) bool  operator!=(const type_info& rhs) const;
    __declspec(dllimport) int  before(const type_info& rhs) const;
    __declspec(dllimport) const char*  name(Type_info_node* __ptype_info_node = &Type_info_root_node) const;
    __declspec(dllimport) const char*  raw_name() const;
private:
    void* data_;
    char name_[1];
    type_info(const type_info& rhs);
    type_info&  operator=(const type_info& rhs);
    __declspec(dllimport) static const char* __cdecl Name_base(const type_info*, Type_info_node* __ptype_info_node);
    __declspec(dllimport) static void __cdecl Type_info_dtor(type_info*);
};

namespace std {

    using ::type_info;

}

namespace std {

    class __declspec(dllimport) bad_cast : public exception {
    public:
        bad_cast(const char* message = "bad cast");
        bad_cast(const bad_cast&);
        virtual  ~bad_cast();

    };

    class __declspec(dllimport) bad_typeid : public exception {
    public:
        bad_typeid(const char* message = "bad typeid");
        bad_typeid(const bad_typeid&);
        virtual  ~bad_typeid();
    };

    class __declspec(dllimport) __non_rtti_object : public bad_typeid {
    public:
        __non_rtti_object(const char* message);
        __non_rtti_object(const __non_rtti_object&);
        virtual  ~__non_rtti_object();

    };

}
#pragma pack(pop)

#pragma once
#pragma pack(push,8)
#pragma warning(push,3)
namespace std {
    struct DebugHeapTag_t {
        int Type;
    };
}

[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] __declspec(dllimport) void * __cdecl operator new(size_t Size,
        const std::DebugHeapTag_t&, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char*, int)
throw(...);

[returnvalue:SA_Post(Null=SA_No, WritableBytes="Size")] __declspec(dllimport) void * __cdecl operator new[](size_t Size,
        const std::DebugHeapTag_t&, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char*, int)
throw(...);

__declspec(dllimport) void __cdecl operator delete(void*,
        const std::DebugHeapTag_t&, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char*, int)
throw();

__declspec(dllimport) void __cdecl operator delete[](void*,
        const std::DebugHeapTag_t&, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char*, int)
throw();

namespace std {

    __declspec(dllimport) const DebugHeapTag_t& __cdecl DebugHeapTag_func();
    template<class T1>
    void __cdecl DebugHeapDelete(T1* Ptr) {
        if (Ptr != 0) {
            Ptr->~T1();
            free(Ptr);
        }
    }
    template<class T1>
    class DebugHeapAllocator
            : public allocator<T1> {
    public:

        template<class Other>
        struct rebind {
            typedef typename DebugHeapAllocator<Other> other;
        };

        typename allocator<T1>::pointer __cdecl allocate(typename allocator<T1>::size_type count, const void*) {
            if (count <= 0)
                count = 0;
            else if (((size_t)(-1) / count) < sizeof(T1))
                throw std::bad_alloc(0);

            return ((T1*)new(std::DebugHeapTag_func(), "..\\include\\xdebug", 86) char[count * sizeof(T1)]);
        }

        typename allocator<T1>::pointer  allocate(typename allocator<T1>::size_type count) {
            if (count <= 0)
                count = 0;
            else if (((size_t)(-1) / count) < sizeof(T1))
                throw std::bad_alloc(0);

            return ((T1*)new(std::DebugHeapTag_func(), "..\\include\\xdebug", 97) char[count * sizeof(T1)]);
        }

        void  deallocate(typename allocator<T1>::pointer Ptr, typename allocator<T1>::size_type) {
            std::DebugHeapDelete((void*)Ptr);
        }
    };

    template class __declspec(dllimport) DebugHeapAllocator<char>;
    class __declspec(dllimport) DebugHeapString
            : public basic_string<char, char_traits<char>, DebugHeapAllocator<char> > {
    public:
        typedef DebugHeapString MyType;
        typedef basic_string<char, char_traits<char>, DebugHeapAllocator<char> >
        MyBaseType;
        typedef char TElement;

        DebugHeapString()
            : MyBaseType() {
        }

        DebugHeapString(const MyType& right)
            : MyBaseType(right) {
        }

        DebugHeapString(const TElement* Ptr)
            : MyBaseType(Ptr) {
        }

        DebugHeapString(const string& str_)
            : MyBaseType(str_.c_str()) {
        }

        operator string() const {
            return (string(c_str()));
        }
    };
}
#pragma warning(pop)
#pragma pack(pop)

#pragma once

#pragma once

#pragma once
extern "C" {

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isctype([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Type);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isctype_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int Type, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl isalpha([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isalpha_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl isupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl islower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _islower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl isdigit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isdigit_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl isxdigit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isxdigit_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl isspace([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isspace_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl ispunct([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _ispunct_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl isalnum([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isalnum_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl isprint([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isprint_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl isgraph([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _isgraph_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl iscntrl([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _iscntrl_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl toupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl tolower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl _tolower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _tolower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(dllimport) int __cdecl _toupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _toupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __isascii([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __toascii([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __iscsymf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl __iscsym([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _C);

    __declspec(dllimport) int __cdecl _chvalidator([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int chr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mask);

    __declspec(dllimport) int __cdecl _chvalidator_l([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int chr, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mask);

}

#pragma once
#pragma pack(push,8)

extern "C" {
    struct lconv {
        char* decimal_point;
        char* thousands_sep;
        char* grouping;
        char* int_curr_symbol;
        char* currency_symbol;
        char* mon_decimal_point;
        char* mon_thousands_sep;
        char* mon_grouping;
        char* positive_sign;
        char* negative_sign;
        char int_frac_digits;
        char frac_digits;
        char p_cs_precedes;
        char p_sep_by_space;
        char n_cs_precedes;
        char n_sep_by_space;
        char p_sign_posn;
        char n_sign_posn;
    };
    __declspec(dllimport) int __cdecl _configthreadlocale([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Flag);
    __declspec(dllimport) char* __cdecl setlocale([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TCategory, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Locale);
    __declspec(dllimport) struct lconv* __cdecl localeconv(void);
    __declspec(dllimport) _locale_t __cdecl _get_current_locale(void);
    __declspec(dllimport) _locale_t __cdecl _create_locale([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TCategory, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Locale);
    __declspec(dllimport) void __cdecl _free_locale([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "_get_current_locale" " instead. See online help for details.")) __declspec(dllimport) _locale_t __cdecl __get_current_locale(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "_create_locale" " instead. See online help for details.")) __declspec(dllimport) _locale_t __cdecl __create_locale([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int TCategory, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Locale);
    __declspec(deprecated("This function or variable has been superceded by newer library or operating system functionality. Consider using " "_free_locale" " instead. See online help for details.")) __declspec(dllimport) void __cdecl __free_locale([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale);

}

#pragma pack(pop)
#pragma pack(push,8)
extern "C" {
    typedef struct Collvec {
        unsigned long Hand;
        unsigned int Page;
    } Collvec;

    typedef struct Ctypevec {
        unsigned long Hand;
        unsigned int Page;
        const short* Table;
        int Delfl;
    } Ctypevec;

    typedef struct Cvtvec {
        unsigned long Hand;
        unsigned int Page;
    } Cvtvec;
    __declspec(dllimport) Collvec __cdecl Getcoll();
    __declspec(dllimport) Ctypevec __cdecl Getctype();
    __declspec(dllimport) Cvtvec __cdecl Getcvt();

    __declspec(dllimport) int __cdecl Getdateorder();

    __declspec(dllimport) int __cdecl Mbrtowc([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t*, const char*, size_t,
            mbstate_t*, const Cvtvec*);
    __declspec(dllimport) float __cdecl Stof(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**, long);
    __declspec(dllimport) double __cdecl Stod(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**, long);
    __declspec(dllimport) long double __cdecl Stold(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**,
            long);
    __declspec(dllimport) int __cdecl Strcoll(const char*, const char*,
            const char*, const char*, const Collvec*);
    __declspec(dllimport) size_t __cdecl Strxfrm(
        [SA_Pre(Null=SA_No, WritableElements="\n@""_End1-_String1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="return")] [SA_Post(Deref=1, Valid=SA_Yes)] char* _String1,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* _End1, const char*, const char*, const Collvec*);
    __declspec(dllimport) int __cdecl _Tolower(int, const Ctypevec*);
    __declspec(dllimport) int __cdecl _Toupper(int, const Ctypevec*);
    __declspec(dllimport) int __cdecl _Wcrtomb([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] char*, wchar_t, mbstate_t*,
            const Cvtvec*);
    __declspec(dllimport) int __cdecl _Wcscoll(const wchar_t*, const wchar_t*,
            const wchar_t*, const wchar_t*, const Collvec*);
    __declspec(dllimport) size_t __cdecl _Wcsxfrm(
        [SA_Pre(Null=SA_No, WritableElements="\n@""_End1-_String1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="return")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String1,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* _End1, const wchar_t*, const wchar_t*, const Collvec*);

    __declspec(dllimport) short __cdecl _Getwctype(wchar_t, const Ctypevec*);
    __declspec(dllimport) const wchar_t* __cdecl _Getwctypes(const wchar_t*, const wchar_t*,
            short*, const Ctypevec*);
    __declspec(dllimport) wchar_t __cdecl _Towlower(wchar_t, const Ctypevec*);
    __declspec(dllimport) wchar_t __cdecl _Towupper(wchar_t, const Ctypevec*);

}

extern "C" {
    __declspec(dllimport) void* __cdecl _Gettnames();
    __declspec(dllimport) char* __cdecl _Getdays();
    __declspec(dllimport) char* __cdecl _Getmonths();
    __declspec(dllimport) size_t __cdecl _Strftime(
        [SA_Pre(Null=SA_No, WritableElements="_Maxsize")] [SA_Pre(Deref=1, Valid=SA_No)] char*, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Maxsize,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)]  const char*, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const struct tm*, void*);
}

extern "C" {
    _locale_t __cdecl _GetLocaleForCP(unsigned int);
}
#pragma pack(pop)

#pragma pack(push,8)
#pragma warning(push,3)
#pragma warning(disable:4412)
namespace std {
    class __declspec(dllimport) Timevec {
    public:
        Timevec(void* Ptr = 0)
            : timePtr_(Ptr) {
        }

        Timevec(const Timevec& right) {
            *this = right;
        }

        ~Timevec() {
            free(timePtr_);
        }

        Timevec&  operator=(const Timevec& right) {
            timePtr_ = right.timePtr_;
            ((Timevec*)&right)->timePtr_ = 0;
            return (*this);
        }

        void* Getptr() const {
            return (timePtr_);
        }

    private:
        void* timePtr_;
    };
#pragma warning(push)
#pragma warning(disable:4412)
    class __declspec(dllimport) LocInfo {
    public:
        typedef ::Collvec Collvec;
        typedef ::Ctypevec Ctypevec;
        typedef ::Cvtvec Cvtvec;
        typedef std::Timevec Timevec;

        static  void __cdecl LocInfo_ctor(LocInfo*, const char*);
        static  void __cdecl LocInfo_ctor(LocInfo*, const string&);
        static  void __cdecl LocInfo_ctor(LocInfo*, int, const char*);
        static  void __cdecl LocInfo_dtor(LocInfo*);
        static  LocInfo& __cdecl LocInfo_addCats(LocInfo*, int, const char*);

        LocInfo(const char* pChar = "C")

            : lock(0)

        {
            if (pChar == 0)
                throw runtime_error("bad locale name");

            LocInfo_ctor(this, pChar);
        }

        LocInfo(int I, const char* pChar)

            : lock(0)

        {
            if (pChar == 0)
                throw runtime_error("bad locale name");

            LocInfo_ctor(this, I, pChar);
        }

        LocInfo(const string& locstr)

            : lock(0)

        {
            LocInfo_ctor(this, locstr);
        }

        ~LocInfo() {
            LocInfo_dtor(this);
        }

        LocInfo&  _Addcats(int I, const char* pChar) {
            if (pChar == 0)
                throw runtime_error("bad locale name");

            return LocInfo_addCats(this, I, pChar);
        }

        string  GetName() const {
            return (newLocName_);
        }

        Collvec  Getcoll() const {
            return (::Getcoll());
        }

        Ctypevec  Getctype() const {
            return (::Getctype());
        }

        Cvtvec  Getcvt() const {
            return (::Getcvt());
        }

        const lconv* _Getlconv() const {
            return (localeconv());
        }

        Timevec  _Gettnames() const {
            return (Timevec(::_Gettnames()));
        }

        const char* _Getdays() const {
            const char* Ptr = ::_Getdays();

            if (Ptr != 0) {
                ((LocInfo*)this)->days_ = Ptr;
                free((void*)Ptr);
            }

            return (days_.size() != 0 ? days_.c_str()
                    : ":Sun:Sunday:Mon:Monday:Tue:Tuesday:Wed:Wednesday"
                    ":Thu:Thursday:Fri:Friday:Sat:Saturday");
        }

        const char* _Getmonths() const {
            const char* Ptr = ::_Getmonths();

            if (Ptr != 0) {
                ((LocInfo*)this)->months_ = Ptr;
                free((void*)Ptr);
            }

            return (months_.size() != 0 ? months_.c_str()
                    : ":Jan:January:Feb:February:Mar:March"
                    ":Apr:April:May:May:Jun:June"
                    ":Jul:July:Aug:August:Sep:September"
                    ":Oct:October:Nov:November:Dec:December");
        }

        const char* _Getfalse() const {
            return ("false");
        }

        const char* _Gettrue() const {
            return ("true");
        }

        int  Getdateorder() const {
            return ::Getdateorder();
        }

    private:

        Lockit lock;

        string days_;
        string months_;
        string oldLocName_;
        string newLocName_;
    };
#pragma warning(pop)
    template<class TElement> inline
    int __cdecl _LStrcoll(const TElement* first1, const TElement* Last1,
                          const TElement* first2, const TElement* Last2,
                          const LocInfo::Collvec*) {
        for (; first1 != Last1 && first2 != Last2; ++first1, ++first2)
            if (*first1 < *first2)
                return (-1);
            else if (*first2 < *first1)
                return (+1);

        return (first2 != Last2 ? -1 : first1 != Last1 ? +1 : 0);
    }

    template<> inline
    int __cdecl _LStrcoll(const char* first1, const char* Last1,
                          const char* first2, const char* Last2,
                          const LocInfo::Collvec* _Vector) {
        return (Strcoll(first1, Last1, first2, Last2, _Vector));
    }

    template<> inline
    int __cdecl _LStrcoll(const wchar_t* first1, const wchar_t* Last1,
                          const wchar_t* first2, const wchar_t* Last2,
                          const LocInfo::Collvec* _Vector) {
        return (_Wcscoll(first1, Last1, first2, Last2, _Vector));
    }
    template<class TElement> inline
    size_t __cdecl _LStrxfrm(TElement* first1, TElement* Last1,
                             const TElement* first2, const TElement* Last2,
                             const LocInfo::Collvec*) {
        size_t count = Last2 - first2;

        if (count <= (size_t)(Last1 - first1))
            ::memcpy_s((first1), ((Last1 - first1)* sizeof(TElement)), (first2), (count * sizeof(TElement)));

        return (count);
    }

    template<> inline
    size_t __cdecl _LStrxfrm(
        [SA_Pre(Null=SA_No, WritableElements="\n@""Last1-first1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="return")] [SA_Post(Deref=1, Valid=SA_Yes)] char* first1,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* Last1,
        const char* first2, const char* Last2,
        const LocInfo::Collvec* _Vector) {
        return (Strxfrm(first1, Last1, first2, Last2, _Vector));
    }

    template<> inline
    size_t __cdecl _LStrxfrm(
        [SA_Pre(Null=SA_No, WritableElements="\n@""Last1-first1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="return")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* first1,
        [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t* Last1,
        const wchar_t* first2, const wchar_t* Last2,
        const LocInfo::Collvec* _Vector) {
        return (_Wcsxfrm(first1, Last1, first2, Last2, _Vector));
    }
}

#pragma warning(pop)
#pragma pack(pop)
#pragma pack(push,8)
#pragma warning(push,3)
#pragma warning(disable:4412)
namespace std {
    template<class Dummy>
    class LocBase {
    public:
        static const int collate = ((1 << (1)) >> 1);
        static const int ctype = ((1 << (2)) >> 1);
        static const int monetary = ((1 << (3)) >> 1);
        static const int numeric = ((1 << (4)) >> 1);
        static const int time = ((1 << (5)) >> 1);
        static const int messages = ((1 << (6)) >> 1);
        static const int all = (((1 << (7)) >> 1) - 1);
        static const int none = 0;
    };

    template<class Dummy>
    const int LocBase<Dummy>::collate;
    template<class Dummy>
    const int LocBase<Dummy>::ctype;
    template<class Dummy>
    const int LocBase<Dummy>::monetary;
    template<class Dummy>
    const int LocBase<Dummy>::numeric;
    template<class Dummy>
    const int LocBase<Dummy>::time;
    template<class Dummy>
    const int LocBase<Dummy>::messages;
    template<class Dummy>
    const int LocBase<Dummy>::all;
    template<class Dummy>
    const int LocBase<Dummy>::none;
    class locale;
    template<class TFacet>
    const TFacet& __cdecl use_facet(const locale&);
#pragma warning(push)
#pragma warning(disable:4412)
    class __declspec(dllimport) locale
            : public LocBase<int> {
    public:
        typedef int category;
        class __declspec(dllimport) id {
        public:
            id(size_t Value = 0)
                : id_(Value) {
            }

            operator size_t() {
                if (id_ == 0) {
                    {
                        ::std:: Lockit lock(0);

                        if (id_ == 0)
                            id_ = ++id_count_;
                    }
                }

                return (id_);
            }

        private:
            id(const id&);
            id&  operator=(const id&);

            size_t id_;
            static  int& __cdecl id_count_func_();

            static int id_count_;

        };

        class LocImp;
        class facet {
            friend class locale;
            friend class LocImp;

        public:
            __declspec(dllimport) static size_t __cdecl GetCat(const facet** = 0,
                    const locale * = 0) {
                return ((size_t)(-1));
            }

            __declspec(dllimport) void  IncRef() {
                {
                    ::std:: Lockit lock(0);

                    if (refs_ < (size_t)(-1))
                        ++refs_;
                }
            }

            __declspec(dllimport) facet* DecRef() {
                {
                    ::std:: Lockit lock(0);

                    if (0 < refs_ && refs_ < (size_t)(-1))
                        --refs_;

                    return (refs_ == 0 ? this : 0);
                }
            }

            void  Register() {
                facet_Register(this);
            }
            __declspec(dllimport) void* __cdecl operator new(size_t Size) {
                return (operator new(Size, std::DebugHeapTag_func(),
                                     "..\\include\\xlocale", 146));
            }

            __declspec(dllimport) void* __cdecl operator new(size_t Size,
                    const std::DebugHeapTag_t& _Tag, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* File, int Line) {
                return (::operator new(Size, _Tag, File, Line));
            }

            __declspec(dllimport) void __cdecl operator delete(void* Ptr,
                    const std::DebugHeapTag_t&, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char*, int) {
                operator delete(Ptr);
            }

            __declspec(dllimport) void __cdecl operator delete(void* Ptr) {
                std::DebugHeapDelete((facet*)Ptr);
            }
            __declspec(dllimport) virtual  ~facet() {
            }

        protected:
            __declspec(dllimport) explicit  facet(size_t initRefs = 0)
                : refs_(initRefs) {
            }

        private:

            static void __cdecl facet_Register(facet*);
            facet(const facet&);
            facet&  operator=(const facet&);

            size_t refs_;
        };

#pragma warning(push)
#pragma warning(disable:4275)

        class __declspec(dllimport) LocImp
                : public facet {
        protected:
            ~LocImp() {
                _Locimp_dtor(this);
            }

        private:
            static  void __cdecl _Locimp_dtor(LocImp*);
            static void _Locimp_ctor(LocImp*, const LocImp&);
            static  void __cdecl _Locimp_Addfac(LocImp*, facet*, size_t);
            friend class locale;

            LocImp(bool _Transparent = false)
                : locale::facet(1), _Facetvec(0), _Facetcount(0),
                  _Catmask(none), _Xparent(_Transparent), _Name("*")
            { }

            LocImp(const LocImp& right)
                : locale::facet(1), _Facetvec(0), _Facetcount(right._Facetcount),
                  _Catmask(right._Catmask), _Xparent(right._Xparent), _Name(right._Name) {
                _Locimp_ctor(this, right);
            }

            void  _Addfac(facet* _Pfacet, size_t id_) {
                _Locimp_Addfac(this, _Pfacet, id_);
            }

            static LocImp* _Makeloc(const LocInfo&,
                                    category, LocImp*, const locale*);

            static void _Makewloc(const LocInfo&,
                                  category, LocImp*, const locale*);
            static void _Makeushloc(const LocInfo&,
                                    category, LocImp*, const locale*);
            static void _Makexloc(const LocInfo&,
                                  category, LocImp*, const locale*);

            facet** _Facetvec;
            size_t _Facetcount;
            category _Catmask;
            bool _Xparent;
            DebugHeapString _Name;

            static  LocImp *& __cdecl _Clocptr_func();

            static LocImp* _Clocptr;

        private:
            LocImp&  operator=(const LocImp&);

        };
#pragma warning(pop)

        __declspec(deprecated("This is an obsolete part of the Standard C++ Library Implementation. Do not use it.")) locale&  _Addfac(facet* _Fac, size_t id_,
                size_t _Catmask) {
            if (1 < this->Ptr->refs_) {
                this->Ptr->DecRef();
                this->Ptr = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 265) LocImp(*this->Ptr);
            }

            this->Ptr->_Addfac(_Fac, id_);

            if (_Catmask != 0)
                this->Ptr->_Name = "*";

            return (*this);
        }
        template < class TElement,
                 class TTraits,
                 class TAllocator >
        bool  operator()(const basic_string<TElement, TTraits, TAllocator>& left,
                         const basic_string<TElement, TTraits, TAllocator>& right) const {
            const std::collate<TElement>& _Coll_fac =
                std::use_facet<std::collate<TElement> >(*this);

            return (_Coll_fac.compare(left.c_str(), left.c_str() + left.size(),
                                      right.c_str(), right.c_str() + right.size()) < 0);
        }

        template<class TFacet>
        locale  combine(const locale& _Loc) const {
            TFacet* _Facptr;

            try {
                _Facptr = (TFacet*)&std::use_facet<TFacet>(_Loc);
            } catch (...) {
                throw runtime_error("locale::combine facet missing");
            }

            LocImp* _Newimp = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 299) LocImp(*Ptr);
            _Newimp->_Addfac(_Facptr, TFacet::id);
            _Newimp->_Catmask = 0;
            _Newimp->_Name = "*";
            return (locale(_Newimp));
        }

        template<class TFacet>
        locale(const locale& _Loc, const TFacet* _Facptr)
            : Ptr(new(std::DebugHeapTag_func(), "..\\include\\xlocale", 308) LocImp(*_Loc.Ptr)) {
            if (_Facptr != 0) {
                Ptr->_Addfac((TFacet*)_Facptr, TFacet::id);

                if (TFacet::GetCat() != (size_t)(-1)) {
                    Ptr->_Catmask = 0;
                    Ptr->_Name = "*";
                }
            }
        }
        locale() throw()
            : Ptr(_Init()) {
            _Getgloballocale()->IncRef();
        }

        locale(_Uninitialized) {
        }

        locale(const locale& right) throw()
            : Ptr(right.Ptr) {
            Ptr->IncRef();
        }

        locale(const locale& _Loc, const locale& Other,
               category Cat)
            : Ptr(new(std::DebugHeapTag_func(), "..\\include\\xlocale", 340) LocImp(*_Loc.Ptr)) {
            try {
                {
                    LocInfo _Lobj(_Loc.Ptr->_Catmask, _Loc.Ptr->_Name.c_str());
                    LocImp::_Makeloc(_Lobj._Addcats(Cat & Other.Ptr->_Catmask,
                                                    Other.Ptr->_Name.c_str()), Cat, Ptr, &Other);
                }
            } catch (...) {
                std::DebugHeapDelete(Ptr->DecRef());
                throw;
            }
        }

        explicit  locale(const char* _Locname,
                         category Cat = all)
            : Ptr(new(std::DebugHeapTag_func(), "..\\include\\xlocale", 355) LocImp) {
            try {
                _Init();
                {
                    LocInfo _Lobj(Cat, _Locname);

                    if (_Lobj.GetName().compare("*") == 0)
                        throw runtime_error("bad locale name");

                    LocImp::_Makeloc(_Lobj, Cat, Ptr, 0);
                }
            } catch (...) {
                std::DebugHeapDelete(Ptr->DecRef());
                throw;
            }
        }

        locale(const locale& _Loc, const char* _Locname,
               category Cat)
            : Ptr(new(std::DebugHeapTag_func(), "..\\include\\xlocale", 372) LocImp(*_Loc.Ptr)) {
            try {
                {
                    LocInfo _Lobj(_Loc.Ptr->_Catmask, _Loc.Ptr->_Name.c_str());
                    bool _Hadname = _Lobj.GetName().compare("*") != 0;
                    _Lobj._Addcats(Cat, _Locname);

                    if (_Hadname && _Lobj.GetName().compare("*") == 0)
                        throw runtime_error("bad locale name");

                    LocImp::_Makeloc(_Lobj, Cat, Ptr, 0);
                }
            } catch (...) {
                std::DebugHeapDelete(Ptr->DecRef());
                throw;
            }
        }

        ~locale() throw() {
            if (Ptr != 0)
                std::DebugHeapDelete(Ptr->DecRef());
        }

        locale&  operator=(const locale& right) throw() {
            if (Ptr != right.Ptr) {
                std::DebugHeapDelete(Ptr->DecRef());
                Ptr = right.Ptr;
                Ptr->IncRef();
            }

            return (*this);
        }

        string  name() const {
            return (Ptr->_Name);
        }

        const facet* _Getfacet(size_t id_) const {
            const facet* _Facptr = id_ < Ptr->_Facetcount
                                   ? Ptr->_Facetvec[id_] : 0;

            if (_Facptr != 0 || !Ptr->_Xparent)
                return (_Facptr);
            else {
                locale::LocImp* Ptr = _Getgloballocale();
                return (id_ < Ptr->_Facetcount
                        ? Ptr->_Facetvec[id_]
                        : 0);
            }
        }
        bool  operator==(const locale& _Loc) const {
            return (Ptr == _Loc.Ptr
                    || name().compare("*") != 0 && name().compare(_Loc.name()) == 0);
        }

        bool  operator!=(const locale& right) const {
            return (!(*this == right));
        }

        static  const locale& __cdecl classic();

        static  locale __cdecl global(const locale&);

        static  locale __cdecl empty();

    private:
        locale(LocImp* _Ptrimp)
            : Ptr(_Ptrimp) {
        }

        static  LocImp* __cdecl _Getgloballocale();
        static  LocImp* __cdecl _Init();
        static  void __cdecl _Setgloballocale(void*);

        LocImp* Ptr;
    };
#pragma warning(pop)
    template<class TFacet>
    struct _Facetptr {
        static const locale::facet* _Psave;
    };

    template<class TFacet>
    const locale::facet* _Facetptr<TFacet>::_Psave = 0;

    template<class TFacet> inline __declspec(deprecated("This is an obsolete part of the Standard C++ Library Implementation. Do not use it."))
    locale _Addfac(locale _Loc, const TFacet* _Facptr) {
        size_t Cat = TFacet::GetCat(0, 0);
        locale _Newloc = _Loc._Addfac((TFacet*)_Facptr, TFacet::id, Cat);
        return (_Newloc);
    }
    template<class TFacet> inline
    const TFacet& __cdecl use_facet(const locale& _Loc) {
        {
            ::std:: Lockit lock(0);
            const locale::facet* _Psave =
                _Facetptr<TFacet>::_Psave;
            size_t id_ = TFacet::id;
            const locale::facet* _Pf = _Loc._Getfacet(id_);

            if (_Pf != 0)
                ;
            else if (_Psave != 0)
                _Pf = _Psave;
            else if (TFacet::GetCat(&_Psave, &_Loc) == (size_t)(-1))
                throw bad_cast();
            else {
                _Pf = _Psave;
                _Facetptr<TFacet>::_Psave = _Psave;
                locale::facet* _Pfmod = (TFacet*)_Psave;
                _Pfmod->IncRef();
                _Pfmod->DecRef();
            }

            return ((const TFacet&)(*_Pf));
        }
    }

    template<class TFacet> inline __declspec(deprecated("This is an obsolete part of the Standard C++ Library Implementation. Do not use it."))
    const TFacet& __cdecl use_facet(const locale& _Loc, const TFacet*,
                                    bool = false) {
        return use_facet<TFacet>(_Loc);
    }
    template < class TElement,
             class TInputIterator > inline
    int __cdecl _Getloctxt(TInputIterator& first, TInputIterator& Last, size_t _Numfields,
                           const TElement* Ptr) {
        for (size_t Offset = 0; Ptr[Offset] != (TElement)0; ++Offset)
            if (Ptr[Offset] == Ptr[0])
                ++_Numfields;

        string str_(_Numfields, '\0');

        int Ans = -2;

        for (size_t _Column = 1; ; ++_Column, ++first, Ans = -1) {
            bool  _Prefix = false;
            size_t Offset = 0;
            size_t _Field = 0;

            for (; _Field < _Numfields; ++_Field) {
                for (; Ptr[Offset] != (TElement)0 && Ptr[Offset] != Ptr[0]; ++Offset)
                    ;

                if (str_[_Field] != '\0')
                    Offset += str_[_Field];
                else if (Ptr[Offset += _Column] == Ptr[0]
                         || Ptr[Offset] == (TElement)0) {
                    str_[_Field] = (char)(_Column < 127
                                          ? _Column : 127);
                    Ans = (int)_Field;
                } else if (first == Last || Ptr[Offset] != *first)
                    str_[_Field] = (char)(_Column < 127
                                          ? _Column : 127);
                else
                    _Prefix = true;
            }

            if (!_Prefix || first == Last)
                break;
        }

        return (Ans);
    }

    template<class TElement> inline
    char __cdecl _Maklocbyte(TElement _Char,
                             const LocInfo::Cvtvec&) {
        return ((char)(unsigned char)_Char);
    }

    template<> inline
    char __cdecl _Maklocbyte(wchar_t _Char,
                             const LocInfo::Cvtvec& _Cvt) {
        char byte = '\0';
        mbstate_t _Mbst1 = {0};
        _Wcrtomb(&byte, _Char, &_Mbst1, &_Cvt);
        return (byte);
    }
    template<class TElement> inline
    TElement __cdecl _Maklocchr(char byte, TElement*,
                                const LocInfo::Cvtvec&) {
        return ((TElement)(unsigned char)byte);
    }

    template<> inline
    wchar_t __cdecl _Maklocchr(char byte, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t*,
                               const LocInfo::Cvtvec& _Cvt) {
        wchar_t _Wc = L'\0';
        mbstate_t _Mbst1 = {0};
        Mbrtowc(&_Wc, &byte, 1, &_Mbst1, &_Cvt);
        return (_Wc);
    }
    template<class TElement> inline
    TElement* __cdecl _Maklocstr(const char* Ptr, TElement*,
                                 const LocInfo::Cvtvec&) {
        size_t count = ::strlen(Ptr) + 1;
        TElement* _Ptrdest = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 626) TElement[count];
#pragma warning(push)
#pragma warning(disable: 6011)

        for (TElement* _Ptrnext = _Ptrdest; 0 < count; --count, ++_Ptrnext, ++Ptr)
            *_Ptrnext = (TElement)(unsigned char) * Ptr;

        return (_Ptrdest);
#pragma warning(pop)
    }

    template<> inline
    wchar_t* __cdecl _Maklocstr(const char* Ptr, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] wchar_t*,
                                const LocInfo::Cvtvec& _Cvt) {
        size_t count, _Count1;
        size_t _Wchars;
        const char* _Ptr1;
        int _Bytes;
        wchar_t _Wc;
        mbstate_t _Mbst1 = {0};
        _Count1 = ::strlen(Ptr) + 1;

        for (count = _Count1, _Wchars = 0, _Ptr1 = Ptr; 0 < count;
                count -= _Bytes, _Ptr1 += _Bytes, ++_Wchars)
            if ((_Bytes = Mbrtowc(&_Wc, _Ptr1, count, &_Mbst1, &_Cvt)) <= 0)
                break;

        ++_Wchars;
        wchar_t* _Ptrdest = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 655) wchar_t[_Wchars];
        wchar_t* _Ptrnext = _Ptrdest;
        mbstate_t _Mbst2 = {0};
#pragma warning(push)
#pragma warning(disable: 6011)

        for (; 0 < _Wchars;
                count -= _Bytes, Ptr += _Bytes, --_Wchars, ++_Ptrnext)
            if ((_Bytes = Mbrtowc(_Ptrnext, Ptr, _Count1, &_Mbst2, &_Cvt)) <= 0)
                break;

        *_Ptrnext = L'\0';
#pragma warning(pop)
        return (_Ptrdest);
    }
#pragma warning(push)
#pragma warning(disable:4275)

    class __declspec(dllimport) codecvt_base
            : public locale::facet {
    public:
        enum {
            ok, partial, error, noconv
        };
        typedef int result;

        codecvt_base(size_t refs_ = 0)
            : locale::facet(refs_) {
        }

        bool  always_noconv() const throw() {
            return (do_always_noconv());
        }

        int  max_length() const throw() {
            return (do_max_length());
        }

        int  encoding() const throw() {
            return (do_encoding());
        }

        ~codecvt_base() {
        }

    protected:
        virtual bool  do_always_noconv() const throw() {
            return (true);
        }

        virtual int  do_max_length() const throw() {
            return (1);
        }

        virtual int  do_encoding() const throw() {
            return (1);
        }
    };
#pragma warning(pop)
    template < class TElement,
             class byte,
             class _Statype >
    class codecvt
            : public codecvt_base {
    public:
        typedef TElement internTpe;
        typedef byte externTpe;
        typedef _Statype stateTpe;

        result  in(_Statype& _State,
                   const byte* first1, const byte* Last1, const byte *& _Mid1,
                   TElement* first2, TElement* Last2, TElement *& _Mid2) const {
            return (do_in(_State,
                          first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  out(_Statype& _State,
                    const TElement* first1, const TElement* Last1, const TElement *& _Mid1,
                    byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_out(_State,
                           first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  unshift(_Statype& _State,
                        byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_unshift(_State, first2, Last2, _Mid2));
        }

        int  length(const _Statype& _State, const byte* first1,
                    const byte* Last1, size_t count) const {
            return (do_length(_State, first1, Last1, count));
        }

        static locale::id id;

        explicit  codecvt(size_t refs_ = 0)
            : codecvt_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        codecvt(const LocInfo& _Lobj, size_t refs_ = 0)
            : codecvt_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 785) codecvt<TElement, byte, _Statype>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~codecvt() {
        }

    protected:
        void  _Init(const LocInfo&) {
        }

        virtual result  do_in(_Statype&,
                              const byte* first1, const byte*, const byte *& _Mid1,
                              TElement* first2, TElement*, TElement *& _Mid2) const {
            _Mid1 = first1, _Mid2 = first2;
            return (noconv);
        }

        virtual result  do_out(_Statype&,
                               const TElement* first1, const TElement*, const TElement *& _Mid1,
                               byte* first2, byte*, byte *& _Mid2) const {
            _Mid1 = first1, _Mid2 = first2;
            return (noconv);
        }

        virtual result  do_unshift(_Statype&,
                                   byte* first2, byte*, byte *&_Mid2) const {
            _Mid2 = first2;
            return (noconv);
        }

        virtual int  do_length(const _Statype&, const byte* first1,
                               const byte* Last1, size_t count) const {
            return ((int)(count < (size_t)(Last1 - first1)
                          ? count : Last1 - first1));
        }
    };
    template < class TElement,
             class byte,
             class _Statype >
    locale::id codecvt<TElement, byte, _Statype>::id;
    template<> class __declspec(dllimport) codecvt<wchar_t, char, _Mbstatet>
            : public codecvt_base {
    public:
        typedef wchar_t TElement;
        typedef char byte;
        typedef _Mbstatet _Statype;
        typedef TElement internTpe;
        typedef byte externTpe;
        typedef _Statype stateTpe;

        result  in(_Statype& _State,
                   const byte* first1, const byte* Last1, const byte *& _Mid1,
                   TElement* first2, TElement* Last2, TElement *& _Mid2) const {
            return (do_in(_State,
                          first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  out(_Statype& _State,
                    const TElement* first1, const TElement* Last1, const TElement *& _Mid1,
                    byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_out(_State,
                           first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  unshift(_Statype& _State,
                        byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_unshift(_State,
                               first2, Last2, _Mid2));
        }

        int  length(const _Statype& _State, const byte* first1,
                    const byte* Last1, size_t count) const {
            return (do_length(_State, first1, Last1, count));
        }

        static  locale::id& __cdecl _Id_func();

        static locale::id id;
        explicit  codecvt(size_t refs_ = 0)
            : codecvt_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        codecvt(const LocInfo& _Lobj, size_t refs_ = 0)
            : codecvt_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 903) codecvt<TElement, byte, _Statype>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~codecvt() {
        }

    protected:
        void  _Init(const LocInfo& _Lobj) {
            _Cvt = _Lobj.Getcvt();
        }

        virtual result  do_in(_Statype& _State,
                              const byte* first1, const byte* Last1, const byte *& _Mid1,
                              TElement* first2, TElement* Last2, TElement *& _Mid2) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 923);
            Debug_range(first2, Last2, L"..\\include\\xlocale", 924);
            _Mid1 = first1, _Mid2 = first2;
            result Ans = _Mid1 == Last1 ? ok : partial;
            int _Bytes;

            while (_Mid1 != Last1 && _Mid2 != Last2)
                switch (_Bytes = Mbrtowc(_Mid2, _Mid1, Last1 - _Mid1,
                                         &_State, &_Cvt)) {
                    case -2:
                        _Mid1 = Last1;
                        return (Ans);
                    case -1:
                        return (error);
                    case 0:

                        if (*_Mid2 == (TElement)0)
                            _Bytes = (int)::strlen(_Mid1) + 1;

                    default:

                        if (_Bytes == -3)
                            _Bytes = 0;

                        _Mid1 += _Bytes;
                        ++_Mid2;
                        Ans = ok;
                }

            return (Ans);
        }

        virtual result  do_out(_Statype& _State,
                               const TElement* first1, const TElement* Last1, const TElement *& _Mid1,
                               byte* first2, byte* Last2, byte *& _Mid2) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 959);
            Debug_range(first2, Last2, L"..\\include\\xlocale", 960);
            _Mid1 = first1, _Mid2 = first2;
            result Ans = _Mid1 == Last1 ? ok : partial;
            int _Bytes;

            while (_Mid1 != Last1 && _Mid2 != Last2)
                if ((int)___mb_cur_max_func() <= Last2 - _Mid2)
                    if ((_Bytes = _Wcrtomb(_Mid2, *_Mid1,
                                           &_State, &_Cvt)) < 0)
                        return (error);
                    else
                        ++_Mid1, _Mid2 += _Bytes, Ans = ok;
                else {
                    byte Buffer[5];
                    _Statype _Stsave = _State;

                    if ((_Bytes = _Wcrtomb(Buffer, *_Mid1,
                                           &_State, &_Cvt)) < 0)
                        return (error);
                    else if (Last2 - _Mid2 < _Bytes) {
                        _State = _Stsave;
                        return (Ans);
                    } else {
                        ::memcpy_s((_Mid2), (Last2 - _Mid2), (Buffer), (_Bytes));
                        ++_Mid1, _Mid2 += _Bytes, Ans = ok;
                    }
                }

            return (Ans);
        }

        virtual result  do_unshift(_Statype& _State,
                                   byte* first2, byte* Last2, byte *& _Mid2) const {
            Debug_range(first2, Last2, L"..\\include\\xlocale", 997);
            _Mid2 = first2;
            result Ans = ok;
            int _Bytes;
            byte Buffer[5];
            _Statype _Stsave = _State;

            if ((_Bytes = _Wcrtomb(Buffer, L'\0', &_State, &_Cvt)) <= 0)
                Ans = error;
            else if (Last2 - _Mid2 < --_Bytes) {
                _State = _Stsave;
                Ans = partial;
            } else if (0 < _Bytes) {
                ::memcpy_s((_Mid2), (Last2 - _Mid2), (Buffer), (_Bytes));
                _Mid2 += _Bytes;
            }

            return (Ans);
        }

        virtual int  do_length(const _Statype& _State, const byte* first1,
                               const byte* Last1, size_t count) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 1028);
            int _Wchars;
            const byte* _Mid1;
            _Statype _Mystate = _State;

            for (_Wchars = 0, _Mid1 = first1;
                    (size_t)_Wchars < count && _Mid1 != Last1;) {
                int _Bytes;
                TElement chr;

                switch (_Bytes = Mbrtowc(&chr, _Mid1, Last1 - _Mid1,
                                         &_Mystate, &_Cvt)) {
                    case -2:
                        return (_Wchars);
                    case -1:
                        return (_Wchars);
                    case 0:

                        if (chr == (TElement)0)
                            _Bytes = (int)::strlen(_Mid1) + 1;

                    default:

                        if (_Bytes == -3)
                            _Bytes = 0;

                        _Mid1 += _Bytes;
                        ++_Wchars;
                }
            }

            return (_Wchars);
        }

        virtual bool  do_always_noconv() const throw() {
            return (false);
        }

        virtual int  do_max_length() const throw() {
            return (5);
        }

    private:
        LocInfo::Cvtvec _Cvt;
    };

    template<> class __declspec(dllimport) codecvt<unsigned short, char, _Mbstatet>
            : public codecvt_base {
    public:
        typedef unsigned short TElement;
        typedef char byte;
        typedef _Mbstatet _Statype;
        typedef TElement internTpe;
        typedef byte externTpe;
        typedef _Statype stateTpe;

        result  in(_Statype& _State,
                   const byte* first1, const byte* Last1, const byte *& _Mid1,
                   TElement* first2, TElement* Last2, TElement *& _Mid2) const {
            return (do_in(_State,
                          first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  out(_Statype& _State,
                    const TElement* first1, const TElement* Last1, const TElement *& _Mid1,
                    byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_out(_State,
                           first1, Last1, _Mid1, first2, Last2, _Mid2));
        }

        result  unshift(_Statype& _State,
                        byte* first2, byte* Last2, byte *& _Mid2) const {
            return (do_unshift(_State,
                               first2, Last2, _Mid2));
        }

        int  length(const _Statype& _State, const byte* first1,
                    const byte* Last1, size_t count) const {
            return (do_length(_State, first1, Last1, count));
        }

        static  locale::id& __cdecl _Id_func();

        static locale::id id;
        explicit  codecvt(size_t refs_ = 0)
            : codecvt_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        codecvt(const LocInfo& _Lobj, size_t refs_ = 0)
            : codecvt_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 1145) codecvt<TElement, byte, _Statype>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~codecvt() {
        }

    protected:
        codecvt(const char* _Locname, size_t refs_ = 0)
            : codecvt_base(refs_) {
            {
                LocInfo _Lobj(_Locname);
                _Init(_Lobj);
            }
        }

        void  _Init(const LocInfo& _Lobj) {
            _Cvt = _Lobj.Getcvt();
        }

        virtual result  do_in(_Statype& _State,
                              const byte* first1, const byte* Last1, const byte *& _Mid1,
                              TElement* first2, TElement* Last2, TElement *& _Mid2) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 1173);
            Debug_range(first2, Last2, L"..\\include\\xlocale", 1174);
            _Mid1 = first1, _Mid2 = first2;
            result Ans = _Mid1 == Last1 ? ok : partial;
            int _Bytes;

            while (_Mid1 != Last1 && _Mid2 != Last2)
                switch (_Bytes = Mbrtowc((wchar_t*)_Mid2, _Mid1, Last1 - _Mid1,
                                         &_State, &_Cvt)) {
                    case -2:
                        _Mid1 = Last1;
                        return (Ans);
                    case -1:
                        return (error);
                    case 0:

                        if (*_Mid2 == (TElement)0)
                            _Bytes = (int)::strlen(_Mid1) + 1;

                    default:

                        if (_Bytes == -3)
                            _Bytes = 0;

                        _Mid1 += _Bytes;
                        ++_Mid2;
                        Ans = ok;
                }

            return (Ans);
        }

        virtual result  do_out(_Statype& _State,
                               const TElement* first1, const TElement* Last1, const TElement *& _Mid1,
                               byte* first2, byte* Last2, byte *& _Mid2) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 1209);
            Debug_range(first2, Last2, L"..\\include\\xlocale", 1210);
            _Mid1 = first1, _Mid2 = first2;
            result Ans = _Mid1 == Last1 ? ok : partial;
            int _Bytes;

            while (_Mid1 != Last1 && _Mid2 != Last2)
                if (5 <= Last2 - _Mid2)
                    if ((_Bytes = _Wcrtomb(_Mid2, *_Mid1,
                                           &_State, &_Cvt)) < 0)
                        return (error);
                    else
                        ++_Mid1, _Mid2 += _Bytes, Ans = ok;
                else {
                    byte Buffer[5];
                    _Statype _Stsave = _State;

                    if ((_Bytes = _Wcrtomb(Buffer, *_Mid1,
                                           &_State, &_Cvt)) < 0)
                        return (error);
                    else if (Last2 - _Mid2 < _Bytes) {
                        _State = _Stsave;
                        return (Ans);
                    } else {
                        ::memcpy_s((_Mid2), (Last2 - _Mid2), (Buffer), (_Bytes));
                        ++_Mid1, _Mid2 += _Bytes, Ans = ok;
                    }
                }

            return (Ans);
        }

        virtual result  do_unshift(_Statype& _State,
                                   byte* first2, byte* Last2, byte *& _Mid2) const {
            Debug_range(first2, Last2, L"..\\include\\xlocale", 1247);
            _Mid2 = first2;
            result Ans = ok;
            int _Bytes;
            byte Buffer[5];
            _Statype _Stsave = _State;

            if ((_Bytes = _Wcrtomb(Buffer, L'\0', &_State, &_Cvt)) <= 0)
                Ans = error;
            else if (Last2 - _Mid2 < --_Bytes) {
                _State = _Stsave;
                Ans = partial;
            } else if (0 < _Bytes) {
                ::memcpy_s((_Mid2), (Last2 - _Mid2), (Buffer), (_Bytes));
                _Mid2 += _Bytes;
            }

            return (Ans);
        }

        virtual int  do_length(const _Statype& _State, const byte* first1,
                               const byte* Last1, size_t count) const {
            Debug_range(first1, Last1, L"..\\include\\xlocale", 1272);
            int _Wchars;
            const byte* _Mid1;
            _Statype _Mystate = _State;

            for (_Wchars = 0, _Mid1 = first1;
                    (size_t)_Wchars < count && _Mid1 != Last1;) {
                int _Bytes;
                TElement chr;

                switch (_Bytes = Mbrtowc((wchar_t*)&chr, _Mid1, Last1 - _Mid1,
                                         &_Mystate, &_Cvt)) {
                    case -2:
                        return (_Wchars);
                    case -1:
                        return (_Wchars);
                    case 0:

                        if (chr == (TElement)0)
                            _Bytes = (int)::strlen(_Mid1) + 1;

                    default:

                        if (_Bytes == -3)
                            _Bytes = 0;

                        _Mid1 += _Bytes;
                        ++_Wchars;
                }
            }

            return (_Wchars);
        }

        virtual bool  do_always_noconv() const throw() {
            return (false);
        }

        virtual int  do_max_length() const throw() {
            return (5);
        }

    private:
        LocInfo::Cvtvec _Cvt;
    };

    template < class TElement,
             class byte,
             class _Statype >
    class codecvt_byname
            : public codecvt<TElement, byte, _Statype> {
    public:
        explicit  codecvt_byname(const char* _Locname, size_t refs_ = 0)
            : codecvt<TElement, byte, _Statype>(_Locname, refs_) {
        }

    protected:
        virtual  ~codecvt_byname() {
        }
    };

#pragma warning(push)
#pragma warning(disable:4275)

    struct __declspec(dllimport) ctype_base
: public locale::facet {
    enum {
        alnum = 0x4 | 0x2 | 0x1 | 0x100, alpha = 0x2 | 0x1 | 0x100,
        cntrl = 0x20, digit = 0x4, graph = 0x4 | 0x2 | 0x10 | 0x1 | 0x100,
        lower = 0x2, print = 0x4 | 0x2 | 0x10 | 0x40 | 0x1 | 0x100 | 0x80,
        punct = 0x10, space = 0x8 | 0x40 | 0x000, upper = 0x1,
        xdigit = 0x80
    };
    typedef short mask;

    ctype_base(size_t refs_ = 0)
        : locale::facet(refs_) {
    }

    ~ctype_base() {
    }

protected:
    static void __cdecl Xran() {
        throw out_of_range("out_of_range in ctype<T1>");
    }
    };
#pragma warning(pop)
    template<class TElement>
    class ctype
            : public ctype_base {
    public:
        typedef TElement char_type;

        bool  is(mask _Maskval, TElement chr) const {
            return (do_is(_Maskval, chr));
        }

        const TElement* is(const TElement* first, const TElement* Last,
                           mask* Dest) const {
            return (do_is(first, Last, Dest));
        }

        const TElement* scan_is(mask _Maskval, const TElement* first,
                                const TElement* Last) const {
            return (do_scan_is(_Maskval, first, Last));
        }

        const TElement* scan_not(mask _Maskval, const TElement* first,
                                 const TElement* Last) const {
            return (do_scan_not(_Maskval, first, Last));
        }

        TElement  tolower(TElement chr) const {
            return (do_tolower(chr));
        }

        const TElement* tolower(TElement* first, const TElement* Last) const {
            return (do_tolower(first, Last));
        }

        TElement  toupper(TElement chr) const {
            return (do_toupper(chr));
        }

        const TElement* toupper(TElement* first, const TElement* Last) const {
            return (do_toupper(first, Last));
        }

        TElement  widen(char byte) const {
            return (do_widen(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const char* widen(const char* first, const char* Last,
                          TElement* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_widen(first, Last, Dest));
#pragma warning(pop)
        }

        const char* _Widen_s(const char* first, const char* Last,
                             TElement* Dest, size_t destSize) const {
            return (Do_widen_s(first, Last, Dest, destSize));
        }

        char  narrow(TElement chr, char Dflt = '\0') const {
            return (do_narrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const TElement* narrow(const TElement* first, const TElement* Last,
                               char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_narrow(first, Last, Dflt, Dest));
#pragma warning(pop)
        }

        const TElement* _Narrow_s(const TElement* first, const TElement* Last,
                                  char Dflt, [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest, size_t destSize) const {
            return (Do_narrow_s(first, Last, Dflt, Dest, destSize));
        }

        static locale::id id;

        explicit  ctype(size_t refs_ = 0)
            : ctype_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        ctype(const LocInfo& _Lobj, size_t refs_ = 0)
            : ctype_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 1487) ctype<TElement>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~ctype() {
            if (ctype_.Delfl)
                free((void*)ctype_.Table);
        }

    protected:
        void  _Init(const LocInfo& _Lobj) {
            ctype_ = _Lobj.Getctype();
        }

        virtual bool  do_is(mask _Maskval, TElement chr) const {
            return ((ctype_.Table[(unsigned char)narrow(chr)]
                     & _Maskval) != 0);
        }

        virtual const TElement* do_is(const TElement* first, const TElement* Last,
                                      mask* Dest) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1514);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1515);

            for (; first != Last; ++first, ++Dest)
                *Dest = ctype_.Table[(unsigned char)narrow(*first)];

            return (first);
        }

        virtual const TElement* do_scan_is(mask _Maskval,
                                           const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1524);

            for (; first != Last && !is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual const TElement* do_scan_not(mask _Maskval,
                                            const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1533);

            for (; first != Last && is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual TElement  do_tolower(TElement chr) const {
            unsigned char byte = (unsigned char)narrow(chr, '\0');

            if (byte == '\0')
                return (chr);
            else
                return (widen((char)_Tolower(byte, &ctype_)));
        }

        virtual const TElement* do_tolower(TElement* first, const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 1550);

            for (; first != Last; ++first) {
                unsigned char byte = (unsigned char)narrow(*first, '\0');

                if (byte != '\0')
                    *first = (widen((char)_Tolower(byte, &ctype_)));
            }

            return ((const TElement*)first);
        }

        virtual TElement  do_toupper(TElement chr) const {
            unsigned char byte = (unsigned char)narrow(chr, '\0');

            if (byte == '\0')
                return (chr);
            else
                return (widen((char)_Toupper(byte, &ctype_)));
        }

        virtual const TElement* do_toupper(TElement* first, const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 1571);

            for (; first != Last; ++first) {
                unsigned char byte = (unsigned char)narrow(*first, '\0');

                if (byte != '\0')
                    *first = (widen((char)_Toupper(byte, &ctype_)));
            }

            return ((const TElement*)first);
        }

        virtual TElement  do_widen(char byte) const {
            return (_Maklocchr(byte, (TElement*)0, _Cvt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const char* do_widen(const char* first,
                                     const char* Last, TElement* Dest) const {
            return Do_widen_s(first, Last, Dest, Last - first);
        }

        virtual const char* Do_widen_s(const char* first,
                                       const char* Last, TElement* Dest, size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1597);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1598);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 1599, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 1599, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Maklocchr(*first, (TElement*)0, _Cvt);

            return (first);
        }

        char  _Donarrow(TElement chr, char Dflt) const {
            char byte;

            if (chr == (TElement)0)
                return ('\0');
            else if ((byte = _Maklocbyte((TElement)chr, _Cvt)) == '\0')
                return (Dflt);
            else
                return (byte);
        }

        virtual char  do_narrow(TElement chr, char Dflt) const {
            return (_Donarrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const TElement* do_narrow(const TElement* first,
                                          const TElement* Last, char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
            return Do_narrow_s(first, Last, Dflt, Dest, Last - first);
        }

        virtual const TElement* Do_narrow_s(const TElement* first,
                                            const TElement* Last, char Dflt,
                                            [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                            size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1634);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1635);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 1636, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 1636, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Donarrow(*first, Dflt);

            return (first);
        }

    private:
        LocInfo::Ctypevec ctype_;
        LocInfo::Cvtvec _Cvt;
    };
    template<class TElement>
    locale::id ctype<TElement>::id;
    template<> class __declspec(dllimport) ctype<char>
            : public ctype_base {
        typedef ctype<char> MyType;

    public:
        typedef char TElement;
        typedef TElement char_type;

        bool  is(mask _Maskval, TElement chr) const {
            return ((ctype_.Table[(unsigned char)chr] & _Maskval) != 0);
        }

        const TElement* is(const TElement* first,
                           const TElement* Last, mask* Dest) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1669);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1670);

            for (; first != Last; ++first, ++Dest)
                *Dest = ctype_.Table[(unsigned char)*first];

            return (first);
        }

        const TElement* scan_is(mask _Maskval,
                                const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1679);

            for (; first != Last && !is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        const TElement* scan_not(mask _Maskval,
                                 const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1688);

            for (; first != Last && is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        TElement  tolower(TElement chr) const {
            return (do_tolower(chr));
        }

        const TElement* tolower(TElement* first, const TElement* Last) const {
            return (do_tolower(first, Last));
        }

        TElement  toupper(TElement chr) const {
            return (do_toupper(chr));
        }

        const TElement* toupper(TElement* first, const TElement* Last) const {
            return (do_toupper(first, Last));
        }

        TElement  widen(char byte) const {
            return (do_widen(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const TElement* widen(const char* first, const char* Last,
                              TElement* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_widen(first, Last, Dest));
#pragma warning(pop)
        }

        const TElement* _Widen_s(const char* first, const char* Last,
                                 TElement* Dest, size_t destSize) const {
            return (Do_widen_s(first, Last, Dest, destSize));
        }

        TElement  narrow(TElement chr, char Dflt = '\0') const {
            return (do_narrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const TElement* narrow(const TElement* first, const TElement* Last,
                               char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_narrow(first, Last, Dflt, Dest));
#pragma warning(pop)
        }

        const TElement* _Narrow_s(const TElement* first, const TElement* Last,
                                  char Dflt, [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                  size_t destSize) const {
            return (Do_narrow_s(first, Last, Dflt, Dest, destSize));
        }

        static  locale::id& __cdecl _Id_func();

        static locale::id id;
        explicit  ctype(const mask* Table = 0,
                        bool _Deletetable = false,
                        size_t refs_ = 0)
            : ctype_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }

            if (Table != 0) {
                Tidy_();
                ctype_.Table = Table;
                ctype_.Delfl = _Deletetable ? -1 : 0;
            }
        }

        ctype(const LocInfo& _Lobj, size_t refs_ = 0)
            : ctype_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 1792) ctype<TElement>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

        static const size_t table_size = 1 << 8;

    protected:
        virtual  ~ctype() {
            Tidy_();
        }

    protected:
        void  _Init(const LocInfo& _Lobj) {
            ctype_ = _Lobj.Getctype();
        }

        void  Tidy_() {
            if (0 < ctype_.Delfl)
                free((void*)ctype_.Table);
            else if (ctype_.Delfl < 0)
                delete[](void*)ctype_.Table;
        }

        virtual TElement  do_tolower(TElement chr) const {
            return ((TElement)_Tolower((unsigned char)chr, &ctype_));
        }

        virtual const TElement* do_tolower(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 1827);

            for (; first != Last; ++first)
                *first = (TElement)_Tolower((unsigned char) * first, &ctype_);

            return ((const TElement*)first);
        }

        virtual TElement  do_toupper(TElement chr) const {
            return ((TElement)_Toupper((unsigned char)chr, &ctype_));
        }

        virtual const TElement* do_toupper(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 1841);

            for (; first != Last; ++first)
                *first = (TElement)_Toupper((unsigned char) * first, &ctype_);

            return ((const TElement*)first);
        }

        virtual TElement  do_widen(char byte) const {
            return (byte);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const TElement* do_widen(const char* first,
                                         const char* Last, TElement* Dest) const {
            return Do_widen_s(first, Last, Dest, Last - first);
        }

        virtual const TElement* Do_widen_s(const char* first,
                                           const char* Last, TElement* Dest, size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1863);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1864);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 1865, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 1865, 0);
                };
            };
            ::memcpy_s((Dest), (destSize), (first), (Last - first));
            return (Last);
        }

        virtual TElement  do_narrow(TElement chr, char) const {
            return (chr);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const TElement* do_narrow(const TElement* first,
                                          const TElement* Last, char Dflt,
                                          [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
            return Do_narrow_s(first, Last, Dflt, Dest, Last - first);
        }

        virtual const TElement* Do_narrow_s(const TElement* first,
                                            const TElement* Last, char, [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                            size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 1888);
            Debug_pointer(Dest, L"..\\include\\xlocale", 1889);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 1890, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 1890, 0);
                };
            };
            ::memcpy_s((Dest), (destSize), (first), (Last - first));
            return (Last);
        }

        const mask* table() const throw() {
            return (ctype_.Table);
        }

        static const mask* __cdecl classic_table() throw() {
            const MyType& ctype_facet = use_facet<MyType >(locale::classic());
            return (ctype_facet.table());
        }

    private:
        LocInfo::Ctypevec ctype_;
    };
    template<> class __declspec(dllimport) ctype<wchar_t>
            : public ctype_base {
        typedef ctype<wchar_t> MyType;

    public:
        typedef wchar_t TElement;
        typedef TElement char_type;

        bool  is(mask _Maskval, TElement chr) const {
            return (do_is(_Maskval, chr));
        }

        const TElement* is(const TElement* first, const TElement* Last,
                           mask* Dest) const {
            return (do_is(first, Last, Dest));
        }

        const TElement* scan_is(mask _Maskval, const TElement* first,
                                const TElement* Last) const {
            return (do_scan_is(_Maskval, first, Last));
        }

        const TElement* scan_not(mask _Maskval, const TElement* first,
                                 const TElement* Last) const {
            return (do_scan_not(_Maskval, first, Last));
        }

        TElement  tolower(TElement chr) const {
            return (do_tolower(chr));
        }

        const TElement* tolower(TElement* first, const TElement* Last) const {
            return (do_tolower(first, Last));
        }

        TElement  toupper(TElement chr) const {
            return (do_toupper(chr));
        }

        const TElement* toupper(TElement* first, const TElement* Last) const {
            return (do_toupper(first, Last));
        }

        TElement  widen(char byte) const {
            return (do_widen(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const char* widen(const char* first, const char* Last,
                          TElement* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_widen(first, Last, Dest));
#pragma warning(pop)
        }

        const char* _Widen_s(const char* first, const char* Last,
                             TElement* Dest, size_t destSize) const {
            return (Do_widen_s(first, Last, Dest, destSize));
        }

        char  narrow(TElement chr, char Dflt = '\0') const {
            return (do_narrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const TElement* narrow(const TElement* first, const TElement* Last,
                               char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_narrow(first, Last, Dflt, Dest));
#pragma warning(pop)
        }

        const TElement* _Narrow_s(const TElement* first, const TElement* Last,
                                  char Dflt, [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                  size_t destSize) const {
            return (Do_narrow_s(first, Last, Dflt, Dest, destSize));
        }

        static  locale::id& __cdecl _Id_func();

        static locale::id id;
        explicit  ctype(size_t refs_ = 0)
            : ctype_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        ctype(const LocInfo& _Lobj, size_t refs_ = 0)
            : ctype_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 2033) ctype<TElement>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~ctype() {
            if (ctype_.Delfl)
                free((void*)ctype_.Table);
        }

    protected:
        void  _Init(const LocInfo& _Lobj) {
            ctype_ = _Lobj.Getctype();
            _Cvt = _Lobj.Getcvt();
        }

        virtual bool  do_is(mask _Maskval, TElement chr) const {
            return ((::_Getwctype(chr, &ctype_) & _Maskval) != 0);
        }

        virtual const wchar_t* do_is(const TElement* first,
                                     const TElement* Last, mask* Dest) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2060);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2061);
            return (::_Getwctypes(first, Last, Dest, &ctype_));
        }

        virtual const TElement* do_scan_is(mask _Maskval,
                                           const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2068);

            for (; first != Last && !is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual const TElement* do_scan_not(mask _Maskval,
                                            const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2077);

            for (; first != Last && is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual TElement  do_tolower(TElement chr) const {
            return (_Towlower(chr, &ctype_));
        }

        virtual const TElement* do_tolower(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 2091);

            for (; first != Last; ++first)
                *first = _Towlower(*first, &ctype_);

            return ((const TElement*)first);
        }

        virtual TElement  do_toupper(TElement chr) const {
            return (_Towupper(chr, &ctype_));
        }

        virtual const TElement* do_toupper(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 2105);

            for (; first != Last; ++first)
                *first = _Towupper(*first, &ctype_);

            return ((const TElement*)first);
        }

        TElement  _Dowiden(char byte) const {
            mbstate_t _Mbst = {0};
            wchar_t _Wc;
            return (Mbrtowc(&_Wc, &byte, 1, &_Mbst, &_Cvt) < 0
                    ? (wchar_t)(wint_t)(0xFFFF) : _Wc);
        }

        virtual TElement  do_widen(char byte) const {
            return (_Dowiden(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const char* do_widen(const char* first,
                                     const char* Last, TElement* Dest) const {
            return Do_widen_s(first, Last, Dest, Last - first);
        }

        virtual const char* Do_widen_s(const char* first,
                                       const char* Last, TElement* Dest, size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2135);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2136);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 2137, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 2137, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Dowiden(*first);

            return (first);
        }

        char  _Donarrow(TElement chr, char Dflt) const {
            char Buffer[5];
            mbstate_t _Mbst = {0};
            return (_Wcrtomb(Buffer, chr, &_Mbst, &_Cvt) != 1
                    ? Dflt : Buffer[0]);
        }

        virtual char  do_narrow(TElement chr, char Dflt) const {
            return (_Donarrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const TElement* do_narrow(const TElement* first,
                                          const TElement* Last, char Dflt,
                                          [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
            return Do_narrow_s(first, Last, Dflt, Dest, Last - first);
        }

        virtual const TElement* Do_narrow_s(const TElement* first,
                                            const TElement* Last, char Dflt,
                                            [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                            size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2170);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2171);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 2172, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 2172, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Donarrow(*first, Dflt);

            return (first);
        }

    private:
        LocInfo::Ctypevec ctype_;
        LocInfo::Cvtvec _Cvt;
    };

    template<> class __declspec(dllimport) ctype<unsigned short>
            : public ctype_base {
        typedef ctype<unsigned short> MyType;

    public:
        typedef unsigned short TElement;
        typedef TElement char_type;

        bool  is(mask _Maskval, TElement chr) const {
            return (do_is(_Maskval, chr));
        }

        const TElement* is(const TElement* first, const TElement* Last,
                           mask* Dest) const {
            return (do_is(first, Last, Dest));
        }

        const TElement* scan_is(mask _Maskval, const TElement* first,
                                const TElement* Last) const {
            return (do_scan_is(_Maskval, first, Last));
        }

        const TElement* scan_not(mask _Maskval, const TElement* first,
                                 const TElement* Last) const {
            return (do_scan_not(_Maskval, first, Last));
        }

        TElement  tolower(TElement chr) const {
            return (do_tolower(chr));
        }

        const TElement* tolower(TElement* first, const TElement* Last) const {
            return (do_tolower(first, Last));
        }

        TElement  toupper(TElement chr) const {
            return (do_toupper(chr));
        }

        const TElement* toupper(TElement* first, const TElement* Last) const {
            return (do_toupper(first, Last));
        }

        TElement  widen(char byte) const {
            return (do_widen(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const char* widen(const char* first, const char* Last,
                          TElement* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_widen(first, Last, Dest));
#pragma warning(pop)
        }

        const char* _Widen_s(const char* first, const char* Last,
                             TElement* Dest, size_t destSize) const {
            return (Do_widen_s(first, Last, Dest, destSize));
        }

        char  narrow(TElement chr, char Dflt = '\0') const {
            return (do_narrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        const TElement* narrow(const TElement* first, const TElement* Last,
                               char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
#pragma warning(push)
#pragma warning(disable:4996)
            return (do_narrow(first, Last, Dflt, Dest));
#pragma warning(pop)
        }

        const TElement* _Narrow_s(const TElement* first, const TElement* Last,
                                  char Dflt, [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                  size_t destSize) const {
            return (Do_narrow_s(first, Last, Dflt, Dest, destSize));
        }

        static  locale::id& __cdecl _Id_func();

        static locale::id id;
        explicit  ctype(size_t refs_ = 0)
            : ctype_base(refs_) {
            {
                LocInfo _Lobj;
                _Init(_Lobj);
            }
        }

        ctype(const LocInfo& _Lobj, size_t refs_ = 0)
            : ctype_base(refs_) {
            _Init(_Lobj);
        }

        static size_t __cdecl GetCat(const locale::facet** _Ppf = 0,
                                     const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::DebugHeapTag_func(), "..\\include\\xlocale", 2307) ctype<TElement>(
                    LocInfo(_Ploc->name()));

            return (2);
        }

    protected:
        virtual  ~ctype() {
            if (ctype_.Delfl)
                free((void*)ctype_.Table);
        }

    protected:
        ctype(const char* _Locname, size_t refs_ = 0)
            : ctype_base(refs_) {
            {
                LocInfo _Lobj(_Locname);
                _Init(_Lobj);
            }
        }

        void  _Init(const LocInfo& _Lobj) {
            ctype_ = _Lobj.Getctype();
            _Cvt = _Lobj.Getcvt();
        }

        virtual bool  do_is(mask _Maskval, TElement chr) const {
            return ((::_Getwctype(chr, &ctype_) & _Maskval) != 0);
        }

        virtual const TElement* do_is(const TElement* first,
                                      const TElement* Last, mask* Dest) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2342);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2343);
            return ((const TElement*)::_Getwctypes((const wchar_t*)first,
                                                   (const wchar_t*)Last, Dest, &ctype_));
        }

        virtual const TElement* do_scan_is(mask _Maskval,
                                           const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2351);

            for (; first != Last && !is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual const TElement* do_scan_not(mask _Maskval,
                                            const TElement* first, const TElement* Last) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2360);

            for (; first != Last && is(_Maskval, *first); ++first)
                ;

            return (first);
        }

        virtual TElement  do_tolower(TElement chr) const {
            return (_Towlower(chr, &ctype_));
        }

        virtual const TElement* do_tolower(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 2374);

            for (; first != Last; ++first)
                *first = _Towlower(*first, &ctype_);

            return ((const TElement*)first);
        }

        virtual TElement  do_toupper(TElement chr) const {
            return (_Towupper(chr, &ctype_));
        }

        virtual const TElement* do_toupper(TElement* first,
                                           const TElement* Last) const {
            Debug_range((const TElement*)first, Last, L"..\\include\\xlocale", 2388);

            for (; first != Last; ++first)
                *first = _Towupper(*first, &ctype_);

            return ((const TElement*)first);
        }

        TElement  _Dowiden(char byte) const {
            mbstate_t _Mbst = {0};
            unsigned short _Wc;
            return (Mbrtowc((wchar_t*)&_Wc, &byte, 1, &_Mbst, &_Cvt) < 0
                    ? (unsigned short)(wint_t)(0xFFFF) : _Wc);
        }

        virtual TElement  do_widen(char byte) const {
            return (_Dowiden(byte));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const char* do_widen(const char* first,
                                     const char* Last, TElement* Dest) const {
            return Do_widen_s(first, Last, Dest, Last - first);
        }

        virtual const char* Do_widen_s(const char* first,
                                       const char* Last, TElement* Dest, size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2418);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2419);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 2420, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 2420, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Dowiden(*first);

            return (first);
        }

        char  _Donarrow(TElement chr, char Dflt) const {
            char Buffer[5];
            mbstate_t _Mbst = {0};
            return (_Wcrtomb(Buffer, chr, &_Mbst, &_Cvt) != 1
                    ? Dflt : Buffer[0]);
        }

        virtual char  do_narrow(TElement chr, char Dflt) const {
            return (_Donarrow(chr, Dflt));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual const TElement* do_narrow(const TElement* first,
                                          const TElement* Last, char Dflt, [SA_Pre(Null=SA_No, WritableElements="\n@""Last-first")] [SA_Pre(Deref=1, Valid=SA_No)] char* Dest) const {
            return Do_narrow_s(first, Last, Dflt, Dest, Last - first);
        }

        virtual const TElement* Do_narrow_s(const TElement* first,
                                            const TElement* Last, char Dflt,
                                            [SA_Pre(Null=SA_No, WritableElements="destSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""Last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* Dest,
                                            size_t destSize) const {
            Debug_range(first, Last, L"..\\include\\xlocale", 2452);
            Debug_pointer(Dest, L"..\\include\\xlocale", 2453);
            {
                if (!(destSize >= (size_t)(Last - first))) {
                    (void)((!!((("destSize >= (size_t)(Last - first)", 0)))) || (1 != _CrtDbgReportW(2, L"..\\include\\xlocale", 2454, 0, L"(\"destSize >= (size_t)(Last - first)\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\include\\xlocale", 2454, 0);
                } ;
            };

            for (; first != Last; ++first, ++Dest)
                *Dest = _Donarrow(*first, Dflt);

            return (first);
        }

    private:
        LocInfo::Ctypevec ctype_;
        LocInfo::Cvtvec _Cvt;
    };

    template<class TElement>
    class ctype_byname
            : public ctype<TElement> {
    public:
        explicit  ctype_byname(const char* _Locname, size_t refs_ = 0)
            : ctype<TElement>(_Locname, refs_) {
        }

    protected:
        virtual  ~ctype_byname() {
        }
    };
    template<> class ctype_byname<char>
            : public ctype<char> {
    public:
        explicit  ctype_byname(const char* _Locname, size_t refs_ = 0)
            : ctype<char>(_Locname, refs_) {
        }

    protected:
        virtual  ~ctype_byname() {
        }
    };

    template class __declspec(dllimport) codecvt<char, char, _Mbstatet>;
}

extern "C" {

    extern __declspec(dllimport) float __cdecl _Stofx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**,
            long, int*);
    extern __declspec(dllimport) double __cdecl _Stodx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**,
            long, int*);
    extern __declspec(dllimport) long double __cdecl _Stoldx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**,
            long, int*);
    extern __declspec(dllimport) long __cdecl _Stolx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**, int,
            int*);
    extern __declspec(dllimport) unsigned long __cdecl _Stoulx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] char**,
            int, int*);

    extern __declspec(dllimport) __int64 __cdecl _Stollx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)]  char**,
            int, int*);
    extern __declspec(dllimport) unsigned __int64 __cdecl _Stoullx(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)]  char**,
            int, int*);

}
