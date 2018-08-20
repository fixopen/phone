extern "C" {
    struct _iobuffer {
        char* _ptr;
        int   _cnt;
        char* _base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char* _tmpfname;
    };
    typedef struct _iobuffer FILE;
    __declspec(dllimport) FILE* __cdecl __iob_func(void);

    typedef __int64 fpos_t;
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _filbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _flsbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _fsopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ShFlag);
    __declspec(dllimport) void __cdecl clearerr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) errno_t __cdecl clearerr_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) int __cdecl fclose([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fcloseall(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _fdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _FileHandle, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl feof([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl ferror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] FILE* _File);
    __declspec(dllimport) int __cdecl fflush([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl fgetc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fgetchar(void);
    __declspec(dllimport) int __cdecl fgetpos([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File , [SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] fpos_t* _Pos);
    __declspec(dllimport) char* __cdecl fgets([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _MaxCount, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _fileno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] FILE* _File);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) char * __cdecl _tempnam([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _DirName, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _FilePrefix);

    __declspec(dllimport) int __cdecl _flushall(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) FILE * __cdecl fopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode);

    __declspec(dllimport) errno_t __cdecl fopen_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] FILE** _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode);

    __declspec(dllimport) int __cdecl fprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl fprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl fputc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fputchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch);
    __declspec(dllimport) int __cdecl fputs([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) size_t __cdecl fread([SA_Pre(Null=SA_No, WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1, Valid=SA_No)] void* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) size_t __cdecl fread_s([SA_Pre(Null=SA_No, WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1, Valid=SA_No)] void* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "freopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) FILE * __cdecl freopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) errno_t __cdecl freopen_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] FILE** _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _OldFile);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl fscanf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _fscanf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl fscanf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl _fscanf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl fsetpos([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] const fpos_t* _Pos);
    __declspec(dllimport) int __cdecl fseek([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long __cdecl ftell([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) int __cdecl _fseeki64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _ftelli64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) size_t __cdecl fwrite([SA_Pre(Null=SA_No, ValidElements="\n@""_Size*_Count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl getc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl getchar(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _getmaxstdio(void);

    __declspec(dllimport) char* __cdecl gets_s([SA_Pre(Null=SA_No, WritableElements="_Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _Size);

    extern "C++" {
        template <size_t _Size> inline char* __cdecl gets_s(char(&_Buffer)[_Size]) {
            return gets_s(_Buffer, _Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "gets_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl gets([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Pre(Null=SA_No, WritableElementsConst=((size_t)-1))] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buffer);
[returnvalue:SA_Post(MustCheck=SA_Yes)] int __cdecl _getw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) void __cdecl perror([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _ErrMsg);

    __declspec(dllimport) int __cdecl _pclose([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _popen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Command, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Mode);
    __declspec(dllimport) int __cdecl printf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl printf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl putc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl putchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch);
    __declspec(dllimport) int __cdecl puts([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Str);
    __declspec(dllimport) int __cdecl _putw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Word, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl remove([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl rename([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _OldFilename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _NewFilename);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _unlink([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_unlink" ". See online help for details.")) __declspec(dllimport) int __cdecl unlink([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Filename);
    __declspec(dllimport) void __cdecl rewind([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _rmtmp(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "scanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl scanf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_scanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _scanf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl scanf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl _scanf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "setvbuf" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) void __cdecl setbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_Maybe, WritableElementsConst=512)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] [SA_Post(ValidElementsConst=0)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buffer);
    __declspec(dllimport) int __cdecl _setmaxstdio([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Max);
    __declspec(dllimport) unsigned int __cdecl _set_output_format([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] unsigned int _Format);
    __declspec(dllimport) unsigned int __cdecl _get_output_format(void);
    __declspec(dllimport) int __cdecl setvbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes, WritableBytes="_Size")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Size);
    __declspec(dllimport) int __cdecl _snprintf_s([SA_Pre(Null=SA_No, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    extern "C++" {
        __pragma(warning(push));
        __pragma(warning(disable: 4793));
        template <size_t _Size> inline int __cdecl _snprintf_s(char(&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...) {
            va_list _ArgList;
            (_ArgList = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
            return _vsnprintf_s(_Dest, _Size, _MaxCount, _Format, _ArgList);
        }
        __pragma(warning(pop));
    }

    __declspec(dllimport) int __cdecl sprintf_s([SA_Pre(Null=SA_No, WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);

    extern "C++" {
        __pragma(warning(push));
        __pragma(warning(disable: 4793));
        template <size_t _Size> inline int __cdecl sprintf_s(char(&_Dest)[_Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...) {
            va_list _ArgList;
            (_ArgList = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
            return vsprintf_s(_Dest, _Size, _Format, _ArgList);
        }
        __pragma(warning(pop));
    }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "sscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl sscanf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_sscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _sscanf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl sscanf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, ...);

    __declspec(dllimport) int __cdecl _sscanf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snscanf([SA_Pre(Null=SA_No, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snscanf_l([SA_Pre(Null=SA_No, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _snscanf_s([SA_Pre(Null=SA_No, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, ...);
    __declspec(dllimport) int __cdecl _snscanf_s_l([SA_Pre(Null=SA_No, ValidBytes="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const char* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "tmpfile_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) FILE * __cdecl tmpfile(void);

    __declspec(dllimport) errno_t __cdecl tmpfile_s([SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No)] [SA_Post(Deref=2, Valid=SA_Yes)] FILE** _File);
    __declspec(dllimport) errno_t __cdecl tmpnam_s([SA_Pre(Null=SA_No, WritableElements="_Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] rsize_t _Size);

    extern "C++" {
        template <size_t _Size> inline errno_t __cdecl tmpnam_s([SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] char(&_Buf)[_Size]) {
            return tmpnam_s(_Buf, _Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "tmpnam_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) char* __cdecl tmpnam([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Buffer);
    __declspec(dllimport) int __cdecl ungetc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl vfprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vfprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vprintf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "vsnprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl vsnprintf([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vsnprintf_s([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _vsnprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
    extern "C++" {
        template <size_t _Size> inline int __cdecl _vsnprintf_s(char(&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _Args) {
            return _vsnprintf_s(_Dest, _Size, _MaxCount, _Format, _Args);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snprintf([SA_Pre(Null=SA_No, WritableElements="_Count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char* _Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _vsnprintf([SA_Pre(Null=SA_No, WritableElements="_Count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char* _Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _Args);

    __declspec(dllimport) int __cdecl vsprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInBytes")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    extern "C++" {
        template <size_t _Size> inline int __cdecl vsprintf_s(char(&_Dest)[_Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _Args) {
            return vsprintf_s(_Dest, _Size, _Format, _Args);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "sprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl sprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "vsprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl vsprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _Args);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _snprintf_c([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(dllimport) int __cdecl _vsnprintf_c([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _fprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(dllimport) int __cdecl _printf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(dllimport) int __cdecl _sprintf_p([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
    __declspec(dllimport) int __cdecl _vfprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsprintf_p([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _set_printf_count_output([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Value);
    __declspec(dllimport) int __cdecl _get_printf_count_output();

    __declspec(dllimport) int __cdecl _printf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _printf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _printf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vprintf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(dllimport) int __cdecl _fprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _fprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _fprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vfprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vfprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vfprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_sprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _sprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _sprintf_p_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _sprintf_s_l([SA_Pre(Null=SA_No, WritableBytes="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _vsprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsprintf_p_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale,  va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(dllimport) int __cdecl _scprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _scprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vscprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vscprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snprintf_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _snprintf_c_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _snprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _vsnprintf_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsnprintf_c_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, const char*, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsnprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _wfsopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _ShFlag);
    __declspec(dllimport) wint_t __cdecl fgetwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wint_t __cdecl _fgetwchar(void);
    __declspec(dllimport) wint_t __cdecl fputwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wint_t __cdecl _fputwchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl getwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wint_t __cdecl getwchar(void);
    __declspec(dllimport) wint_t __cdecl putwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wint_t __cdecl putwchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Ch);
    __declspec(dllimport) wint_t __cdecl ungetwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    __declspec(dllimport) wchar_t* __cdecl fgetws([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl fputws([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wchar_t* __cdecl _getws_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t _Size> inline wchar_t* __cdecl _getws_s(wchar_t (&_String)[_Size]) {
            return _getws_s(_String, _Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_getws_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _getws([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String);
    __declspec(dllimport) int __cdecl _putws([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Str);

    __declspec(dllimport) int __cdecl fwprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl fwprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl wprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl wprintf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scwprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl vfwprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vfwprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vwprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl vwprintf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl swprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);

    extern "C++" {
        __pragma(warning(push));
        __pragma(warning(disable: 4793));
        template <size_t _Size> inline int __cdecl swprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...) {
            va_list _ArgList;
            (_ArgList = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
            return vswprintf_s(_Dest, _Size, _Format, _ArgList);
        }
        __pragma(warning(pop));
    }

    __declspec(dllimport) int __cdecl vswprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    extern "C++" {
        template <size_t _Size> inline int __cdecl vswprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _Args) {
            return vswprintf_s(_Dest, _Size, _Format, _Args);
        }
    }

    __declspec(dllimport) int __cdecl _swprintf_c([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _vswprintf_c([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _snwprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    extern "C++" {
        __pragma(warning(push));
        __pragma(warning(disable: 4793));
        template <size_t _Size> inline int __cdecl _snwprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...) {
            va_list _ArgList;
            (_ArgList = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
            return _vsnwprintf_s(_Dest, _Size, _Count, _Format, _ArgList);
        }
        __pragma(warning(pop));
    }
    __declspec(dllimport) int __cdecl _vsnwprintf_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
    extern "C++" {
        template <size_t _Size> inline int __cdecl _vsnwprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _Args) {
            return _vsnwprintf_s(_Dest, _Size, _Count, _Format, _Args);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snwprintf([SA_Pre(Null=SA_No, WritableElements="_Count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t* _Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _vsnwprintf([SA_Pre(Null=SA_No, WritableElements="_Count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t* _Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _Args);

    __declspec(dllimport) int __cdecl _fwprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _wprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _vfwprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vwprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
    __declspec(dllimport) int __cdecl _swprintf_p([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _vswprintf_p([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scwprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscwprintf_p([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);

    __declspec(dllimport) int __cdecl _wprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _wprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _wprintf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vwprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vwprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vwprintf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(dllimport) int __cdecl _fwprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _fwprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _fwprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vfwprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vfwprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vfwprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(dllimport) int __cdecl _swprintf_c_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _swprintf_p_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _swprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _vswprintf_c_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vswprintf_p_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vswprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scwprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _scwprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscwprintf_p_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snwprintf_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _snwprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnwprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _vsnwprintf_l([SA_Pre(Null=SA_No, WritableElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_No)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
    __declspec(dllimport) int __cdecl _vsnwprintf_s_l([SA_Pre(Null=SA_No, WritableElements="_DstSize")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_swprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(dllimport) int __cdecl _swprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "vswprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(dllimport) int __cdecl _vswprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _Args);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "__swprintf_l_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(dllimport) int __cdecl __swprintf_l(wchar_t* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, _locale_t _Plocinfo, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vswprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(dllimport) int __cdecl __vswprintf_l(wchar_t* _Dest, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, _locale_t _Plocinfo, va_list _Args);

    static __inline int swprintf(wchar_t* _String, size_t _Count, const wchar_t* _Format, ...) {
        va_list _Arglist;
        int _Ret;
        (_Arglist = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
        _Ret = _vswprintf_c_l(_String, _Count, _Format, 0, _Arglist);
        (_Arglist = (va_list)0);
        return _Ret;
    }

    static __inline int __cdecl vswprintf(wchar_t* _String, size_t _Count, const wchar_t* _Format, va_list _Ap) {
        return _vswprintf_c_l(_String, _Count, _Format, 0, _Ap);
    }

    static __inline int _swprintf_l(wchar_t* _String, size_t _Count, const wchar_t* _Format, _locale_t _Plocinfo, ...) {
        va_list _Arglist;
        int _Ret;
        (_Arglist = (va_list)(&reinterpret_cast<const char&>(_Plocinfo)) + ((sizeof(_Plocinfo) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
        _Ret = _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Arglist);
        (_Arglist = (va_list)0);
        return _Ret;
    }

    static __inline int __cdecl _vswprintf_l(wchar_t* _String, size_t _Count, const wchar_t* _Format, _locale_t _Plocinfo, va_list _Ap) {
        return _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Ap);
    }

    extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "swprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int swprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, ...) {
        va_list _Arglist;
        (_Arglist = (va_list)(&reinterpret_cast<const char&>(_Format)) + ((sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
        int _Ret = _vswprintf(_String, _Format, _Arglist);
        (_Arglist = (va_list)0);
        return _Ret;
    }

    extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "vswprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int __cdecl vswprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _Ap) {
        return _vswprintf(_String, _Format, _Ap);
    }

    extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_swprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int _swprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, _locale_t _Plocinfo, ...) {
        va_list _Arglist;
        (_Arglist = (va_list)(&reinterpret_cast<const char&>(_Plocinfo)) + ((sizeof(_Plocinfo) + sizeof(int) - 1) & ~(sizeof(int) - 1)));
        int _Ret = __vswprintf_l(_String, _Format, _Plocinfo, _Arglist);
        (_Arglist = (va_list)0);
        return _Ret;
    }

    extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vswprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int __cdecl _vswprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _String, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, _locale_t _Plocinfo, va_list _Ap) {
        return __vswprintf_l(_String, _Format, _Plocinfo, _Ap);
    }

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) wchar_t * __cdecl _wtempnam([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Directory, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _FilePrefix);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscwprintf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _vscwprintf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl fwscanf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fwscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _fwscanf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl fwscanf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl _fwscanf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "swscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl swscanf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_swscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _swscanf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl swscanf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl _swscanf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Src, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snwscanf([SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const wchar_t* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _snwscanf_l([SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const wchar_t* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
    __declspec(dllimport) int __cdecl _snwscanf_s([SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const wchar_t* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, ...);
    __declspec(dllimport) int __cdecl _snwscanf_s_l([SA_Pre(Null=SA_No, ValidElements="_MaxCount")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] const wchar_t* _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "wscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl wscanf([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, ...);
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) int __cdecl _wscanf_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

    __declspec(dllimport) int __cdecl wscanf_s([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, ...);

    __declspec(dllimport) int __cdecl _wscanf_s_l([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _locale_t _Locale, ...);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _wfdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _FileHandle , [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) FILE * __cdecl _wfopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode);
    __declspec(dllimport) errno_t __cdecl _wfopen_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] FILE** _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfreopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) FILE * __cdecl _wfreopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _OldFile);
    __declspec(dllimport) errno_t __cdecl _wfreopen_s([SA_Pre(Null=SA_No, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_Maybe)] [SA_Post(Deref=2, Valid=SA_Yes)] FILE** _File, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _OldFile);

    __declspec(dllimport) void __cdecl _wperror([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _ErrMsg);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) FILE * __cdecl _wpopen([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Command, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) int __cdecl _wremove([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t* _Filename);
    __declspec(dllimport) errno_t __cdecl _wtmpnam_s([SA_Pre(Null=SA_No, WritableElements="_SizeInWords")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _SizeInWords);
    extern "C++" {
        template <size_t _Size> inline errno_t __cdecl _wtmpnam_s([SA_Post(Deref=1, Null=SA_No, NullTerminated=SA_Yes)] [SA_Post(Deref=2, Valid=SA_Yes)] wchar_t (&_Buffer)[_Size]) {
            return _wtmpnam_s(_Buffer, _Size);
        }
    }
    __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wtmpnam_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(dllimport) wchar_t* __cdecl _wtmpnam([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _Buffer);

    __declspec(dllimport) wint_t __cdecl _fgetwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wint_t __cdecl _fputwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) wint_t __cdecl _ungetwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] wint_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);

    inline wint_t __cdecl getwchar() {
        return (fgetwc((&__iob_func()[0])));
    }
    inline wint_t __cdecl putwchar(wchar_t _C) {
        return (fputwc(_C, (&__iob_func()[1])));
    }
    __declspec(dllimport) void __cdecl _lock_file([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) void __cdecl _unlock_file([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fclose_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fflush_nolock([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) size_t __cdecl _fread_nolock([SA_Pre(Null=SA_No, WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1, Valid=SA_No)] void* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) size_t __cdecl _fread_nolock_s([SA_Pre(Null=SA_No, WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1, Valid=SA_No)] void* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fseek_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] long _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) long __cdecl _ftell_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _fseeki64_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] __int64 _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(dllimport) __int64 __cdecl _ftelli64_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) size_t __cdecl _fwrite_nolock([SA_Pre(Null=SA_No, ValidBytes="\n@""_Size*_Count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const void* _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(dllimport) int __cdecl _ungetc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_tempnam" ". See online help for details.")) __declspec(dllimport) char* __cdecl tempnam([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Directory, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _FilePrefix);

    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fcloseall" ". See online help for details.")) __declspec(dllimport) int __cdecl fcloseall(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fdopen" ". See online help for details.")) __declspec(dllimport) FILE * __cdecl fdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _FileHandle, [SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const char* _Format);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fgetchar" ". See online help for details.")) __declspec(dllimport) int __cdecl fgetchar(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fileno" ". See online help for details.")) __declspec(dllimport) int __cdecl fileno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] FILE* _File);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_flushall" ". See online help for details.")) __declspec(dllimport) int __cdecl flushall(void);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fputchar" ". See online help for details.")) __declspec(dllimport) int __cdecl fputchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_getw" ". See online help for details.")) __declspec(dllimport) int __cdecl getw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
    __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_putw" ". See online help for details.")) __declspec(dllimport) int __cdecl putw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] FILE* _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_rmtmp" ". See online help for details.")) __declspec(dllimport) int __cdecl rmtmp(void);
}

typedef FILE _iobuffer;

extern "C" {
    typedef unsigned int size_t;
    typedef size_t rsize_t;
    typedef  int            intptr_t;
    typedef  unsigned int   uintptr_t;
    typedef  int            ptrdiff_t;
    typedef unsigned short wint_t;
    typedef unsigned short wctype_t;
    typedef char*  va_list;
    __declspec(deprecated("This name was supported during some Whidbey pre-releases. Instead, use the standard name errno_t."))
    typedef int errcode;
    typedef int errno_t;
    typedef  long __time32_t;
    typedef __int64 __time64_t;
    typedef __time64_t time_t;
    struct threadlocaleinfostruct;
    struct threadmbcinfostruct;
    typedef struct threadlocaleinfostruct* pthreadlocinfo;
    typedef struct threadmbcinfostruct* pthreadmbcinfo;
    struct __lc_time_data;
    typedef struct localeinfo_struct {
        pthreadlocinfo locinfo;
        pthreadmbcinfo mbcinfo;
    } _locale_tstruct, *_locale_t;

    typedef struct tagLC_ID {
        unsigned short wLanguage;
        unsigned short wCountry;
        unsigned short wCodePage;
    } LC_ID, *LPLC_ID;

    typedef struct threadlocaleinfostruct {
        int refcount;
        unsigned int lc_codepage;
        unsigned int lc_collate_cp;
        unsigned long lc_handle[6];
        LC_ID lc_id[6];
        struct {
            char* locale;
            wchar_t* wlocale;
            int* refcount;
            int* wrefcount;
        } lc_category[6];
        int lc_clike;
        int mb_cur_max;
        int* lconv_intl_refcount;
        int* lconv_num_refcount;
        int* lconv_mon_refcount;
        struct lconv* lconv;
        int* ctype1_refcount;
        unsigned short* ctype1;
        const unsigned short* pctype;
        struct __lc_time_data* lc_time_curr;
    } threadlocinfo;


    extern "C" {
        typedef unsigned long ULONG;
        typedef ULONG* PULONG;
        typedef unsigned short USHORT;
        typedef USHORT* PUSHORT;
        typedef unsigned char UCHAR;
        typedef UCHAR* PUCHAR;
        typedef char* PSZ;
        typedef unsigned long       DWORD;
        typedef int                 BOOL;
        typedef unsigned char       BYTE;
        typedef unsigned short      WORD;
        typedef float               FLOAT;
        typedef FLOAT* PFLOAT;
        typedef BOOL* PBOOL;
        typedef BOOL* LPBOOL;
        typedef BYTE* PBYTE;
        typedef BYTE* LPBYTE;
        typedef int* PINT;
        typedef int* LPINT;
        typedef WORD* PWORD;
        typedef WORD* LPWORD;
        typedef long* LPLONG;
        typedef DWORD* PDWORD;
        typedef DWORD* LPDWORD;
        typedef short* LPSHORT;
        typedef void* LPVOID;
        typedef const void* LPCVOID;
        typedef int                 INT;
        typedef unsigned int        UINT;
        typedef unsigned int* PUINT;
        extern "C" {
            typedef unsigned int size_t;
#pragma pack(push,8)
            extern "C" {
                extern int __argc;
                extern char** __argv;
                extern wchar_t** __wargv;
                struct _exception {
                    int type;
                    char* name;
                    double arg1;
                    double arg2;
                    double retval;
                };

                extern "C++" {
                    template <typename _CountofType, size_t _SizeOfArray>
                    char(*__countof_helper(__unaligned _CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];
                }
                __declspec(noalias)
                __declspec(restrict)
                void* __cdecl calloc(size_t _NumOfElements, size_t _SizeOfElements);
                __declspec(noalias)
                void   __cdecl free(void* _Memory);
                __declspec(noalias)
                __declspec(restrict)
                void* __cdecl malloc(size_t _Size);
                __declspec(noalias)
                __declspec(restrict)
                void* __cdecl realloc(void* _Memory, size_t _NewSize);
                size_t  __cdecl _msize(void* _Memory);
                void* __cdecl _alloca(size_t _Size);
                __declspec(noreturn) void __cdecl exit(int _Code);
                __declspec(noreturn) void __cdecl _exit(int _Code);
                void __cdecl _cexit(void);
                void __cdecl _c_exit(void);
                void __cdecl __security_init_cookie(void);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl wcscat(wchar_t* _Dest, const wchar_t* _Source);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl wcscpy(wchar_t* _Dest, const wchar_t* _Source);
                const wchar_t* __cdecl wcschr(const wchar_t* _Str, wchar_t _Ch);
                int __cdecl wcscmp(const wchar_t* _Str1, const wchar_t* _Str2);
                size_t __cdecl wcscspn(const wchar_t* _Str, const wchar_t* _Control);
                size_t __cdecl wcslen(const wchar_t* _Str);
                size_t __cdecl wcsnlen(const wchar_t* _Src, size_t _MaxCount);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl wcsncat(wchar_t* _Dest, const wchar_t* _Source, size_t _Count);
                int __cdecl wcsncmp(const wchar_t* _Str1, const wchar_t* _Str2, size_t _MaxCount);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl wcsncpy(wchar_t* _Dest, const wchar_t* _Source, size_t _Count);
                const wchar_t* __cdecl wcspbrk(const wchar_t* _Str, const wchar_t* _Control);
                const wchar_t* __cdecl wcsrchr(const wchar_t* _Str, wchar_t _Ch);
                size_t __cdecl wcsspn(const wchar_t* _Str, const wchar_t* _Control);
                const wchar_t* __cdecl wcsstr(const wchar_t* _Str, const wchar_t* _SubStr);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcstok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl wcstok(wchar_t* _Str, const wchar_t* _Delim);
                wchar_t* __cdecl _wcsdup(const wchar_t* _Str);
                int __cdecl _wcsicmp(const wchar_t* _Str1, const wchar_t* _Str2);
                int __cdecl _wcsnicmp(const wchar_t* _Str1, const wchar_t* _Str2, size_t _MaxCount);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _wcsnset(wchar_t* _Str, wchar_t _Val, size_t _MaxCount);
                wchar_t* __cdecl _wcsrev(wchar_t* _Str);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _wcsset(wchar_t* _Str, wchar_t _Val);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcslwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _wcslwr(wchar_t* _String);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _wcsupr(wchar_t* _String);
                extern "C++" {
                    inline  wchar_t* __cdecl wcschr(wchar_t* _Str, wchar_t _Ch) {
                        return ((wchar_t*)wcschr((const wchar_t*)_Str, _Ch));
                    }
                    inline  wchar_t* __cdecl wcspbrk(wchar_t* _Str, const wchar_t* _Control) {
                        return ((wchar_t*)wcspbrk((const wchar_t*)_Str, _Control));
                    }
                    inline  wchar_t* __cdecl wcsrchr(wchar_t* _Str, wchar_t _Ch) {
                        return ((wchar_t*)wcsrchr((const wchar_t*)_Str, _Ch));
                    }
                    inline  wchar_t* __cdecl wcsstr(wchar_t* _Str, const wchar_t* _SubStr) {
                        return ((wchar_t*)wcsstr((const wchar_t*)_Str, _SubStr));
                    }
                }
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strcpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl strcpy(char* _Dest, const char* _Source);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strcat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl strcat(char* _Dest, const char* _Source);
                int __cdecl strcmp(const char* _Str1, const char* _Str2);
                size_t __cdecl strlen(const char* _Str);
                const char*  __cdecl strchr(const char* _Str, int _Val);
                size_t __cdecl strcspn(const char* _Str, const char* _Control);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl strncat(char* _Dest, const char* _Source, size_t _Count);
                int __cdecl strncmp(const char* _Str1, const char* _Str2, size_t _MaxCount);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl strncpy(char* _Dest, const char* _Source, size_t _Count);
                const char*  __cdecl strstr(const char* _Str, const char* _SubStr);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strtok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char*  __cdecl strtok(char* _Str, const char* _Delim);
                int __cdecl _stricmp(const char* _Str1, const char* _Str2);
                int __cdecl _strnicmp(const char* _Str1, const char* _Str2, size_t _MaxCount);
                const char*  __cdecl strpbrk(const char* _Str, const char* _Control);
                const char*  __cdecl strrchr(const char* _Str, int _Ch);
                size_t  __cdecl strspn(const char* _Str, const char* _Control);
                char*  __cdecl _strdup(const char* _Src);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char*  __cdecl _strnset(char* _Str, int _Val, size_t _MaxCount);
                char*  __cdecl _strrev(char* _Str);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char*  __cdecl _strset(char* _Str, int _Val);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strlwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _strlwr(char* _String);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _strupr(char* _String);
                extern "C++" {
                    inline  char* __cdecl strchr(char* _Str, int _Ch) {
                        return (char*)strchr((const char*)_Str, _Ch);
                    }
                    inline  char* __cdecl strpbrk(char* _Str, const char* _Control) {
                        return (char*)strpbrk((const char*)_Str, _Control);
                    }
                    inline  char* __cdecl strrchr(char* _Str, int _Ch) {
                        return (char*)strrchr((const char*)_Str, _Ch);
                    }
                    inline  char* __cdecl strstr(char* _Str, const char* _SubStr) {
                        return (char*)strstr((const char*)_Str, _SubStr);
                    }
                }
                void*  __cdecl _memccpy(void* _Dst, const void* _Src, int _Val, size_t _MaxCount);
                const void*  __cdecl memchr(const void* _Buf, int _Val, size_t _MaxCount);
                int     __cdecl _memicmp(const void* _Buf1, const void* _Buf2, size_t _Size);
                int     __cdecl memcmp(const void* _Buf1, const void* _Buf2, size_t _Size);
                void*  __cdecl memcpy(void* _Dst, const void* _Src, size_t _Size);
                void* __cdecl memset(void* _Dst, int _Val, size_t _Size);
                void*  __cdecl memmove(void* _Dst, const void* _Src, size_t _Size);
                extern "C++" {
                    inline  void* __cdecl memchr(void* _Pv, int _C, size_t _N) {
                        return (void*)memchr((const void*)_Pv, _C, _N);
                    }
                }

                int __cdecl iswctype(wint_t _C, wctype_t _Type);
                wint_t __cdecl towupper(wint_t _C);
                wint_t __cdecl towlower(wint_t _C);
                int __cdecl toupper(int _C);
                int __cdecl tolower(int _C);
                int __cdecl _isctype(int _C, int _Type);
                typedef void FILE;
                typedef __int64 fpos_t;
                int __cdecl setvbuf(FILE* _File, char* _Buf, int _Mode, size_t _Size);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "sscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl sscanf(const char* _Src, const char* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "sprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl sprintf(char* _Dest, const char* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "vsprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl vsprintf(char* _Dest, const char* _Format, va_list _Args);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl _snprintf(char* _Dest, size_t _Count, const char* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl _vsnprintf(char* _Dest, size_t _Count, const char* _Format, va_list _Args);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "swprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl swprintf(wchar_t* _Dest, const wchar_t* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "vswprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl vswprintf(wchar_t* _Dest, const wchar_t* _Format, va_list _Args);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl _snwprintf(wchar_t* _Dest, size_t _Count, const wchar_t* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl _vsnwprintf(wchar_t* _Dest, size_t _Count, const wchar_t* _Format, va_list _Args);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "swscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl swscanf(const wchar_t* _Src, const wchar_t* _Format, ...);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "scanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl scanf(const char* _Format, ...);
                int __cdecl printf(const char* _Format, ...);
                int __cdecl vprintf(const char* _Format, va_list _ArgList);
                int __cdecl getchar(void);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "gets_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl gets(char* _Buffer);
                int __cdecl putchar(int _Ch);
                int __cdecl puts(const char* _Str);
                int __cdecl fgetc(FILE* _File);
                char* __cdecl fgets(char* _Buf, int _MaxCount, FILE* _File);
                int __cdecl fputc(int _Ch, FILE* _File);
                int __cdecl fputs(const char* _Str, FILE* _File);
                int __cdecl ungetc(int _Ch, FILE* _File);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "fopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                FILE* __cdecl fopen(const char* _Filename, const char* _Mode);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "fscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl fscanf(FILE* _File, const char* _Format, ...);
                int __cdecl fprintf(FILE* _File, const char* _Format, ...);
                int __cdecl vfprintf(FILE* _File, const char* _Format, va_list _ArgList);
                FILE*  __cdecl _getstdfilex(int _FileHandle);
                int __cdecl fclose(FILE* _File);
                int __cdecl _fcloseall(void);
                size_t __cdecl fread(void* _DstBuf, size_t _ElementSize, size_t _Count, FILE* _File);
                size_t __cdecl fwrite(const void* _Str, size_t _Size, size_t _Count, FILE* _File);
                int __cdecl fflush(FILE* _File);
                int __cdecl _flushall(void);
                int __cdecl feof(FILE* _File);
                int __cdecl ferror(FILE* _File);
                void __cdecl clearerr(FILE* _File);
                int __cdecl fgetpos(FILE* _File, fpos_t* _Pos);
                int __cdecl fsetpos(FILE* _File, const fpos_t* _Pos);
                int __cdecl fseek(FILE* _File, long _Offset, int _Origin);
                long __cdecl ftell(FILE* _File);
                void* __cdecl _fileno(FILE* _File);
                FILE* __cdecl _wfdopen(void* _FileHandle, const wchar_t* _Mode);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfreopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                FILE* __cdecl _wfreopen(const wchar_t* _Filename, const wchar_t* _Mode, FILE* _OldFile);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl wscanf(const wchar_t* _Format, ...);
                int __cdecl wprintf(const wchar_t* _Format, ...);
                int __cdecl vwprintf(const wchar_t* _Format, va_list _ArgList);
                wint_t __cdecl getwchar(void);
                wint_t __cdecl putwchar(wchar_t _Ch);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_getws_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _getws(wchar_t* _String);
                int __cdecl _putws(const wchar_t* _Str);
                wint_t __cdecl fgetwc(FILE* _File);
                wint_t __cdecl fputwc(wchar_t _Ch, FILE* _File);
                wint_t __cdecl ungetwc(wint_t _Ch, FILE* _File);
                wchar_t* __cdecl fgetws(wchar_t* _Dst, int _SizeInWords, FILE* _File);
                int __cdecl fputws(const wchar_t* _Str, FILE* _File);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                FILE* __cdecl _wfopen(const wchar_t* _Filename, const wchar_t* _Mode);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "fwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                int __cdecl fwscanf(FILE* _File, const wchar_t* _Format, ...);
                int __cdecl fwprintf(FILE* _File, const wchar_t* _Format, ...);
                int __cdecl vfwprintf(FILE* _File, const wchar_t* _Format, va_list _ArgList);
                int __cdecl _setmode(FILE* _File, int _Mode);
                errno_t __cdecl mbstowcs_s(size_t* _PtNumOfCharConverted, wchar_t* _DstBuf, size_t _SizeInWords, const char* _SrcBuf, size_t _MaxCount);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "mbstowcs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                size_t __cdecl mbstowcs(wchar_t* _Dest, const char* _Source, size_t _MaxCount);
                errno_t __cdecl wcstombs_s(size_t* _PtNumOfCharConverted, char* _Dst, size_t _DstSizeInBytes, const wchar_t* _Src, size_t _MaxCountInBytes);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcstombs_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                size_t __cdecl wcstombs(char* _Dest, const wchar_t* _Source, size_t _MaxCount);
                double __cdecl strtod(const char* _Str, char** _EndPtr);
                long   __cdecl strtol(const char* _Str, char** _EndPtr, int _Radix);
                unsigned long __cdecl strtoul(const char* _Str, char** _EndPtr, int _Radix);
                __int64 __cdecl _atoi64(const char* _String);
                int    __cdecl atoi(const char* _Str);
                double  __cdecl atof(const char* _String);
                long   __cdecl atol(const char* _Str);
                unsigned short __cdecl _byteswap_ushort(unsigned short _Short);
                unsigned long  __cdecl _byteswap_ulong(unsigned long _Long);
                unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64 _Int64);
                errno_t __cdecl _itoa_s(int _Value, char* _DstBuf, size_t _Size, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_itoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _itoa(int _Value, char* _Dest, int _Radix);
                errno_t __cdecl _ltoa_s(long _Val, char* _DstBuf, size_t _Size, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ltoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _ltoa(long _Value, char* _Dest, int _Radix);
                errno_t __cdecl _ultoa_s(unsigned long _Val, char* _DstBuf, size_t _Size, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ultoa_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _ultoa(unsigned long _Value, char* _Dest, int _Radix);
                errno_t __cdecl _ecvt_s(char* _DstBuf, size_t _Size, double _Val, int _NumOfDights, int* _PtDec, int* _PtSign);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ecvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _ecvt(double _Val, int _NumOfDigits, int* _PtDec, int* _PtSign);
                errno_t __cdecl _fcvt_s(char* _DstBuf, size_t _Size, double _Val, int _NumOfDec, int* _PtDec, int* _PtSign);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _fcvt(double _Val, int _NumOfDec, int* _PtDec, int* _PtSign);
                errno_t __cdecl _gcvt_s(char* _DstBuf, size_t _Size, double _Val, int _NumOfDigits);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_gcvt_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _gcvt(double _Val, int _NumOfDigits, char* _DstBuf);
                double __cdecl wcstod(const wchar_t* _Str, wchar_t** _EndPtr);
                long   __cdecl wcstol(const wchar_t* _Str, wchar_t** _EndPtr, int _Radix);
                errno_t __cdecl _itow_s(int _Val, wchar_t* _DstBuf, size_t _SizeInWords, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_itow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _itow(int _Value, wchar_t* _Dest, int _Radix);
                errno_t __cdecl _ltow_s(long _Val, wchar_t* _DstBuf, size_t _SizeInWords, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ltow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _ltow(long _Value, wchar_t* _Dest, int _Radix);
                errno_t __cdecl _ultow_s(unsigned long _Val, wchar_t* _DstBuf, size_t _SizeInWords, int _Radix);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_ultow_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                wchar_t* __cdecl _ultow(unsigned long _Value, wchar_t* _Dest, int _Radix);
                long __cdecl _wtol(const wchar_t* _Str);
                __int64   __cdecl _wtoll(const wchar_t* _Str);
                unsigned long __cdecl wcstoul(const wchar_t* _Str, wchar_t** _EndPtr, int _Radix);
                unsigned long __cdecl _lrotl(unsigned long _Val, int _Shift);
                unsigned long __cdecl _lrotr(unsigned long _Val, int _Shift);
                unsigned int __cdecl _rotl(unsigned int _Val, int _Shift);
                unsigned int __cdecl _rotr(unsigned int _Val, int _Shift);
                unsigned __int64 __cdecl _rotl64(unsigned __int64 _Val, int _Shift);
                unsigned __int64 __cdecl _rotr64(unsigned __int64 _Val, int _Shift);
                void   __cdecl _swab(char* _Buf1, char* _Buf2, int _SizeInBytes);
                int    __cdecl rand(void);
                errno_t __cdecl rand_s(unsigned int* _RandomValue);
                void   __cdecl srand(unsigned int _Seed);
                void __cdecl qsort_s(void* _Base,
                                     rsize_t _NumOfElements, rsize_t _SizeOfElements,
                                     int (__cdecl* _PtFuncCompare)(void*, const void*, const void*), void* _Context);
                void __cdecl qsort(void* _Base,
                                   size_t _NumOfElements, size_t _SizeOfElements,
                                   int (__cdecl* _PtFuncCompare)(const void*, const void*));
                double __cdecl difftime(__time32_t _Time1, __time32_t _Time2);
                int    __cdecl atexit(void (__cdecl*)(void));
                typedef int (__cdecl* _onexit_t)(void);
                typedef struct _div_t {
                    int quot;
                    int rem;
                } div_t;
                typedef struct _ldiv_t {
                    long quot;
                    long rem;
                } ldiv_t;
                div_t  __cdecl div(int _Numerator, int _Denominator);
                ldiv_t __cdecl ldiv(long _Numerator, long _Denominator);
                typedef void (__cdecl* _invalid_parameter_handler)(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t);
                _invalid_parameter_handler __cdecl _set_invalid_parameter_handler(_invalid_parameter_handler _Handler);
                _invalid_parameter_handler __cdecl _get_invalid_parameter_handler(void);
                unsigned int __cdecl _clearfp(void);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_controlfp_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                unsigned int __cdecl _controlfp(unsigned int _NewValue, unsigned int _Mask);
                unsigned int __cdecl _statusfp(void);
                void __cdecl _fpreset(void);
                double __cdecl _copysign(double _Number, double _Sign);
                double __cdecl _chgsign(double _X);
                double __cdecl _scalb(double _X, long _Y);
                double __cdecl _logb(double _X);
                double __cdecl _nextafter(double _X, double _Y);
                int    __cdecl _finite(double _X);
                int    __cdecl _isnan(double _X);
                int    __cdecl _fpclass(double _X);
                typedef float           _FP32;
                typedef double          _FP64;
                typedef short           _I16;
                typedef int             _I32;
                typedef unsigned short  _U16;
                typedef unsigned int    _U32;
                typedef struct {
                    unsigned long W[4];
                } _U32ARRAY;
                typedef struct {
                    unsigned short W[5];
                } _FP80;
                typedef struct {
                    unsigned long W[4];
                } _FP128;
                typedef struct {
                    unsigned long W[2];
                } _I64;
                typedef struct {
                    unsigned long W[2];
                } _U64;
                typedef struct {
                    unsigned short W[5];
                } _BCD80;
                typedef struct {
                    union {
                        _FP32        Fp32Value;
                        _FP64        Fp64Value;
                        _FP80        Fp80Value;
                        _FP128       Fp128Value;
                        _I16         I16Value;
                        _I32         I32Value;
                        _I64         I64Value;
                        _U16         U16Value;
                        _U32         U32Value;
                        _U64         U64Value;
                        _BCD80       Bcd80Value;
                        char* StringValue;
                        int      CompareValue;
                        _U32ARRAY    U32ArrayValue;
                    } Value;
                    unsigned int OperandValid : 1;
                    unsigned int Format : 4;
                } _FPIEEE_VALUE;
                typedef struct {
                    unsigned int Inexact : 1;
                    unsigned int Underflow : 1;
                    unsigned int Overflow : 1;
                    unsigned int ZeroDivide : 1;
                    unsigned int InvalidOperation : 1;
                } _FPIEEE_EXCEPTION_FLAGS;
                typedef struct {
                    unsigned int RoundingMode : 2;
                    unsigned int Precision : 3;
                    unsigned int Operation : 12;
                    _FPIEEE_EXCEPTION_FLAGS Cause;
                    _FPIEEE_EXCEPTION_FLAGS Enable;
                    _FPIEEE_EXCEPTION_FLAGS Status;
                    _FPIEEE_VALUE Operand1;
                    _FPIEEE_VALUE Operand2;
                    _FPIEEE_VALUE Result;
                } _FPIEEE_RECORD;
                struct _EXCEPTION_POINTERS;
                int __cdecl _fpieee_flt(
                    unsigned long _ExceptionCode,
                    struct _EXCEPTION_POINTERS* _PtExceptionPtr,
                    int (__cdecl* _Handler)(_FPIEEE_RECORD*)
                );
                double _frnd(double _X);
                double _fsqrt(double _X);
                struct _complex {
                    double x, y;
                };
                __declspec(dllimport)
                extern double _HUGE;
                int       __cdecl abs(int _X);
                long      __cdecl labs(long _X);
                __int64   __cdecl _abs64(__int64 _X);
                float     __cdecl ceilf(float _X);
                float     __cdecl fabsf(float _X);
                float     __cdecl floorf(float _X);
                float     __cdecl fmodf(float _X, float _Y);
                float     __cdecl sqrtf(float _X);
                double    __cdecl acos(double _X);
                double    __cdecl asin(double _X);
                double    __cdecl atan(double _X);
                double    __cdecl atan2(double _Y, double _X);
                double    __cdecl cos(double _X);
                double    __cdecl cosh(double _X);
                double    __cdecl exp(double _X);
                double    __cdecl fabs(double _X);
                double    __cdecl fmod(double _X, double _Y);
                double    __cdecl log(double _X);
                double    __cdecl log10(double _X);
                double    __cdecl pow(double _X, double _Y);
                double    __cdecl sin(double _X);
                double    __cdecl sinh(double _X);
                double    __cdecl sqrt(double _X);
                double    __cdecl tan(double _X);
                double    __cdecl tanh(double _X);
                double    __cdecl _cabs(struct _complex _Complex);
                double    __cdecl ceil(double _X);
                double    __cdecl floor(double _X);
                double    __cdecl frexp(double _X, int* _Y);
                double    __cdecl _hypot(double _X, double _Y);
                double    __cdecl _j0(double _X);
                double    __cdecl _j1(double _X);
                double    __cdecl _jn(int _X, double _Y);
                double    __cdecl ldexp(double _X, int _Y);
                int       __cdecl _matherr(struct _exception* _Except);
                double    __cdecl modf(double _X, double* _Y);
                double    __cdecl _y0(double _X);
                double    __cdecl _y1(double _X);
                double    __cdecl _yn(int _X, double _Y);
                typedef int jmp_buf[11];
                int __cdecl _setjmp(jmp_buf _Buf);
                __declspec(noreturn)
                void __cdecl longjmp(jmp_buf _Buf, int _Value) throw(...);
            }
#pragma pack(pop)

            typedef unsigned long POINTER_64_INT;
            extern "C" {
                typedef int LONG32, *PLONG32;
                typedef int INT32, *PINT32;
                typedef unsigned int ULONG32, *PULONG32;
                typedef unsigned int DWORD32, *PDWORD32;
                typedef unsigned int UINT32, *PUINT32;
                typedef int INT_PTR, *PINT_PTR;
                typedef unsigned int UINT_PTR, *PUINT_PTR;
                typedef long LONG_PTR, *PLONG_PTR;
                typedef unsigned long ULONG_PTR, *PULONG_PTR;
                typedef unsigned short UHALF_PTR, *PUHALF_PTR;
                typedef short HALF_PTR, *PHALF_PTR;
                typedef ULONG_PTR SIZE_T, *PSIZE_T;
                typedef LONG_PTR SSIZE_T, *PSSIZE_T;
                typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
                typedef __int64 LONG64, *PLONG64;
                typedef __int64 INT64, *PINT64;
                typedef unsigned __int64 ULONG64, *PULONG64;
                typedef unsigned __int64 DWORD64, *PDWORD64;
                typedef unsigned __int64 UINT64, *PUINT64;
            }
            typedef void* PVOID;
            typedef void* PVOID64;
            typedef char CHAR;
            typedef short SHORT;
            typedef long LONG;
            typedef wchar_t WCHAR;
            typedef WCHAR* PWCHAR, *LPWCH, *PWCH;
            typedef const WCHAR* LPCWCH, *PCWCH;
            typedef  WCHAR* NWPSTR, *LPWSTR, *PWSTR;
            typedef  WCHAR __unaligned* LPUWSTR, *PUWSTR;
            typedef  const WCHAR* LPCWSTR, *PCWSTR;
            typedef  const WCHAR __unaligned* LPCUWSTR, *PCUWSTR;
            typedef const WCHAR* LPCWCHAR, *PCWCHAR;
            typedef const WCHAR __unaligned* LPCUWCHAR, *PCUWCHAR;
            typedef CHAR* PCHAR, *LPCH, *PCH;
            typedef const CHAR* LPCCH, *PCCH;
            typedef  CHAR* NPSTR, *LPSTR, *PSTR;
            typedef  const CHAR* LPCSTR, *PCSTR;
            typedef wchar_t         _TCHAR;
            typedef wchar_t         _TSCHAR;
            typedef wchar_t         _TUCHAR;
            typedef wchar_t         _TXCHAR;
            typedef wint_t          _TINT;
            typedef WCHAR           TCHAR, *PTCHAR;
            typedef WCHAR           TBYTE, *PTBYTE;
            typedef LPWSTR          LPTCH, PTCH;
            typedef LPWSTR          LPTSTR, PTSTR;
            typedef LPCWSTR         LPCTSTR, PCTSTR;
            typedef LPWSTR          LP;
            typedef SHORT* PSHORT;
            typedef LONG* PLONG;
            typedef void* HANDLE;
            typedef HANDLE* PHANDLE;
            typedef BYTE   FCHAR;
            typedef WORD   FSHORT;
            typedef DWORD  FLONG;
            typedef LONG HRESULT;

            typedef char CCHAR;
            typedef DWORD LCID;
            typedef PDWORD PLCID;
            typedef WORD   LANGID;
            typedef struct _FLOAT128 {
                __int64 LowPart;
                __int64 HighPart;
            } FLOAT128;
            typedef FLOAT128* PFLOAT128;
            typedef __int64 LONGLONG;
            typedef unsigned __int64 ULONGLONG;
            typedef LONGLONG* PLONGLONG;
            typedef ULONGLONG* PULONGLONG;
            typedef LONGLONG USN;
            typedef union _LARGE_INTEGER {
                struct {
                    DWORD LowPart;
                    LONG HighPart;
                };
                struct {
                    DWORD LowPart;
                    LONG HighPart;
                } u;
                LONGLONG QuadPart;
            } LARGE_INTEGER;
            typedef LARGE_INTEGER* PLARGE_INTEGER;
            typedef union _ULARGE_INTEGER {
                struct {
                    DWORD LowPart;
                    DWORD HighPart;
                };
                struct {
                    DWORD LowPart;
                    DWORD HighPart;
                } u;
                ULONGLONG QuadPart;
            } ULARGE_INTEGER;
            typedef ULARGE_INTEGER* PULARGE_INTEGER;
            typedef struct _LUID {
                DWORD LowPart;
                LONG HighPart;
            } LUID, *PLUID;
            typedef ULONGLONG  DWORDLONG;
            typedef DWORDLONG* PDWORDLONG;
            typedef BYTE  BOOLEAN;
            typedef BOOLEAN* PBOOLEAN;
            typedef struct _LIST_ENTRY {
                struct _LIST_ENTRY* Flink;
                struct _LIST_ENTRY* Blink;
            } LIST_ENTRY, *PLIST_ENTRY, * PRLIST_ENTRY;
            typedef struct _SINGLE_LIST_ENTRY {
                struct _SINGLE_LIST_ENTRY* Next;
            } SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;
            typedef struct _GUID {
                DWORD Data1;
                WORD   Data2;
                WORD   Data3;
                BYTE  Data4[8];
            } GUID;
            typedef struct  _OBJECTID {
                GUID Lineage;
                DWORD Uniquifier;
            } OBJECTID;
            extern "C++" template <typename T, size_t N> char(*RtlpNumberOf(__unaligned T(&)[N]))[N];
            typedef long NTSTATUS, *PNTSTATUS;
            typedef struct _LDT_ENTRY {
                WORD    LimitLow;
                WORD    BaseLow;
                union {
                    struct {
                        BYTE    BaseMid;
                        BYTE    Flags1;
                        BYTE    Flags2;
                        BYTE    BaseHi;
                    } Bytes;
                    struct {
                        DWORD   BaseMid : 8;
                        DWORD   Type : 5;
                        DWORD   Dpl : 2;
                        DWORD   Pres : 1;
                        DWORD   LimitHi : 4;
                        DWORD   Sys : 1;
                        DWORD   Reserved_0 : 1;
                        DWORD   Default_Big : 1;
                        DWORD   Granularity : 1;
                        DWORD   BaseHi : 8;
                    } Bits;
                } HighWord;
            } LDT_ENTRY, *PLDT_ENTRY;
            typedef struct _CONTEXT {
                ULONG ContextFlags;
                ULONG R0;
                ULONG R1;
                ULONG R2;
                ULONG R3;
                ULONG R4;
                ULONG R5;
                ULONG R6;
                ULONG R7;
                ULONG R8;
                ULONG R9;
                ULONG R10;
                ULONG R11;
                ULONG R12;
                ULONG Sp;
                ULONG Lr;
                ULONG Pc;
                ULONG Psr;
                ULONG Fpscr;
                ULONG FpExc;
                ULONG S[32+1];
                ULONG FpExtra[8];
            } CONTEXT;
            typedef CONTEXT* PCONTEXT;
            typedef struct _EXCEPTION_RECORD {
                DWORD    ExceptionCode;
                DWORD ExceptionFlags;
                struct _EXCEPTION_RECORD* ExceptionRecord;
                PVOID ExceptionAddress;
                DWORD NumberParameters;
                ULONG_PTR ExceptionInformation[15];
            } EXCEPTION_RECORD;
            typedef EXCEPTION_RECORD* PEXCEPTION_RECORD;
            typedef struct _EXCEPTION_POINTERS {
                PEXCEPTION_RECORD ExceptionRecord;
                PCONTEXT ContextRecord;
            } EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
            typedef PVOID PACCESS_TOKEN;
            typedef PVOID PSECURITY_DESCRIPTOR;
            typedef PVOID PSID;
            typedef DWORD ACCESS_MASK;
            typedef ACCESS_MASK* PACCESS_MASK;
            typedef struct _GENERIC_MAPPING {
                ACCESS_MASK GenericRead;
                ACCESS_MASK GenericWrite;
                ACCESS_MASK GenericExecute;
                ACCESS_MASK GenericAll;
            } GENERIC_MAPPING;
            typedef GENERIC_MAPPING* PGENERIC_MAPPING;

#pragma pack(push)
#pragma pack(4)
            typedef struct _LUID_AND_ATTRIBUTES {
                LUID Luid;
                DWORD Attributes;
            } LUID_AND_ATTRIBUTES, * PLUID_AND_ATTRIBUTES;
            typedef LUID_AND_ATTRIBUTES LUID_AND_ATTRIBUTES_ARRAY[1];
            typedef LUID_AND_ATTRIBUTES_ARRAY* PLUID_AND_ATTRIBUTES_ARRAY;
#pragma pack(pop)
            typedef struct _SID_IDENTIFIER_AUTHORITY {
                BYTE  Value[6];
            } SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
            typedef struct _SID {
                BYTE  Revision;
                BYTE  SubAuthorityCount;
                SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
                DWORD SubAuthority[1];
            } SID, *PISID;
            typedef enum _SID_NAME_USE {
                SidTypeUser = 1,
                SidTypeGroup,
                SidTypeDomain,
                SidTypeAlias,
                SidTypeWellKnownGroup,
                SidTypeDeletedAccount,
                SidTypeInvalid,
                SidTypeUnknown,
                SidTypeComputer
            } SID_NAME_USE, *PSID_NAME_USE;
            typedef struct _SID_AND_ATTRIBUTES {
                PSID Sid;
                DWORD Attributes;
            } SID_AND_ATTRIBUTES, * PSID_AND_ATTRIBUTES;
            typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[1];
            typedef SID_AND_ATTRIBUTES_ARRAY* PSID_AND_ATTRIBUTES_ARRAY;

            typedef struct _ACL {
                BYTE  AclRevision;
                BYTE  Sbz1;
                WORD   AclSize;
                WORD   AceCount;
                WORD   Sbz2;
            } ACL;
            typedef ACL* PACL;
            typedef struct _ACE_HEADER {
                BYTE  AceType;
                BYTE  AceFlags;
                WORD   AceSize;
            } ACE_HEADER;
            typedef ACE_HEADER* PACE_HEADER;
            typedef struct _ACCESS_ALLOWED_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD SidStart;
            } ACCESS_ALLOWED_ACE;
            typedef ACCESS_ALLOWED_ACE* PACCESS_ALLOWED_ACE;
            typedef struct _ACCESS_DENIED_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD SidStart;
            } ACCESS_DENIED_ACE;
            typedef ACCESS_DENIED_ACE* PACCESS_DENIED_ACE;
            typedef struct _SYSTEM_AUDIT_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD SidStart;
            } SYSTEM_AUDIT_ACE;
            typedef SYSTEM_AUDIT_ACE* PSYSTEM_AUDIT_ACE;
            typedef struct _SYSTEM_ALARM_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD SidStart;
            } SYSTEM_ALARM_ACE;
            typedef SYSTEM_ALARM_ACE* PSYSTEM_ALARM_ACE;
            typedef struct _ACCESS_ALLOWED_OBJECT_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD Flags;
                GUID ObjectType;
                GUID InheritedObjectType;
                DWORD SidStart;
            } ACCESS_ALLOWED_OBJECT_ACE, *PACCESS_ALLOWED_OBJECT_ACE;
            typedef struct _ACCESS_DENIED_OBJECT_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD Flags;
                GUID ObjectType;
                GUID InheritedObjectType;
                DWORD SidStart;
            } ACCESS_DENIED_OBJECT_ACE, *PACCESS_DENIED_OBJECT_ACE;
            typedef struct _SYSTEM_AUDIT_OBJECT_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD Flags;
                GUID ObjectType;
                GUID InheritedObjectType;
                DWORD SidStart;
            } SYSTEM_AUDIT_OBJECT_ACE, *PSYSTEM_AUDIT_OBJECT_ACE;
            typedef struct _SYSTEM_ALARM_OBJECT_ACE {
                ACE_HEADER Header;
                ACCESS_MASK Mask;
                DWORD Flags;
                GUID ObjectType;
                GUID InheritedObjectType;
                DWORD SidStart;
            } SYSTEM_ALARM_OBJECT_ACE, *PSYSTEM_ALARM_OBJECT_ACE;

            typedef enum _ACL_INFORMATION_CLASS {
                AclRevisionInformation = 1,
                AclSizeInformation
            } ACL_INFORMATION_CLASS;
            typedef struct _ACL_REVISION_INFORMATION {
                DWORD AclRevision;
            } ACL_REVISION_INFORMATION;
            typedef ACL_REVISION_INFORMATION* PACL_REVISION_INFORMATION;
            typedef struct _ACL_SIZE_INFORMATION {
                DWORD AceCount;
                DWORD AclBytesInUse;
                DWORD AclBytesFree;
            } ACL_SIZE_INFORMATION;
            typedef ACL_SIZE_INFORMATION* PACL_SIZE_INFORMATION;
            typedef WORD   SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;
            typedef struct _SECURITY_DESCRIPTOR_RELATIVE {
                BYTE  Revision;
                BYTE  Sbz1;
                SECURITY_DESCRIPTOR_CONTROL Control;
                DWORD Owner;
                DWORD Group;
                DWORD Sacl;
                DWORD Dacl;
            } SECURITY_DESCRIPTOR_RELATIVE, *PISECURITY_DESCRIPTOR_RELATIVE;
            typedef struct _SECURITY_DESCRIPTOR {
                BYTE  Revision;
                BYTE  Sbz1;
                SECURITY_DESCRIPTOR_CONTROL Control;
                PSID Owner;
                PSID Group;
                PACL Sacl;
                PACL Dacl;
            } SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;
            typedef struct _OBJECT_TYPE_LIST {
                WORD   Level;
                WORD   Sbz;
                GUID* ObjectType;
            } OBJECT_TYPE_LIST, *POBJECT_TYPE_LIST;
            typedef enum _AUDIT_EVENT_TYPE {
                AuditEventObjectAccess,
                AuditEventDirectoryServiceAccess
            } AUDIT_EVENT_TYPE, *PAUDIT_EVENT_TYPE;
            typedef struct _PRIVILEGE_SET {
                DWORD PrivilegeCount;
                DWORD Control;
                LUID_AND_ATTRIBUTES Privilege[1];
            } PRIVILEGE_SET, * PPRIVILEGE_SET;
            typedef enum _SECURITY_IMPERSONATION_LEVEL {
                SecurityAnonymous,
                SecurityIdentification,
                SecurityImpersonation,
                SecurityDelegation
            } SECURITY_IMPERSONATION_LEVEL, * PSECURITY_IMPERSONATION_LEVEL;
            typedef enum _TOKEN_TYPE {
                TokenPrimary = 1,
                TokenImpersonation
            } TOKEN_TYPE;
            typedef TOKEN_TYPE* PTOKEN_TYPE;
            typedef enum _TOKEN_INFORMATION_CLASS {
                TokenUser = 1,
                TokenGroups,
                TokenPrivileges,
                TokenOwner,
                TokenPrimaryGroup,
                TokenDefaultDacl,
                TokenSource,
                TokenType,
                TokenImpersonationLevel,
                TokenStatistics,
                TokenRestrictedSids,
                TokenSessionId
            } TOKEN_INFORMATION_CLASS, *PTOKEN_INFORMATION_CLASS;
            typedef struct _TOKEN_USER {
                SID_AND_ATTRIBUTES User;
            } TOKEN_USER, *PTOKEN_USER;
            typedef struct _TOKEN_GROUPS {
                DWORD GroupCount;
                SID_AND_ATTRIBUTES Groups[1];
            } TOKEN_GROUPS, *PTOKEN_GROUPS;
            typedef struct _TOKEN_PRIVILEGES {
                DWORD PrivilegeCount;
                LUID_AND_ATTRIBUTES Privileges[1];
            } TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;
            typedef struct _TOKEN_OWNER {
                PSID Owner;
            } TOKEN_OWNER, *PTOKEN_OWNER;
            typedef struct _TOKEN_PRIMARY_GROUP {
                PSID PrimaryGroup;
            } TOKEN_PRIMARY_GROUP, *PTOKEN_PRIMARY_GROUP;
            typedef struct _TOKEN_DEFAULT_DACL {
                PACL DefaultDacl;
            } TOKEN_DEFAULT_DACL, *PTOKEN_DEFAULT_DACL;
            typedef struct _TOKEN_SOURCE {
                CHAR SourceName[8];
                LUID SourceIdentifier;
            } TOKEN_SOURCE, *PTOKEN_SOURCE;
            typedef struct _TOKEN_STATISTICS {
                LUID TokenId;
                LUID AuthenticationId;
                LARGE_INTEGER ExpirationTime;
                TOKEN_TYPE TokenType;
                SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
                DWORD DynamicCharged;
                DWORD DynamicAvailable;
                DWORD GroupCount;
                DWORD PrivilegeCount;
                LUID ModifiedId;
            } TOKEN_STATISTICS, *PTOKEN_STATISTICS;
            typedef struct _TOKEN_CONTROL {
                LUID TokenId;
                LUID AuthenticationId;
                LUID ModifiedId;
                TOKEN_SOURCE TokenSource;
            } TOKEN_CONTROL, *PTOKEN_CONTROL;
            typedef BOOLEAN SECURITY_CONTEXT_TRACKING_MODE, * PSECURITY_CONTEXT_TRACKING_MODE;
            typedef struct _SECURITY_QUALITY_OF_SERVICE {
                DWORD Length;
                SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
                SECURITY_CONTEXT_TRACKING_MODE ContextTrackingMode;
                BOOLEAN EffectiveOnly;
            } SECURITY_QUALITY_OF_SERVICE, * PSECURITY_QUALITY_OF_SERVICE;
            typedef struct _SE_IMPERSONATION_STATE {
                PACCESS_TOKEN Token;
                BOOLEAN CopyOnOpen;
                BOOLEAN EffectiveOnly;
                SECURITY_IMPERSONATION_LEVEL Level;
            } SE_IMPERSONATION_STATE, *PSE_IMPERSONATION_STATE;
            typedef DWORD SECURITY_INFORMATION, *PSECURITY_INFORMATION;
            typedef struct _NT_TIB {
                struct _EXCEPTION_REGISTRATION_RECORD* ExceptionList;
                PVOID StackBase;
                PVOID StackLimit;
                PVOID SubSystemTib;
                union {
                    PVOID FiberData;
                    DWORD Version;
                };
                PVOID ArbitraryUserPointer;
                struct _NT_TIB* Self;
            } NT_TIB;
            typedef NT_TIB* PNT_TIB;
            typedef struct _QUOTA_LIMITS {
                SIZE_T PagedPoolLimit;
                SIZE_T NonPagedPoolLimit;
                SIZE_T MinimumWorkingSetSize;
                SIZE_T MaximumWorkingSetSize;
                SIZE_T PagefileLimit;
                LARGE_INTEGER TimeLimit;
            } QUOTA_LIMITS;
            typedef QUOTA_LIMITS* PQUOTA_LIMITS;
            typedef struct _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION {
                LARGE_INTEGER TotalUserTime;
                LARGE_INTEGER TotalKernelTime;
                LARGE_INTEGER ThisPeriodTotalUserTime;
                LARGE_INTEGER ThisPeriodTotalKernelTime;
                DWORD TotalPageFaultCount;
                DWORD TotalProcesses;
                DWORD ActiveProcesses;
                DWORD TotalTerminatedProcesses;
            } JOBOBJECT_BASIC_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_ACCOUNTING_INFORMATION;
            typedef struct _JOBOBJECT_BASIC_LIMIT_INFORMATION {
                LARGE_INTEGER PerProcessUserTimeLimit;
                LARGE_INTEGER PerJobUserTimeLimit;
                DWORD LimitFlags;
                SIZE_T MinimumWorkingSetSize;
                SIZE_T MaximumWorkingSetSize;
                DWORD ActiveProcessLimit;
                DWORD Affinity;
                DWORD PriorityClass;
                DWORD SchedulingClass;
            } JOBOBJECT_BASIC_LIMIT_INFORMATION, *PJOBOBJECT_BASIC_LIMIT_INFORMATION;
            typedef struct _JOBOBJECT_BASIC_PROCESS_ID_LIST {
                DWORD NumberOfAssignedProcesses;
                DWORD NumberOfProcessIdsInList;
                ULONG_PTR ProcessIdList[1];
            } JOBOBJECT_BASIC_PROCESS_ID_LIST, *PJOBOBJECT_BASIC_PROCESS_ID_LIST;
            typedef struct _JOBOBJECT_BASIC_UI_RESTRICTIONS {
                DWORD UIRestrictionsClass;
            } JOBOBJECT_BASIC_UI_RESTRICTIONS, *PJOBOBJECT_BASIC_UI_RESTRICTIONS;
            typedef struct _JOBOBJECT_SECURITY_LIMIT_INFORMATION {
                DWORD SecurityLimitFlags;
                HANDLE JobToken;
                PTOKEN_GROUPS SidsToDisable;
                PTOKEN_PRIVILEGES PrivilegesToDelete;
                PTOKEN_GROUPS RestrictedSids;
            } JOBOBJECT_SECURITY_LIMIT_INFORMATION, *PJOBOBJECT_SECURITY_LIMIT_INFORMATION;
            typedef struct _JOBOBJECT_END_OF_JOB_TIME_INFORMATION {
                DWORD EndOfJobTimeAction;
            } JOBOBJECT_END_OF_JOB_TIME_INFORMATION, *PJOBOBJECT_END_OF_JOB_TIME_INFORMATION;
            typedef struct _JOBOBJECT_ASSOCIATE_COMPLETION_PORT {
                PVOID CompletionKey;
                HANDLE CompletionPort;
            } JOBOBJECT_ASSOCIATE_COMPLETION_PORT, *PJOBOBJECT_ASSOCIATE_COMPLETION_PORT;
            typedef enum _JOBOBJECTINFOCLASS {
                JobObjectBasicAccountingInformation = 1,
                JobObjectBasicLimitInformation,
                JobObjectBasicProcessIdList,
                JobObjectBasicUIRestrictions,
                JobObjectSecurityLimitInformation,
                JobObjectEndOfJobTimeInformation,
                JobObjectAssociateCompletionPortInformation,
                MaxJobObjectInfoClass
            } JOBOBJECTINFOCLASS;
            typedef struct _MEMORY_BASIC_INFORMATION {
                PVOID BaseAddress;
                PVOID AllocationBase;
                DWORD AllocationProtect;
                SIZE_T RegionSize;
                DWORD State;
                DWORD Protect;
                DWORD Type;
            } MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;
            typedef struct _MEMORY_BASIC_INFORMATION_VLM {
                union {
                    PVOID64 BaseAddress;
                    ULONGLONG BaseAddressAsUlongLong;
                };
                union {
                    PVOID64 AllocationBase;
                    ULONGLONG AllocationBaseAsUlongLong;
                };
                ULONGLONG RegionSize;
                DWORD AllocationProtect;
                DWORD State;
                DWORD Protect;
                DWORD Type;
            } MEMORY_BASIC_INFORMATION_VLM, *PMEMORY_BASIC_INFORMATION_VLM;
            typedef struct _FILE_NOTIFY_INFORMATION {
                DWORD NextEntryOffset;
                DWORD Action;
                DWORD FileNameLength;
                WCHAR FileName[1];
            } FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;
            typedef union _FILE_SEGMENT_ELEMENT {
                PVOID64 Buffer;
                ULONGLONG Alignment;
            } FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;
            typedef struct _REPARSE_DATA_BUFFER {
                DWORD  ReparseTag;
                WORD   ReparseDataLength;
                WORD   Reserved;
                union {
                    struct {
                        WORD   SubstituteNameOffset;
                        WORD   SubstituteNameLength;
                        WORD   PrintNameOffset;
                        WORD   PrintNameLength;
                        WCHAR PathBuffer[1];
                    } SymbolicLinkReparseBuffer;
                    struct {
                        WORD   SubstituteNameOffset;
                        WORD   SubstituteNameLength;
                        WORD   PrintNameOffset;
                        WORD   PrintNameLength;
                        WCHAR PathBuffer[1];
                    } MountPointReparseBuffer;
                    struct {
                        BYTE   DataBuffer[1];
                    } GenericReparseBuffer;
                };
            } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
            typedef struct _REPARSE_GUID_DATA_BUFFER {
                DWORD  ReparseTag;
                WORD   ReparseDataLength;
                WORD   Reserved;
                GUID   ReparseGuid;
                struct {
                    BYTE   DataBuffer[1];
                } GenericReparseBuffer;
            } REPARSE_GUID_DATA_BUFFER, *PREPARSE_GUID_DATA_BUFFER;
            typedef struct _REPARSE_POINT_INFORMATION {
                WORD   ReparseDataLength;
                WORD   UnparsedNameLength;
            } REPARSE_POINT_INFORMATION, *PREPARSE_POINT_INFORMATION;
            typedef DWORD EXECUTION_STATE;
            typedef enum {
                LT_DONT_CARE,
                LT_LOWEST_LATENCY
            } LATENCY_TIME;
            typedef struct _POWER_DEVICE_TIMEOUTS {
                DWORD   ConservationIdleTime;
                DWORD   PerformanceIdleTime;
            } POWER_DEVICE_TIMEOUTS, *PPOWER_DEVICE_TIMEOUTS;
#pragma pack(push)
#pragma pack(2)
            typedef struct _IMAGE_DOS_HEADER {
                WORD   e_magic;
                WORD   e_cblp;
                WORD   e_cp;
                WORD   e_crlc;
                WORD   e_cparhdr;
                WORD   e_minalloc;
                WORD   e_maxalloc;
                WORD   e_ss;
                WORD   e_sp;
                WORD   e_csum;
                WORD   e_ip;
                WORD   e_cs;
                WORD   e_lfarlc;
                WORD   e_ovno;
                WORD   e_res[4];
                WORD   e_oemid;
                WORD   e_oeminfo;
                WORD   e_res2[10];
                LONG   e_lfanew;
            } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
            typedef struct _IMAGE_OS2_HEADER {
                WORD   ne_magic;
                CHAR   ne_ver;
                CHAR   ne_rev;
                WORD   ne_enttab;
                WORD   ne_cbenttab;
                LONG   ne_crc;
                WORD   ne_flags;
                WORD   ne_autodata;
                WORD   ne_heap;
                WORD   ne_stack;
                LONG   ne_csip;
                LONG   ne_sssp;
                WORD   ne_cseg;
                WORD   ne_cmod;
                WORD   ne_cbnrestab;
                WORD   ne_segtab;
                WORD   ne_rsrctab;
                WORD   ne_restab;
                WORD   ne_modtab;
                WORD   ne_imptab;
                LONG   ne_nrestab;
                WORD   ne_cmovent;
                WORD   ne_align;
                WORD   ne_cres;
                BYTE   ne_exetyp;
                BYTE   ne_flagsothers;
                WORD   ne_pretthunks;
                WORD   ne_psegrefbytes;
                WORD   ne_swaparea;
                WORD   ne_expver;
            } IMAGE_OS2_HEADER, *PIMAGE_OS2_HEADER;
            typedef struct _IMAGE_VXD_HEADER {
                WORD   e32_magic;
                BYTE   e32_border;
                BYTE   e32_worder;
                DWORD  e32_level;
                WORD   e32_cpu;
                WORD   e32_os;
                DWORD  e32_ver;
                DWORD  e32_mflags;
                DWORD  e32_mpages;
                DWORD  e32_startobj;
                DWORD  e32_eip;
                DWORD  e32_stackobj;
                DWORD  e32_esp;
                DWORD  e32_pagesize;
                DWORD  e32_lastpagesize;
                DWORD  e32_fixupsize;
                DWORD  e32_fixupsum;
                DWORD  e32_ldrsize;
                DWORD  e32_ldrsum;
                DWORD  e32_objtab;
                DWORD  e32_objcnt;
                DWORD  e32_objmap;
                DWORD  e32_itermap;
                DWORD  e32_rsrctab;
                DWORD  e32_rsrccnt;
                DWORD  e32_restab;
                DWORD  e32_enttab;
                DWORD  e32_dirtab;
                DWORD  e32_dircnt;
                DWORD  e32_fpagetab;
                DWORD  e32_frectab;
                DWORD  e32_impmod;
                DWORD  e32_impmodcnt;
                DWORD  e32_impproc;
                DWORD  e32_pagesum;
                DWORD  e32_datapage;
                DWORD  e32_preload;
                DWORD  e32_nrestab;
                DWORD  e32_cbnrestab;
                DWORD  e32_nressum;
                DWORD  e32_autodata;
                DWORD  e32_debuginfo;
                DWORD  e32_debuglen;
                DWORD  e32_instpreload;
                DWORD  e32_instdemand;
                DWORD  e32_heapsize;
                BYTE   e32_res3[12];
                DWORD  e32_winresoff;
                DWORD  e32_winreslen;
                WORD   e32_devid;
                WORD   e32_ddkver;
            } IMAGE_VXD_HEADER, *PIMAGE_VXD_HEADER;
#pragma pack(pop)
            typedef struct _IMAGE_FILE_HEADER {
                WORD    Machine;
                WORD    NumberOfSections;
                DWORD   TimeDateStamp;
                DWORD   PointerToSymbolTable;
                DWORD   NumberOfSymbols;
                WORD    SizeOfOptionalHeader;
                WORD    Characteristics;
            } IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
            typedef struct _IMAGE_DATA_DIRECTORY {
                DWORD   VirtualAddress;
                DWORD   Size;
            } IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
            typedef struct _IMAGE_OPTIONAL_HEADER {
                WORD    Magic;
                BYTE    MajorLinkerVersion;
                BYTE    MinorLinkerVersion;
                DWORD   SizeOfCode;
                DWORD   SizeOfInitializedData;
                DWORD   SizeOfUninitializedData;
                DWORD   AddressOfEntryPoint;
                DWORD   BaseOfCode;
                DWORD   BaseOfData;

                DWORD   ImageBase;
                DWORD   SectionAlignment;
                DWORD   FileAlignment;
                WORD    MajorOperatingSystemVersion;
                WORD    MinorOperatingSystemVersion;
                WORD    MajorImageVersion;
                WORD    MinorImageVersion;
                WORD    MajorSubsystemVersion;
                WORD    MinorSubsystemVersion;
                DWORD   Win32VersionValue;
                DWORD   SizeOfImage;
                DWORD   SizeOfHeaders;
                DWORD   CheckSum;
                WORD    Subsystem;
                WORD    DllCharacteristics;
                DWORD   SizeOfStackReserve;
                DWORD   SizeOfStackCommit;
                DWORD   SizeOfHeapReserve;
                DWORD   SizeOfHeapCommit;
                DWORD   LoaderFlags;
                DWORD   NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            } IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;
            typedef struct _IMAGE_ROM_OPTIONAL_HEADER {
                WORD   Magic;
                BYTE   MajorLinkerVersion;
                BYTE   MinorLinkerVersion;
                DWORD  SizeOfCode;
                DWORD  SizeOfInitializedData;
                DWORD  SizeOfUninitializedData;
                DWORD  AddressOfEntryPoint;
                DWORD  BaseOfCode;
                DWORD  BaseOfData;
                DWORD  BaseOfBss;
                DWORD  GprMask;
                DWORD  CprMask[4];
                DWORD  GpValue;
            } IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;
            typedef struct _IMAGE_OPTIONAL_HEADER64 {
                WORD        Magic;
                BYTE        MajorLinkerVersion;
                BYTE        MinorLinkerVersion;
                DWORD       SizeOfCode;
                DWORD       SizeOfInitializedData;
                DWORD       SizeOfUninitializedData;
                DWORD       AddressOfEntryPoint;
                DWORD       BaseOfCode;
                ULONGLONG   ImageBase;
                DWORD       SectionAlignment;
                DWORD       FileAlignment;
                WORD        MajorOperatingSystemVersion;
                WORD        MinorOperatingSystemVersion;
                WORD        MajorImageVersion;
                WORD        MinorImageVersion;
                WORD        MajorSubsystemVersion;
                WORD        MinorSubsystemVersion;
                DWORD       Win32VersionValue;
                DWORD       SizeOfImage;
                DWORD       SizeOfHeaders;
                DWORD       CheckSum;
                WORD        Subsystem;
                WORD        DllCharacteristics;
                ULONGLONG   SizeOfStackReserve;
                ULONGLONG   SizeOfStackCommit;
                ULONGLONG   SizeOfHeapReserve;
                ULONGLONG   SizeOfHeapCommit;
                DWORD       LoaderFlags;
                DWORD       NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            } IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;
            typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;
            typedef PIMAGE_OPTIONAL_HEADER32            PIMAGE_OPTIONAL_HEADER;
            typedef struct _IMAGE_NT_HEADERS64 {
                DWORD Signature;
                IMAGE_FILE_HEADER FileHeader;
                IMAGE_OPTIONAL_HEADER64 OptionalHeader;
            } IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;
            typedef struct _IMAGE_NT_HEADERS {
                DWORD Signature;
                IMAGE_FILE_HEADER FileHeader;
                IMAGE_OPTIONAL_HEADER32 OptionalHeader;
            } IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;
            typedef struct _IMAGE_ROM_HEADERS {
                IMAGE_FILE_HEADER FileHeader;
                IMAGE_ROM_OPTIONAL_HEADER OptionalHeader;
            } IMAGE_ROM_HEADERS, *PIMAGE_ROM_HEADERS;
            typedef IMAGE_NT_HEADERS32                  IMAGE_NT_HEADERS;
            typedef PIMAGE_NT_HEADERS32                 PIMAGE_NT_HEADERS;
            typedef struct _IMAGE_SECTION_HEADER {
                BYTE    Name[8];
                union {
                    DWORD   PhysicalAddress;
                    DWORD   VirtualSize;
                } Misc;
                DWORD   VirtualAddress;
                DWORD   SizeOfRawData;
                DWORD   PointerToRawData;
                DWORD   PointerToRelocations;
                DWORD   PointerToLinenumbers;
                WORD    NumberOfRelocations;
                WORD    NumberOfLinenumbers;
                DWORD   Characteristics;
            } IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
#pragma pack(push)
#pragma pack(2)
            typedef struct _IMAGE_SYMBOL {
                union {
                    BYTE    ShortName[8];
                    struct {
                        DWORD   Short;
                        DWORD   Long;
                    } Name;
                    PBYTE   LongName[2];
                } N;
                DWORD   Value;
                SHORT   SectionNumber;
                WORD    Type;
                BYTE    StorageClass;
                BYTE    NumberOfAuxSymbols;
            } IMAGE_SYMBOL;
            typedef IMAGE_SYMBOL __unaligned* PIMAGE_SYMBOL;
            typedef union _IMAGE_AUX_SYMBOL {
                struct {
                    DWORD    TagIndex;
                    union {
                        struct {
                            WORD    Linenumber;
                            WORD    Size;
                        } LnSz;
                        DWORD    TotalSize;
                    } Misc;
                    union {
                        struct {
                            DWORD    PointerToLinenumber;
                            DWORD    PointerToNextFunction;
                        } Function;
                        struct {
                            WORD     Dimension[4];
                        } Array;
                    } FcnAry;
                    WORD    TvIndex;
                } Sym;
                struct {
                    BYTE    Name[18];
                } File;
                struct {
                    DWORD   Length;
                    WORD    NumberOfRelocations;
                    WORD    NumberOfLinenumbers;
                    DWORD   CheckSum;
                    SHORT   Number;
                    BYTE    Selection;
                } Section;
            } IMAGE_AUX_SYMBOL;
            typedef IMAGE_AUX_SYMBOL __unaligned* PIMAGE_AUX_SYMBOL;
            typedef struct _IMAGE_RELOCATION {
                union {
                    DWORD   VirtualAddress;
                    DWORD   RelocCount;
                };
                DWORD   SymbolTableIndex;
                WORD    Type;
            } IMAGE_RELOCATION;
            typedef IMAGE_RELOCATION __unaligned* PIMAGE_RELOCATION;
            typedef struct _IMAGE_LINENUMBER {
                union {
                    DWORD   SymbolTableIndex;
                    DWORD   VirtualAddress;
                } Type;
                WORD    Linenumber;
            } IMAGE_LINENUMBER;
            typedef IMAGE_LINENUMBER __unaligned* PIMAGE_LINENUMBER;
#pragma pack(pop)
            typedef struct _IMAGE_BASE_RELOCATION {
                DWORD   VirtualAddress;
                DWORD   SizeOfBlock;
            } IMAGE_BASE_RELOCATION;
            typedef IMAGE_BASE_RELOCATION __unaligned* PIMAGE_BASE_RELOCATION;
            typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER {
                BYTE     Name[16];
                BYTE     Date[12];
                BYTE     UserID[6];
                BYTE     GroupID[6];
                BYTE     Mode[8];
                BYTE     Size[10];
                BYTE     EndHeader[2];
            } IMAGE_ARCHIVE_MEMBER_HEADER, *PIMAGE_ARCHIVE_MEMBER_HEADER;
            typedef struct _IMAGE_EXPORT_DIRECTORY {
                DWORD   Characteristics;
                DWORD   TimeDateStamp;
                WORD    MajorVersion;
                WORD    MinorVersion;
                DWORD   Name;
                DWORD   Base;
                DWORD   NumberOfFunctions;
                DWORD   NumberOfNames;
                DWORD   AddressOfFunctions;
                DWORD   AddressOfNames;
                DWORD   AddressOfNameOrdinals;
            } IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
            typedef struct _IMAGE_IMPORT_BY_NAME {
                WORD    Hint;
                BYTE    Name[1];
            } IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
#pragma pack(push)
#pragma pack(8)
            typedef struct _IMAGE_THUNK_DATA64 {
                union {
                    PBYTE  ForwarderString;
                    PDWORD Function;
                    ULONGLONG Ordinal;
                    PIMAGE_IMPORT_BY_NAME  AddressOfData;
                } u1;
            } IMAGE_THUNK_DATA64;
            typedef IMAGE_THUNK_DATA64* PIMAGE_THUNK_DATA64;
#pragma pack(pop)
            typedef struct _IMAGE_THUNK_DATA32 {
                union {
                    PBYTE  ForwarderString;
                    PDWORD Function;
                    DWORD Ordinal;
                    PIMAGE_IMPORT_BY_NAME  AddressOfData;
                } u1;
            } IMAGE_THUNK_DATA32;
            typedef IMAGE_THUNK_DATA32* PIMAGE_THUNK_DATA32;
            typedef void
            (__cdecl* PIMAGE_TLS_CALLBACK)(
                PVOID DllHandle,
                DWORD Reason,
                PVOID Reserved
            );
            typedef struct _IMAGE_TLS_DIRECTORY64 {
                ULONGLONG   StartAddressOfRawData;
                ULONGLONG   EndAddressOfRawData;
                PDWORD  AddressOfIndex;
                PIMAGE_TLS_CALLBACK* AddressOfCallBacks;
                DWORD   SizeOfZeroFill;
                DWORD   Characteristics;
            } IMAGE_TLS_DIRECTORY64;
            typedef IMAGE_TLS_DIRECTORY64* PIMAGE_TLS_DIRECTORY64;
            typedef struct _IMAGE_TLS_DIRECTORY32 {
                DWORD   StartAddressOfRawData;
                DWORD   EndAddressOfRawData;
                PDWORD  AddressOfIndex;
                PIMAGE_TLS_CALLBACK* AddressOfCallBacks;
                DWORD   SizeOfZeroFill;
                DWORD   Characteristics;
            } IMAGE_TLS_DIRECTORY32;
            typedef IMAGE_TLS_DIRECTORY32* PIMAGE_TLS_DIRECTORY32;
            typedef IMAGE_THUNK_DATA32              IMAGE_THUNK_DATA;
            typedef PIMAGE_THUNK_DATA32             PIMAGE_THUNK_DATA;
            typedef IMAGE_TLS_DIRECTORY32           IMAGE_TLS_DIRECTORY;
            typedef PIMAGE_TLS_DIRECTORY32          PIMAGE_TLS_DIRECTORY;
            typedef struct _IMAGE_IMPORT_DESCRIPTOR {
                union {
                    DWORD   Characteristics;
                    DWORD   OriginalFirstThunk;
                };
                DWORD   TimeDateStamp;


                DWORD   ForwarderChain;
                DWORD   Name;
                DWORD   FirstThunk;
            } IMAGE_IMPORT_DESCRIPTOR;
            typedef IMAGE_IMPORT_DESCRIPTOR __unaligned* PIMAGE_IMPORT_DESCRIPTOR;
            typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR {
                DWORD   TimeDateStamp;
                WORD    OffsetModuleName;
                WORD    NumberOfModuleForwarderRefs;
            } IMAGE_BOUND_IMPORT_DESCRIPTOR, *PIMAGE_BOUND_IMPORT_DESCRIPTOR;
            typedef struct _IMAGE_BOUND_FORWARDER_REF {
                DWORD   TimeDateStamp;
                WORD    OffsetModuleName;
                WORD    Reserved;
            } IMAGE_BOUND_FORWARDER_REF, *PIMAGE_BOUND_FORWARDER_REF;
            typedef struct _IMAGE_STUB_DIRECTORY {
                DWORD   SecondaryImportAddressTable;
                WORD    ExpectedISA[2];
                DWORD   StubAddressTable[2];
            } IMAGE_STUB_DIRECTORY, *PIMAGE_STUB_DIRECTORY;
            typedef struct _IMAGE_RESOURCE_DIRECTORY {
                DWORD   Characteristics;
                DWORD   TimeDateStamp;
                WORD    MajorVersion;
                WORD    MinorVersion;
                WORD    NumberOfNamedEntries;
                WORD    NumberOfIdEntries;
            } IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;
            typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
                union {
                    struct {
                        DWORD NameOffset: 31;
                        DWORD NameIsString: 1;
                    };
                    DWORD   Name;
                    WORD    Id;
                };
                union {
                    DWORD   OffsetToData;
                    struct {
                        DWORD   OffsetToDirectory: 31;
                        DWORD   DataIsDirectory: 1;
                    };
                };
            } IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;
            typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
                WORD    Length;
                CHAR    NameString[ 1 ];
            } IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;
            typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
                WORD    Length;
                WCHAR   NameString[ 1 ];
            } IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;
            typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
                DWORD   OffsetToData;
                DWORD   Size;
                DWORD   CodePage;
                DWORD   Reserved;
            } IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;
            typedef struct {
                DWORD   Size;
                DWORD   TimeDateStamp;
                WORD    MajorVersion;
                WORD    MinorVersion;
                DWORD   GlobalFlagsClear;
                DWORD   GlobalFlagsSet;
                DWORD   CriticalSectionDefaultTimeout;
                DWORD   DeCommitFreeBlockThreshold;
                DWORD   DeCommitTotalFreeThreshold;
                DWORD   LockPrefixTable;
                DWORD   MaximumAllocationSize;
                DWORD   VirtualMemoryThreshold;
                DWORD   ProcessHeapFlags;
                DWORD   ProcessAffinityMask;
                WORD    CSDVersion;
                WORD    Reserved1;
                DWORD   EditList;
                DWORD   SecurityCookie;
                DWORD   SEHandlerTable;
                DWORD   SEHandlerCount;
            } IMAGE_LOAD_CONFIG_DIRECTORY32, *PIMAGE_LOAD_CONFIG_DIRECTORY32;
            typedef struct {
                DWORD      Size;
                DWORD      TimeDateStamp;
                WORD       MajorVersion;
                WORD       MinorVersion;
                DWORD      GlobalFlagsClear;
                DWORD      GlobalFlagsSet;
                DWORD      CriticalSectionDefaultTimeout;
                ULONGLONG  DeCommitFreeBlockThreshold;
                ULONGLONG  DeCommitTotalFreeThreshold;
                ULONGLONG  LockPrefixTable;
                ULONGLONG  MaximumAllocationSize;
                ULONGLONG  VirtualMemoryThreshold;
                ULONGLONG  ProcessAffinityMask;
                DWORD      ProcessHeapFlags;
                WORD       CSDVersion;
                WORD       Reserved1;
                ULONGLONG  EditList;
                ULONGLONG  SecurityCookie;
                ULONGLONG  SEHandlerTable;
                ULONGLONG  SEHandlerCount;
            } IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;
            typedef IMAGE_LOAD_CONFIG_DIRECTORY32     IMAGE_LOAD_CONFIG_DIRECTORY;
            typedef PIMAGE_LOAD_CONFIG_DIRECTORY32    PIMAGE_LOAD_CONFIG_DIRECTORY;
            typedef struct _IMAGE_IA64_RUNTIME_FUNCTION_ENTRY {
                DWORD BeginAddress;
                DWORD EndAddress;
                DWORD UnwindInfoAddress;
            } IMAGE_IA64_RUNTIME_FUNCTION_ENTRY, *PIMAGE_IA64_RUNTIME_FUNCTION_ENTRY;
            typedef struct _IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY {
                DWORD BeginAddress;
                DWORD EndAddress;
                DWORD ExceptionHandler;
                DWORD HandlerData;
                DWORD PrologEndAddress;
            } IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY, *PIMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY;
            typedef struct _IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY {
                ULONGLONG BeginAddress;
                ULONGLONG EndAddress;
                ULONGLONG ExceptionHandler;
                ULONGLONG HandlerData;
                ULONGLONG PrologEndAddress;
            } IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY, *PIMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY;
            typedef  IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY  IMAGE_AXP64_RUNTIME_FUNCTION_ENTRY;
            typedef PIMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY PIMAGE_AXP64_RUNTIME_FUNCTION_ENTRY;
            typedef struct _IMAGE_CE_RUNTIME_FUNCTION_ENTRY {
                DWORD FuncStart;
                DWORD PrologLen : 8;
                DWORD FuncLen : 22;
                DWORD ThirtyTwoBit : 1;
                DWORD ExceptionFlag : 1;
            } IMAGE_CE_RUNTIME_FUNCTION_ENTRY, * PIMAGE_CE_RUNTIME_FUNCTION_ENTRY;
            typedef  IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY  IMAGE_RUNTIME_FUNCTION_ENTRY;
            typedef PIMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY PIMAGE_RUNTIME_FUNCTION_ENTRY;
            typedef struct _IMAGE_DEBUG_DIRECTORY {
                DWORD   Characteristics;
                DWORD   TimeDateStamp;
                WORD    MajorVersion;
                WORD    MinorVersion;
                DWORD   Type;
                DWORD   SizeOfData;
                DWORD   AddressOfRawData;
                DWORD   PointerToRawData;
            } IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;
            typedef struct _IMAGE_COFF_SYMBOLS_HEADER {
                DWORD   NumberOfSymbols;
                DWORD   LvaToFirstSymbol;
                DWORD   NumberOfLinenumbers;
                DWORD   LvaToFirstLinenumber;
                DWORD   RvaToFirstByteOfCode;
                DWORD   RvaToLastByteOfCode;
                DWORD   RvaToFirstByteOfData;
                DWORD   RvaToLastByteOfData;
            } IMAGE_COFF_SYMBOLS_HEADER, *PIMAGE_COFF_SYMBOLS_HEADER;
            typedef struct _FPO_DATA {
                DWORD       ulOffStart;
                DWORD       cbProcSize;
                DWORD       cdwLocals;
                WORD        cdwParams;
                WORD        cbProlog : 8;
                WORD        cbRegs   : 3;
                WORD        fHasSEH  : 1;
                WORD        fUseBP   : 1;
                WORD        reserved : 1;
                WORD        cbFrame  : 2;
            } FPO_DATA, *PFPO_DATA;
            typedef struct _IMAGE_DEBUG_MISC {
                DWORD       DataType;
                DWORD       Length;

                BOOLEAN     Unicode;
                BYTE        Reserved[ 3 ];
                BYTE        Data[ 1 ];
            } IMAGE_DEBUG_MISC, *PIMAGE_DEBUG_MISC;
            typedef struct _IMAGE_FUNCTION_ENTRY {
                DWORD   StartingAddress;
                DWORD   EndingAddress;
                DWORD   EndOfPrologue;
            } IMAGE_FUNCTION_ENTRY, *PIMAGE_FUNCTION_ENTRY;
            typedef struct _IMAGE_FUNCTION_ENTRY64 {
                ULONGLONG   StartingAddress;
                ULONGLONG   EndingAddress;
                ULONGLONG   EndOfPrologue;
            } IMAGE_FUNCTION_ENTRY64, *PIMAGE_FUNCTION_ENTRY64;
            typedef struct _IMAGE_SEPARATE_DEBUG_HEADER {
                WORD        Signature;
                WORD        Flags;
                WORD        Machine;
                WORD        Characteristics;
                DWORD       TimeDateStamp;
                DWORD       CheckSum;
                DWORD       ImageBase;
                DWORD       SizeOfImage;
                DWORD       NumberOfSections;
                DWORD       ExportedNamesSize;
                DWORD       DebugDirectorySize;
                DWORD       SectionAlignment;
                DWORD       Reserved[2];
            } IMAGE_SEPARATE_DEBUG_HEADER, *PIMAGE_SEPARATE_DEBUG_HEADER;

            typedef struct _ImageArchitectureHeader {
                unsigned int AmaskValue: 1;
                int : 7;
                unsigned int AmaskShift: 8;
                int : 16;
                DWORD FirstEntryRVA;
            } IMAGE_ARCHITECTURE_HEADER, *PIMAGE_ARCHITECTURE_HEADER;
            typedef struct _ImageArchitectureEntry {
                DWORD FixupInstRVA;
                DWORD NewInst;
            } IMAGE_ARCHITECTURE_ENTRY, *PIMAGE_ARCHITECTURE_ENTRY;
#pragma pack(pop)
            typedef struct IMPORT_OBJECT_HEADER {
                WORD    Sig1;
                WORD    Sig2;
                WORD    Version;
                WORD    Machine;
                DWORD   TimeDateStamp;
                DWORD   SizeOfData;
                union {
                    WORD    Ordinal;
                    WORD    Hint;
                };
                WORD    Type : 2;
                WORD    NameType : 3;
                WORD    Reserved : 11;
            } IMPORT_OBJECT_HEADER;
            typedef enum IMPORT_OBJECT_TYPE {
                IMPORT_OBJECT_CODE = 0,
                IMPORT_OBJECT_DATA = 1,
                IMPORT_OBJECT_CONST = 2,
            } IMPORT_OBJECT_TYPE;
            typedef enum IMPORT_OBJECT_NAME_TYPE {
                IMPORT_OBJECT_ORDINAL = 0,
                IMPORT_OBJECT_NAME = 1,
                IMPORT_OBJECT_NAME_NO_PREFIX = 2,
                IMPORT_OBJECT_NAME_UNDECORATE = 3,

            } IMPORT_OBJECT_NAME_TYPE;

            extern "C" {
                errno_t  __cdecl memcpy_s(void* _Dst, rsize_t _DstSize, const void* _Src, rsize_t _MaxCount);
                errno_t __cdecl _strset_s(char* _Dst, size_t _DstSize, int _Value);
                errno_t __cdecl strcpy_s(char* _Dst, rsize_t _DstSize, const char* _Src);
                errno_t __cdecl strcat_s(char* _Dst, rsize_t _DstSize, const char* _Src);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "strcat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl strcat(char* _Dest, const char* _Source);
                size_t  __cdecl strnlen(const char* _Str, size_t _MaxCount);
                static __inline   size_t  __cdecl strnlen_s(const char* _Str, size_t _MaxCount) {
                    return strnlen(_Str, _MaxCount);
                }
                errno_t __cdecl memmove_s(void* _Dst, rsize_t _DstSize, const void* _Src, rsize_t _MaxCount);
                errno_t __cdecl _strlwr_s(char* _Str, size_t _Size);
                errno_t __cdecl strncat_s(char* _Dst, rsize_t _DstSize, const char* _Src, rsize_t _MaxCount);
                errno_t __cdecl strncpy_s(char* _Dst, rsize_t _DstSize, const char* _Src, rsize_t _MaxCount);
                errno_t __cdecl _strnset_s(char* _Str, size_t _Size, int _Val, size_t _MaxCount);
                char*  __cdecl strtok_s(char* _Str, const char* _Delim, char** _Context);
                errno_t __cdecl _strupr_s(char* _Str, size_t _Size);
                __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))
                char* __cdecl _strupr(char* _String);
                errno_t __cdecl wcscat_s(wchar_t* _Dst, rsize_t _DstSize, const wchar_t* _Src);
                errno_t __cdecl wcscpy_s(wchar_t* _Dst, rsize_t _DstSize, const wchar_t* _Src);
                size_t __cdecl wcsnlen(const wchar_t* _Src, size_t _MaxCount);
                static __inline   size_t __cdecl wcsnlen_s(const wchar_t* _Src, size_t _MaxCount) {
                    return wcsnlen(_Src, _MaxCount);
                }
                errno_t __cdecl wcsncat_s(wchar_t* _Dst, rsize_t _DstSize, const wchar_t* _Src, rsize_t _MaxCount);
                errno_t __cdecl wcsncpy_s(wchar_t* _Dst, rsize_t _DstSize, const wchar_t* _Src, rsize_t _MaxCount);
                wchar_t* __cdecl wcstok_s(wchar_t* _Str, const wchar_t* _Delim, wchar_t** _Context);
                errno_t __cdecl _wcsnset_s(wchar_t* _Dst, size_t _DstSizeInWords, wchar_t _Val, size_t _MaxCount);
                errno_t __cdecl _wcsset_s(wchar_t* _Str, size_t _SizeInWords, wchar_t _Val);
                errno_t __cdecl _wcslwr_s(wchar_t* _Str, size_t _SizeInWords);
                errno_t __cdecl _wcsupr_s(wchar_t* _Str, size_t _Size);
            }
            SIZE_T
            __cdecl
            RtlCompareMemory(
                const void* Source1,
                const void* Source2,
                SIZE_T Length
            );
            BOOL CeSafeCopyMemory(LPVOID pDst, LPCVOID pSrc, DWORD cbSize);
            __forceinline
            PVOID
            RtlSecureZeroMemory(
                PVOID ptr,
                SIZE_T cnt
            ) {
                volatile char* vptr = (volatile char*)ptr;

                while (cnt) {
                    *vptr = 0;
                    vptr++;
                    cnt--;
                }

                return ptr;
            }
            typedef struct _MESSAGE_RESOURCE_ENTRY {
                WORD   Length;
                WORD   Flags;
                BYTE  Text[ 1 ];
            } MESSAGE_RESOURCE_ENTRY, *PMESSAGE_RESOURCE_ENTRY;
            typedef struct _MESSAGE_RESOURCE_BLOCK {
                DWORD LowId;
                DWORD HighId;
                DWORD OffsetToEntries;
            } MESSAGE_RESOURCE_BLOCK, *PMESSAGE_RESOURCE_BLOCK;
            typedef struct _MESSAGE_RESOURCE_DATA {
                DWORD NumberOfBlocks;
                MESSAGE_RESOURCE_BLOCK Blocks[ 1 ];
            } MESSAGE_RESOURCE_DATA, *PMESSAGE_RESOURCE_DATA;
            typedef struct _RTL_CRITICAL_SECTION_DEBUG {
                WORD   Type;
                WORD   CreatorBackTraceIndex;
                struct _RTL_CRITICAL_SECTION* CriticalSection;
                LIST_ENTRY ProcessLocksList;
                DWORD EntryCount;
                DWORD ContentionCount;
                DWORD Spare[ 2 ];
            } RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;
            typedef struct _RTL_CRITICAL_SECTION {
                PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
                LONG LockCount;
                LONG RecursionCount;
                HANDLE OwningThread;
                HANDLE LockSemaphore;
                DWORD SpinCount;
            } RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
            typedef void (__cdecl* WAITORTIMERCALLBACKFUNC)(PVOID, BOOLEAN);
            typedef void (__cdecl* WORKERCALLBACKFUNC)(PVOID);
            typedef struct _EVENTLOGRECORD {
                DWORD  Length;
                DWORD  Reserved;
                DWORD  RecordNumber;
                DWORD  TimeGenerated;
                DWORD  TimeWritten;
                DWORD  EventID;
                WORD   EventType;
                WORD   NumStrings;
                WORD   EventCategory;
                WORD   ReservedFlags;
                DWORD  ClosingRecordNumber;
                DWORD  StringOffset;
                DWORD  UserSidLength;
                DWORD  UserSidOffset;
                DWORD  DataLength;
                DWORD  DataOffset;
            } EVENTLOGRECORD, *PEVENTLOGRECORD;

            typedef struct _EVENTSFORLOGFILE {
                DWORD                   ulSize;
                WCHAR               szLogicalLogFile[256];
                DWORD                       ulNumRecords;
                EVENTLOGRECORD  pEventLogRecords[];
            } EVENTSFORLOGFILE, *PEVENTSFORLOGFILE;
            typedef struct _PACKEDEVENTINFO {
                DWORD               ulSize;
                DWORD               ulNumEventsForLogFile;
                DWORD                               ulOffsets[];
            } PACKEDEVENTINFO, *PPACKEDEVENTINFO;

            typedef enum _CM_SERVICE_NODE_TYPE {
                DriverType               = 0x00000001,
                FileSystemType           = 0x00000002,
                Win32ServiceOwnProcess   = 0x00000010,
                Win32ServiceShareProcess = 0x00000020,
                AdapterType              = 0x00000004,
                RecognizerType           = 0x00000008
            } SERVICE_NODE_TYPE;
            typedef enum _CM_SERVICE_LOAD_TYPE {
                BootLoad    = 0x00000000,
                SystemLoad  = 0x00000001,
                AutoLoad    = 0x00000002,
                DemandLoad  = 0x00000003,
                DisableLoad = 0x00000004
            } SERVICE_LOAD_TYPE;
            typedef enum _CM_ERROR_CONTROL_TYPE {
                IgnoreError   = 0x00000000,
                NormalError   = 0x00000001,
                SevereError   = 0x00000002,
                CriticalError = 0x00000003
            } SERVICE_ERROR_TYPE;
            typedef struct _TAPE_ERASE {
                DWORD Type;
                BOOLEAN Immediate;
            } TAPE_ERASE, *PTAPE_ERASE;
            typedef struct _TAPE_PREPARE {
                DWORD Operation;
                BOOLEAN Immediate;
            } TAPE_PREPARE, *PTAPE_PREPARE;
            typedef struct _TAPE_WRITE_MARKS {
                DWORD Type;
                DWORD Count;
                BOOLEAN Immediate;
            } TAPE_WRITE_MARKS, *PTAPE_WRITE_MARKS;
            typedef struct _TAPE_GET_POSITION {
                DWORD Type;
                DWORD Partition;
                LARGE_INTEGER Offset;
            } TAPE_GET_POSITION, *PTAPE_GET_POSITION;
            typedef struct _TAPE_SET_POSITION {
                DWORD Method;
                DWORD Partition;
                LARGE_INTEGER Offset;
                BOOLEAN Immediate;
            } TAPE_SET_POSITION, *PTAPE_SET_POSITION;
            typedef struct _TAPE_GET_DRIVE_PARAMETERS {
                BOOLEAN ECC;
                BOOLEAN Compression;
                BOOLEAN DataPadding;
                BOOLEAN ReportSetmarks;
                DWORD DefaultBlockSize;
                DWORD MaximumBlockSize;
                DWORD MinimumBlockSize;
                DWORD MaximumPartitionCount;
                DWORD FeaturesLow;
                DWORD FeaturesHigh;
                DWORD EOTWarningZoneSize;
            } TAPE_GET_DRIVE_PARAMETERS, *PTAPE_GET_DRIVE_PARAMETERS;
            typedef struct _TAPE_SET_DRIVE_PARAMETERS {
                BOOLEAN ECC;
                BOOLEAN Compression;
                BOOLEAN DataPadding;
                BOOLEAN ReportSetmarks;
                DWORD EOTWarningZoneSize;
            } TAPE_SET_DRIVE_PARAMETERS, *PTAPE_SET_DRIVE_PARAMETERS;
            typedef struct _TAPE_GET_MEDIA_PARAMETERS {
                LARGE_INTEGER Capacity;
                LARGE_INTEGER Remaining;
                DWORD BlockSize;
                DWORD PartitionCount;
                BOOLEAN WriteProtected;
            } TAPE_GET_MEDIA_PARAMETERS, *PTAPE_GET_MEDIA_PARAMETERS;
            typedef struct _TAPE_SET_MEDIA_PARAMETERS {
                DWORD BlockSize;
            } TAPE_SET_MEDIA_PARAMETERS, *PTAPE_SET_MEDIA_PARAMETERS;
            typedef struct _TAPE_CREATE_PARTITION {
                DWORD Method;
                DWORD Count;
                DWORD Size;
            } TAPE_CREATE_PARTITION, *PTAPE_CREATE_PARTITION;
        }
        typedef UINT_PTR            WPARAM;
        typedef LONG_PTR            LPARAM;
        typedef LONG_PTR            LRESULT;
        struct HWND__ {
            int unused;
        };
        typedef struct HWND__*HWND;
        struct HHOOK__ {
            int unused;
        };
        typedef struct HHOOK__*HHOOK;
        typedef WORD                ATOM;
        typedef HANDLE* SPHANDLE;
        typedef HANDLE* LPHANDLE;
        typedef HANDLE              HGLOBAL;
        typedef HANDLE              HLOCAL;
        typedef HANDLE              GLOBALHANDLE;
        typedef HANDLE              LOCALHANDLE;
        typedef int (__cdecl* FARPROC)();
        typedef int (__cdecl* NEARPROC)();
        typedef int (__cdecl* PROC)();
        typedef void* HGDIOBJ;
        struct HKEY__ {
            int unused;
        };
        typedef struct HKEY__*HKEY;
        typedef HKEY* PHKEY;
        struct HACCEL__ {
            int unused;
        };
        typedef struct HACCEL__*HACCEL;
        struct HBITMAP__ {
            int unused;
        };
        typedef struct HBITMAP__*HBITMAP;
        struct HBRUSH__ {
            int unused;
        };
        typedef struct HBRUSH__*HBRUSH;
        struct HCOLORSPACE__ {
            int unused;
        };
        typedef struct HCOLORSPACE__*HCOLORSPACE;
        struct HDC__ {
            int unused;
        };
        typedef struct HDC__*HDC;
        struct HGLRC__ {
            int unused;
        };
        typedef struct HGLRC__*HGLRC;
        struct HDESK__ {
            int unused;
        };
        typedef struct HDESK__*HDESK;
        struct HENHMETAFILE__ {
            int unused;
        };
        typedef struct HENHMETAFILE__*HENHMETAFILE;
        struct HFONT__ {
            int unused;
        };
        typedef struct HFONT__*HFONT;
        struct HICON__ {
            int unused;
        };
        typedef struct HICON__*HICON;
        struct HMENU__ {
            int unused;
        };
        typedef struct HMENU__*HMENU;
        struct HMETAFILE__ {
            int unused;
        };
        typedef struct HMETAFILE__*HMETAFILE;
        struct HINSTANCE__ {
            int unused;
        };
        typedef struct HINSTANCE__*HINSTANCE;
        typedef HINSTANCE HMODULE;
        struct HPALETTE__ {
            int unused;
        };
        typedef struct HPALETTE__*HPALETTE;
        struct HPEN__ {
            int unused;
        };
        typedef struct HPEN__*HPEN;
        struct HRGN__ {
            int unused;
        };
        typedef struct HRGN__*HRGN;
        struct HRSRC__ {
            int unused;
        };
        typedef struct HRSRC__*HRSRC;
        struct HSTR__ {
            int unused;
        };
        typedef struct HSTR__*HSTR;
        struct HTASK__ {
            int unused;
        };
        typedef struct HTASK__*HTASK;
        struct HWINSTA__ {
            int unused;
        };
        typedef struct HWINSTA__*HWINSTA;
        struct HKL__ {
            int unused;
        };
        typedef struct HKL__*HKL;
        struct HMONITOR__ {
            int unused;
        };
        typedef struct HMONITOR__*HMONITOR;
        struct HWINEVENTHOOK__ {
            int unused;
        };
        typedef struct HWINEVENTHOOK__*HWINEVENTHOOK;
        typedef int HFILE;
        typedef HICON HCURSOR;
        typedef DWORD   COLORREF;
        typedef DWORD* LPCOLORREF;
        typedef struct tagRECT {
            LONG    left;
            LONG    top;
            LONG    right;
            LONG    bottom;
        } RECT, *PRECT, *NPRECT, *LPRECT;
        typedef const RECT* LPCRECT;
        typedef struct _RECTL {
            LONG    left;
            LONG    top;
            LONG    right;
            LONG    bottom;
        } RECTL, *PRECTL, *LPRECTL;
        typedef const RECTL* LPCRECTL;
        typedef struct tagPOINT {
            LONG  x;
            LONG  y;
        } POINT, *PPOINT, *NPPOINT, *LPPOINT;
        typedef struct _POINTL {
            LONG  x;
            LONG  y;
        } POINTL, *PPOINTL;
        typedef struct tagSIZE {
            LONG        cx;
            LONG        cy;
        } SIZE, *PSIZE, *LPSIZE;
        typedef SIZE               SIZEL;
        typedef SIZE* PSIZEL, *LPSIZEL;
        typedef struct tagPOINTS {
            SHORT   x;
            SHORT   y;
        } POINTS, *PPOINTS, *LPPOINTS;
    }
#pragma pack(push,8)
    extern "C" {
        typedef long clock_t;
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
    }
#pragma pack(pop)
    typedef void* _HFILE;
    typedef int (__cdecl* _CRT_REPORT_HOOK)(int, char*, int*);

    DWORD __GetUserKData(DWORD dwOfst);
    BOOL __cdecl EventModify(HANDLE hEvent, DWORD func);
    extern "C" {
        void __emit(unsigned const __int32);
    }
    _inline BOOL PulseEvent(HANDLE h) {
        return EventModify(h, 1);
    }
    _inline BOOL ResetEvent(HANDLE h) {
        return EventModify(h, 2);
    }
    _inline BOOL SetEvent(HANDLE h) {
        return EventModify(h, 3);
    }
    _inline HANDLE GetCurrentThread(void) {
        return ((HANDLE)(1 + 64));
    }
    _inline HANDLE GetCurrentProcess(void) {
        return ((HANDLE)(2 + 64));
    }
    _inline HANDLE GetCurrentToken(void) {
        return ((HANDLE)(3 + 64));
    }
    _inline DWORD GetCurrentThreadId(void) {
        return __GetUserKData(0x004 + (1 * sizeof(HANDLE)));
    }
    _inline DWORD GetCurrentProcessId(void) {
        return __GetUserKData(0x004 + (2 * sizeof(HANDLE)));
    }
    DWORD __cdecl TlsCall(DWORD p1, DWORD p2);
    _inline DWORD __cdecl TlsAlloc(void) {
        return TlsCall(0, 0);
    }
    _inline BOOL __cdecl TlsFree(DWORD dwTlsIndex) {
        return TlsCall(1, dwTlsIndex);
    }
    _inline LPVOID LockResource(HGLOBAL hResData) {
        return ((LPVOID)hResData);
    }
    DWORD CeGetCurrentTrust(void);
    DWORD CeGetCallerTrust(void);
    DWORD CeGetProcessTrust(HANDLE hProc);

    _CRT_REPORT_HOOK __cdecl _CrtSetReportHook
    (
        [SA_Pre(Null=SA_Maybe)]
        [SA_Pre(Deref=1, Valid=SA_Yes)]
        [SA_Pre(Deref=1, Access=SA_Read)]
        _CRT_REPORT_HOOK
    );
    _CRT_REPORT_HOOK __cdecl _CrtGetReportHook
    (
        void
    );
    int __cdecl _CrtSetReportMode(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int
    );
    _HFILE __cdecl _CrtSetReportFile(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] _HFILE
    );
    int __cdecl _CrtDbgReportW(
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*,
        [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] int,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*,
        [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*,
        ...);

    void __cdecl _invalid_parameter([SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, [SA_Pre(Null=SA_Maybe, NullTerminated=SA_Yes)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] const wchar_t*, unsigned int, uintptr_t);
    __int64   __cdecl _wcstoi64(const wchar_t*, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] [SA_Pre(Deref=1, Null=SA_No)] [SA_Pre(Deref=2, Valid=SA_Yes)] wchar_t**, int);
    unsigned __int64  __cdecl _wcstoui64(const wchar_t*, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1, Valid=SA_Yes, Access=SA_Read)] [SA_Pre(Deref=1, Null=SA_No)] [SA_Pre(Deref=2, Valid=SA_Yes)] wchar_t**, int);
    errno_t __cdecl _gmtime64_s(struct tm* ptm, const __time64_t*);
    errno_t __cdecl _localtime64_s(struct tm* ptm, const __time64_t*);
    size_t __cdecl wcsftime(wchar_t*, size_t, const wchar_t*, const struct tm*);
    __time64_t __cdecl _mktime64(struct tm*);
    __time64_t __cdecl _time64(__time64_t*);
    errno_t __cdecl _i64toa_s(__int64, [SA_Pre(Null=SA_No, WritableElements="_SizeInChars")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char*, size_t _SizeInChars, int);
    errno_t __cdecl _i64tow_s(__int64, [SA_Pre(Null=SA_No, WritableElements="_SizeInChars")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t _SizeInChars, int);
    __int64 __cdecl _strtoi64(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No)] [SA_Post(Deref=2, Valid=SA_Yes)] char**, int);
    unsigned __int64 __cdecl _strtoui64(const char*, [SA_Pre(Null=SA_Maybe, WritableElementsConst=1)] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Pre(Deref=2, Valid=SA_No)] [SA_Post(Deref=1, Null=SA_No)] [SA_Post(Deref=2, Valid=SA_Yes)]  char**, int);
    errno_t __cdecl _ui64toa_s(unsigned __int64, [SA_Pre(Null=SA_No, WritableElements="_Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char*, size_t _Size, int);
    errno_t __cdecl _ui64tow_s(unsigned __int64, [SA_Pre(Null=SA_No, WritableElements="_Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t _Size, int);
    errno_t __cdecl _wmakepath_s([SA_Pre(Null=SA_No, WritableElements="_Size")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t _Size, const wchar_t*, const wchar_t*, const wchar_t*,
                                 const wchar_t*);
    errno_t __cdecl _wsplitpath_s(const wchar_t*, [SA_Pre(Null=SA_Maybe, WritableElements="driveSizeInCharacters")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t driveSizeInCharacters, [SA_Pre(Null=SA_Maybe, WritableElements="dirSizeInCharacters")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*,
                                  size_t dirSizeInCharacters, [SA_Pre(Null=SA_Maybe, WritableElements="nameSizeInCharacters")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t nameSizeInCharacters, [SA_Pre(Null=SA_Maybe, WritableElements="extSizeInCharacters")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t*, size_t extSizeInCharacters);
    __inline errno_t __cdecl wmemcpy_s([SA_Pre(Null=SA_No, WritableElements="_N1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _S1, size_t _N1, const wchar_t* _S2, size_t _N) {
        return memcpy_s(_S1, _N1 * sizeof(wchar_t), _S2, _N * sizeof(wchar_t));
    }
    __inline errno_t __cdecl wmemmove_s([SA_Pre(Null=SA_No, WritableElements="_N1")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="_N")] [SA_Post(Deref=1, Valid=SA_Yes)] wchar_t* _S1, size_t _N1, const wchar_t* _S2, size_t _N) {
        return memmove_s(_S1, _N1 * sizeof(wchar_t), _S2, _N * sizeof(wchar_t));
    }

    int __cdecl swprintf_s(wchar_t* _Dst, size_t _SizeInWords, const wchar_t* _Format, ...);
    int __cdecl vswprintf_s(wchar_t* _Dst, size_t _SizeInWords, const wchar_t* _Format, va_list _ArgList);
    int __cdecl _snwprintf_s(wchar_t* _DstBuf, size_t _DstSizeInWords, size_t _MaxCount, const wchar_t* _Format, ...);
    int __cdecl _vsnwprintf_s(wchar_t* _DstBuf, size_t _DstSizeInWords, size_t _MaxCount, const wchar_t* _Format, va_list _ArgList);
    int __cdecl sscanf_s(const char* _Src, const char* _Format, ...);
    int __cdecl sprintf_s(char* _DstBuf, size_t _DstSize, const char* _Format, ...);
    int __cdecl _snprintf_s(char* _DstBuf, size_t _DstSize, size_t _MaxCount, const char* _Format, ...);
    int __cdecl _vsnprintf_s(char* _DstBuf, size_t _DstSize, size_t _MaxCount, const char* _Format, va_list _ArgList);
    int __cdecl vsprintf_s(char* _DstBuf, size_t _Size, const char* _Format, va_list _ArgList);
    int __cdecl swscanf_s(const wchar_t* _Src, const wchar_t* _Format, ...);
    errno_t __cdecl fopen_s(FILE** _File, const char* _Filename, const char* _Mode);
    int __cdecl fprintf_s(FILE* _File, const char* _Format, ...);
    int __cdecl fscanf_s(FILE* _File, const char* _Format, ...);
    char* __cdecl gets_s(char* _Buf, rsize_t _Size);
    int __cdecl printf_s(const char* _Format, ...);
    int __cdecl scanf_s(const char* _Format, ...);
    int __cdecl vfprintf_s(FILE* _File, const char* _Format, va_list _ArgList);
    int __cdecl vprintf_s(const char* _Format, va_list _ArgList);
    wchar_t* __cdecl _getws_s(wchar_t* _Str, size_t _SizeInWords);
    int __cdecl fwprintf_s(FILE* _File, const wchar_t* _Format, ...);
    int __cdecl wprintf_s(const wchar_t* _Format, ...);
    int __cdecl vfwprintf_s(FILE* _File, const wchar_t* _Format, va_list _ArgList);
    int __cdecl vwprintf_s(const wchar_t* _Format, va_list _ArgList);
    int __cdecl fwscanf_s(FILE* _File, const wchar_t* _Format, ...);
    int __cdecl wscanf_s(const wchar_t* _Format, ...);
    errno_t __cdecl _wfopen_s(FILE** _File, const wchar_t* _Filename, const wchar_t* _Mode);
    errno_t __cdecl clearerr_s(FILE* _File);
    size_t __cdecl fread_s(void* _DstBuf, size_t _DstSize, size_t _ElementSize, size_t _Count, FILE* _File);
    errno_t __cdecl _wfreopen_s(FILE** _File, const wchar_t* _Filename, const wchar_t* _Mode, FILE* _OldFile);
}
extern "C" {
    typedef int mbstate_t;
    typedef wchar_t _Wint_t;
    inline int fwide(FILE*, int _M) {
        return (_M);
    }
    inline int mbsinit(const mbstate_t*) {
        return (1);
    }
    inline const wchar_t* wmemchr(const wchar_t* _S, wchar_t _C, size_t _N) {
        for (; 0 < _N; ++_S, --_N)
            if (*_S == _C)
                return (_S);

        return (0);
    }
    inline int wmemcmp(const wchar_t* _S1, const wchar_t* _S2, size_t _N) {
        for (; 0 < _N; ++_S1, ++_S2, --_N)
            if (*_S1 != *_S2)
                return (*_S1 < *_S2 ? -1 : +1);

        return (0);
    }
    inline wchar_t* wmemcpy(wchar_t* _S1, const wchar_t* _S2, size_t _N) {
        wchar_t* _Su1 = _S1;

        for (; 0 < _N; ++_Su1, ++_S2, --_N)
            *_Su1 = *_S2;

        return (_S1);
    }
    inline wchar_t* wmemmove(wchar_t* _S1, const wchar_t* _S2, size_t _N) {
        wchar_t* _Su1 = _S1;

        if (_S2 < _Su1 && _Su1 < _S2 + _N)
            for (_Su1 += _N, _S2 += _N; 0 < _N; --_N)
                *--_Su1 = *--_S2;
        else
            for (; 0 < _N; --_N)
                *_Su1++ = *_S2++;

        return (_S1);
    }
    inline wchar_t* wmemset(wchar_t* _S, wchar_t _C, size_t _N) {
        wchar_t* _Su = _S;

        for (; 0 < _N; ++_Su, --_N)
            *_Su = _C;

        return (_S);
    }

    extern "C" {
        typedef struct tagVS_FIXEDFILEINFO {
            DWORD   dwSignature;
            DWORD   dwStrucVersion;
            DWORD   dwFileVersionMS;
            DWORD   dwFileVersionLS;
            DWORD   dwProductVersionMS;
            DWORD   dwProductVersionLS;
            DWORD   dwFileFlagsMask;
            DWORD   dwFileFlags;
            DWORD   dwFileOS;
            DWORD   dwFileType;
            DWORD   dwFileSubtype;
            DWORD   dwFileDateMS;
            DWORD   dwFileDateLS;
        } VS_FIXEDFILEINFO;
        DWORD
        __cdecl
        GetFileVersionInfoSizeA(
            LPSTR lptstrFilename,
            LPDWORD lpdwHandle
        );
        DWORD
        __cdecl
        GetFileVersionInfoSizeW(
            LPWSTR lptstrFilename,
            LPDWORD lpdwHandle
        );
        BOOL
        __cdecl
        GetFileVersionInfoA(
            LPSTR lptstrFilename,
            DWORD dwHandle,
            DWORD dwLen,
            LPVOID lpData
        );
        BOOL
        __cdecl
        GetFileVersionInfoW(
            LPWSTR lptstrFilename,
            DWORD dwHandle,
            DWORD dwLen,
            LPVOID lpData
        );
        BOOL
        __cdecl
        VerQueryValueA(
            const LPVOID pBlock,
            LPSTR lpSubBlock,
            LPVOID* lplpBuffer,
            PUINT puLen
        );
        BOOL
        __cdecl
        VerQueryValueW(
            const LPVOID pBlock,
            LPWSTR lpSubBlock,
            LPVOID* lplpBuffer,
            PUINT puLen
        );
    }
    typedef struct _FILETIME {
        DWORD dwLowDateTime;
        DWORD dwHighDateTime;
    } FILETIME, *PFILETIME, *LPFILETIME;
    typedef struct _SYSTEMTIME {
        WORD wYear;
        WORD wMonth;
        WORD wDayOfWeek;
        WORD wDay;
        WORD wHour;
        WORD wMinute;
        WORD wSecond;
        WORD wMilliseconds;
    } SYSTEMTIME, *LPSYSTEMTIME;
    LONG
    __cdecl
    InterlockedIncrement(
        LONG volatile* lpAddend
    );
    LONG
    __cdecl
    InterlockedDecrement(
        LONG volatile* lpAddend
    );
    LONG
    __cdecl
    InterlockedExchange(
        LONG volatile* Target,
        LONG Value
    );
    LONG
    __cdecl
    InterlockedCompareExchange(
        LONG volatile* Target,
        LONG Exchange,
        LONG Comperand
    );
    LONG
    __cdecl
    InterlockedExchangeAdd(
        LONG volatile* lpAddend,
        LONG Value
    );
    DWORD
    __cdecl
    Random();
    int
    __cdecl
    WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd
    );
    LONG __cdecl CompareFileTime(const FILETIME* lpft1, const FILETIME* lpft2);
    BOOL __cdecl FileTimeToSystemTime(const FILETIME* lpft, LPSYSTEMTIME lpst);
    BOOL __cdecl SystemTimeToFileTime(const SYSTEMTIME* lpst, LPFILETIME lpft);
    BOOL __cdecl FileTimeToLocalFileTime(const FILETIME* lpft, LPFILETIME lpftLocal);
    BOOL __cdecl LocalFileTimeToFileTime(const FILETIME* lpftLocal, LPFILETIME lpft);
    void GetCurrentFT(LPFILETIME lpFileTime);
    BOOL
    __cdecl
    DllMain(
        HANDLE hinstDLL,
        DWORD dwReason,
        LPVOID lpvReserved
    );
    FARPROC
    __cdecl
    GetProcAddressW(
        HMODULE hModule,
        LPCWSTR lpProcName
    );
    FARPROC
    __cdecl
    GetProcAddressA(
        HMODULE hModule,
        LPCSTR lpProcName
    );
    int
    __cdecl
    MulDiv(
        int nNumber,
        int nNumerator,
        int nDenominator
    );
    void
    __cdecl
    GetLocalTime(
        LPSYSTEMTIME lpSystemTime
    );
    void
    __cdecl
    GetSystemTime(
        LPSYSTEMTIME lpSystemTime
    );
    void
    __cdecl
    GetSystemTimeAsFileTime(
        LPFILETIME lpSystemTimeAsFileTime
    );
    typedef struct _TIME_ZONE_INFORMATION {
        LONG Bias;
        WCHAR StandardName[ 32 ];
        SYSTEMTIME StandardDate;
        LONG StandardBias;
        WCHAR DaylightName[ 32 ];
        SYSTEMTIME DaylightDate;
        LONG DaylightBias;
    } TIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;
    DWORD
    __cdecl
    GetTimeZoneInformation(
        LPTIME_ZONE_INFORMATION lpTimeZoneInformation
    );
    BOOL
    __cdecl
    SetLocalTime(
        const SYSTEMTIME* lpSystemTime
    );
    BOOL
    __cdecl
    SetSystemTime(
        const SYSTEMTIME* lpSystemTime
    );
    void
    __cdecl
    SetDaylightTime(
        DWORD dst
    );
    BOOL
    __cdecl
    SetTimeZoneInformation(
        const TIME_ZONE_INFORMATION* lpTimeZoneInformation
    );
    LPSTR
    __cdecl
    lstrcatA(
        LPSTR lpString1,
        LPCSTR lpString2
    );
    int
    __cdecl
    lstrcmpA(
        LPCSTR lpString1,
        LPCSTR lpString2
    );
    int
    __cdecl
    lstrcmpW(
        LPCWSTR lpString1,
        LPCWSTR lpString2
    );
    int
    __cdecl
    lstrcmpiA(
        LPCSTR lpString1,
        LPCSTR lpString2
    );
    int
    __cdecl
    lstrcmpiW(
        LPCWSTR lpString1,
        LPCWSTR lpString2
    );
    LPSTR
    __cdecl
    lstrcpyA(
        LPSTR lpString1,
        LPCSTR lpString2
    );
    int
    __cdecl
    lstrlenA(
        LPCSTR lpString
    );
    DWORD
    __cdecl
    CharLowerBuffA(
        LPSTR lpsz,
        DWORD cchLength
    );
    DWORD
    __cdecl
    CharLowerBuffW(
        LPWSTR lpsz,
        DWORD cchLength
    );
    DWORD
    __cdecl
    CharUpperBuffA(
        LPSTR lpsz,
        DWORD cchLength
    );
    DWORD
    __cdecl
    CharUpperBuffW(
        LPWSTR lpsz,
        DWORD cchLength
    );
    LPSTR __cdecl CharLowerA(LPSTR lpsz);
    LPWSTR __cdecl CharLowerW(LPWSTR lpsz);
    LPSTR __cdecl CharUpperA(LPSTR lpsz);
    LPWSTR __cdecl CharUpperW(LPWSTR lpsz);
    LPSTR
    __cdecl
    CharPrevA(
        LPCSTR lpszStart,
        LPCSTR lpszCurrent);
    LPWSTR
    __cdecl
    CharPrevW(
        LPCWSTR lpszStart,
        LPCWSTR lpszCurrent);
    LPSTR
    __cdecl
    CharNextA(
        LPCSTR      lpsz);
    LPWSTR
    __cdecl
    CharNextW(
        LPCWSTR      lpsz);
    int
    __cdecl
    wsprintfA(
        LPSTR,
        LPCSTR,
        ...
    );
    int
    __cdecl
    wsprintfW(
        LPWSTR,
        LPCWSTR,
        ...
    );
    int
    __cdecl
    wvsprintfA(
        LPSTR,
        LPCSTR,
        va_list arglist
    );
    int
    __cdecl
    wvsprintfW(
        LPWSTR,
        LPCWSTR,
        va_list arglist
    );
    typedef struct _MEMORYSTATUS {
        DWORD dwLength;
        DWORD dwMemoryLoad;
        DWORD dwTotalPhys;
        DWORD dwAvailPhys;
        DWORD dwTotalPageFile;
        DWORD dwAvailPageFile;
        DWORD dwTotalVirtual;
        DWORD dwAvailVirtual;
    } MEMORYSTATUS, *LPMEMORYSTATUS;
    void
    __cdecl
    GlobalMemoryStatus(
        LPMEMORYSTATUS lpBuffer
    );
    HLOCAL
    __cdecl
    LocalAlloc(
        UINT fuFlags,
        UINT cbBytes
    );
    HLOCAL
    __cdecl
    LocalAllocTrace(
        UINT fuFlags,
        UINT cbBytes,
        UINT cLineNum,
        LPCWSTR szFilename
    );
    HLOCAL
    __cdecl
    LocalFree(
        HLOCAL hMem
    );
    HLOCAL
    __cdecl
    LocalReAlloc(
        HLOCAL hMem,
        UINT cbBytes,
        UINT fuFlags
    );
    UINT
    __cdecl
    LocalSize(
        HLOCAL hMem
    );
    LPVOID
    __cdecl
    VirtualAlloc(
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD flAllocationType,
        DWORD flProtect
    );
    BOOL
    __cdecl
    VirtualFree(
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD dwFreeType
    );
    LPVOID
    __cdecl
    VirtualAllocEx(
        HANDLE hProcess,
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD flAllocationType,
        DWORD flProtect
    );
    BOOL
    __cdecl
    VirtualFreeEx(
        HANDLE hProcess,
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD dwFreeType
    );
    BOOL
    __cdecl
    VirtualProtect(
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD flNewProtect,
        PDWORD lpflOldProtect
    );
    DWORD
    __cdecl
    VirtualQuery(
        LPCVOID lpAddress,
        PMEMORY_BASIC_INFORMATION lpBuffer,
        DWORD dwLength
    );
    BOOL
    __cdecl
    VirtualProtectEx(
        HANDLE hProcess,
        LPVOID lpAddress,
        DWORD dwSize,
        DWORD flNewProtect,
        PDWORD lpflOldProtect
    );
    DWORD
    __cdecl
    VirtualQueryEx(
        HANDLE hProcess,
        LPCVOID lpAddress,
        PMEMORY_BASIC_INFORMATION lpBuffer,
        DWORD dwLength
    );
    HANDLE __cdecl GetProcessHeap(void);
    HANDLE
    __cdecl
    HeapCreate(
        DWORD flOptions,
        DWORD dwInitialSize,
        DWORD dwMaximumSize
    );
    BOOL
    __cdecl
    HeapDestroy(
        HANDLE hHeap
    );
    LPVOID
    __cdecl
    HeapAlloc(
        HANDLE hHeap,
        DWORD dwFlags,
        DWORD dwBytes
    );
    LPVOID
    __cdecl
    HeapAllocTrace(
        HANDLE hHeap,
        DWORD dwFlags,
        DWORD dwBytes,
        DWORD dwLineNum,
        PCHAR szFilename
    );
    LPVOID
    __cdecl
    HeapReAlloc(
        HANDLE hHeap,
        DWORD dwFlags,
        LPVOID lpMem,
        DWORD dwBytes
    );
    BOOL
    __cdecl
    HeapFree(
        HANDLE hHeap,
        DWORD dwFlags,
        LPVOID lpMem
    );
    DWORD
    __cdecl
    HeapSize(
        HANDLE hHeap,
        DWORD dwFlags,
        LPCVOID lpMem
    );
    BOOL
    __cdecl
    HeapValidate(
        HANDLE hHeap,
        DWORD dwFlags,
        LPCVOID lpMem
    );
    UINT
    __cdecl
    HeapCompact(
        HANDLE hHeap,
        DWORD dwFlags
    );
    typedef HANDLE(__cdecl* PFN_HeapCreate)(DWORD flOptions, DWORD dwInitialSize, DWORD dwMaximumSize);
    typedef BOOL (__cdecl* PFN_HeapDestroy)(HANDLE hHeap);
    typedef LPVOID (__cdecl* PFN_HeapAlloc)(HANDLE hHeap, DWORD dwFlags, DWORD dwBytes);
    typedef LPVOID (__cdecl* PFN_HeapAllocTrace)(HANDLE hHeap, DWORD dwFlags, DWORD dwBytes, DWORD dwLineNum, PCHAR szFileName);
    typedef LPVOID (__cdecl* PFN_HeapReAlloc)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, DWORD dwBytes);
    typedef DWORD (__cdecl* PFN_HeapSize)(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);
    typedef BOOL (__cdecl* PFN_HeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
    typedef UINT(__cdecl* PFN_HeapCompact)(HANDLE hHeap, DWORD dwFlags);
    typedef LPVOID (* PFN_AllocHeapMem)(LPVOID pAddr, DWORD cbSize, DWORD fdwAction, LPDWORD pdwUserData);
    typedef BOOL (* PFN_FreeHeapMem)(LPVOID pAddr, DWORD cbSize, DWORD fdwAction, DWORD dwUserData);
    HANDLE
    __cdecl
    CeHeapCreate(
        DWORD flOptions,
        DWORD dwInitialSize,
        DWORD dwMaximumSize,
        PFN_AllocHeapMem pfnAlloc,
        PFN_FreeHeapMem pfnFree
    );
    void
    __cdecl
    OutputDebugStringA(
        LPCSTR lpOutputString
    );
    void
    __cdecl
    OutputDebugStringW(
        LPCWSTR lpOutputString
    );
    void SignalStarted(DWORD dw);
    BOOL
    __cdecl
    QueryPerformanceCounter(
        LARGE_INTEGER* lpPerformanceCount
    );
    BOOL
    __cdecl
    QueryPerformanceFrequency(
        LARGE_INTEGER* lpFrequency
    );
    typedef struct _OSVERSIONINFOA {
        DWORD dwOSVersionInfoSize;
        DWORD dwMajorVersion;
        DWORD dwMinorVersion;
        DWORD dwBuildNumber;
        DWORD dwPlatformId;
        CHAR   szCSDVersion[ 128 ];
    } OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;
    typedef struct _OSVERSIONINFOW {
        DWORD dwOSVersionInfoSize;
        DWORD dwMajorVersion;
        DWORD dwMinorVersion;
        DWORD dwBuildNumber;
        DWORD dwPlatformId;
        WCHAR  szCSDVersion[ 128 ];
    } OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW;
    typedef OSVERSIONINFOW OSVERSIONINFO;
    typedef POSVERSIONINFOW POSVERSIONINFO;
    typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
    BOOL
    __cdecl
    GetVersionExA(
        LPOSVERSIONINFOA lpVersionInformation
    );
    BOOL
    __cdecl
    GetVersionExW(
        LPOSVERSIONINFOW lpVersionInformation
    );
    typedef struct _SYSTEM_INFO {
        union {
            DWORD dwOemId;
            struct {
                WORD wProcessorArchitecture;
                WORD wReserved;
            };
        };
        DWORD dwPageSize;
        LPVOID lpMinimumApplicationAddress;
        LPVOID lpMaximumApplicationAddress;
        DWORD dwActiveProcessorMask;
        DWORD dwNumberOfProcessors;
        DWORD dwProcessorType;
        DWORD dwAllocationGranularity;
        WORD wProcessorLevel;
        WORD wProcessorRevision;
    } SYSTEM_INFO, *LPSYSTEM_INFO;
    HINSTANCE
    __cdecl
    LoadLibraryA(
        LPCSTR lpLibFileName
    );
    HINSTANCE
    __cdecl
    LoadLibraryW(
        LPCWSTR lpLibFileName
    );
    HMODULE
    __cdecl
    LoadLibraryExA(
        LPCSTR lpLibFileName,
        HANDLE hFile,
        DWORD dwFlags
    );
    HMODULE
    __cdecl
    LoadLibraryExW(
        LPCWSTR lpLibFileName,
        HANDLE hFile,
        DWORD dwFlags
    );
    DWORD
    __cdecl
    GetModuleFileNameA(
        HMODULE hModule,
        LPSTR lpFilename,
        DWORD nSize
    );
    DWORD
    __cdecl
    GetModuleFileNameW(
        HMODULE hModule,
        LPWSTR lpFilename,
        DWORD nSize
    );
    HMODULE
    __cdecl
    GetModuleHandleA(
        LPCSTR lpModuleName
    );
    HMODULE
    __cdecl
    GetModuleHandleW(
        LPCWSTR lpModuleName
    );
    typedef struct MSGQUEUEOPTIONS_OS {
        DWORD dwSize;
        DWORD dwFlags;
        DWORD dwMaxMessages;
        DWORD cbMaxMessage;
        BOOL  bReadAccess;
    } MSGQUEUEOPTIONS, *LPMSGQUEUEOPTIONS, *PMSGQUEUEOPTIONS;
    typedef struct MSGQUEUEINFO {
        DWORD dwSize;
        DWORD dwFlags;
        DWORD dwMaxMessages;
        DWORD cbMaxMessage;
        DWORD dwCurrentMessages;
        DWORD dwMaxQueueMessages;
        WORD  wNumReaders;
        WORD  wNumWriters;
    } MSGQUEUEINFO, *PMSGQUEUEINFO, *LPMSGQUEUEINFO;
    typedef struct _OVERLAPPED {
        DWORD   Internal;
        DWORD   InternalHigh;
        DWORD   Offset;
        DWORD   OffsetHigh;
        HANDLE  hEvent;
    } OVERLAPPED, *LPOVERLAPPED;
    typedef struct _SECURITY_ATTRIBUTES {
        DWORD nLength;
        LPVOID lpSecurityDescriptor;
        BOOL bInheritHandle;
    } SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
    typedef DWORD (__cdecl* LPTHREAD_START_ROUTINE)(LPVOID pvarg);
    typedef LPTHREAD_START_ROUTINE PTHREAD_START_ROUTINE;
    typedef struct _EXCEPTION_DEBUG_INFO {
        EXCEPTION_RECORD ExceptionRecord;
        DWORD dwFirstChance;
    } EXCEPTION_DEBUG_INFO, *LPEXCEPTION_DEBUG_INFO;
    typedef struct _CREATE_THREAD_DEBUG_INFO {
        HANDLE hThread;
        LPVOID lpThreadLocalBase;
        LPTHREAD_START_ROUTINE lpStartAddress;
    } CREATE_THREAD_DEBUG_INFO, *LPCREATE_THREAD_DEBUG_INFO;
    typedef struct _CREATE_PROCESS_DEBUG_INFO {
        HANDLE hFile;
        HANDLE hProcess;
        HANDLE hThread;
        LPVOID lpBaseOfImage;
        DWORD dwDebugInfoFileOffset;
        DWORD nDebugInfoSize;
        LPVOID lpThreadLocalBase;
        LPTHREAD_START_ROUTINE lpStartAddress;
        LPVOID lpImageName;
        WORD fUnicode;
    } CREATE_PROCESS_DEBUG_INFO, *LPCREATE_PROCESS_DEBUG_INFO;
    typedef struct _EXIT_THREAD_DEBUG_INFO {
        DWORD dwExitCode;
    } EXIT_THREAD_DEBUG_INFO, *LPEXIT_THREAD_DEBUG_INFO;
    typedef struct _EXIT_PROCESS_DEBUG_INFO {
        DWORD dwExitCode;
    } EXIT_PROCESS_DEBUG_INFO, *LPEXIT_PROCESS_DEBUG_INFO;
    typedef struct _LOAD_DLL_DEBUG_INFO {
        HANDLE hFile;
        LPVOID lpBaseOfDll;
        DWORD dwDebugInfoFileOffset;
        DWORD nDebugInfoSize;
        LPVOID lpImageName;
        WORD fUnicode;
    } LOAD_DLL_DEBUG_INFO, *LPLOAD_DLL_DEBUG_INFO;
    typedef struct _UNLOAD_DLL_DEBUG_INFO {
        LPVOID lpBaseOfDll;
    } UNLOAD_DLL_DEBUG_INFO, *LPUNLOAD_DLL_DEBUG_INFO;
    typedef struct _OUTPUT_DEBUG_STRING_INFO {
        LPSTR lpDebugStringData;
        WORD fUnicode;
        WORD nDebugStringLength;
    } OUTPUT_DEBUG_STRING_INFO, *LPOUTPUT_DEBUG_STRING_INFO;
    typedef struct _RIP_INFO {
        DWORD dwError;
        DWORD dwType;
    } RIP_INFO, *LPRIP_INFO;
    typedef struct _DEBUG_EVENT {
        DWORD dwDebugEventCode;
        DWORD dwProcessId;
        DWORD dwThreadId;
        union {
            EXCEPTION_DEBUG_INFO Exception;
            CREATE_THREAD_DEBUG_INFO CreateThread;
            CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
            EXIT_THREAD_DEBUG_INFO ExitThread;
            EXIT_PROCESS_DEBUG_INFO ExitProcess;
            LOAD_DLL_DEBUG_INFO LoadDll;
            UNLOAD_DLL_DEBUG_INFO UnloadDll;
            OUTPUT_DEBUG_STRING_INFO DebugString;
            RIP_INFO RipInfo;
        } u;
    } DEBUG_EVENT, *LPDEBUG_EVENT;
    typedef PCONTEXT LPCONTEXT;
    typedef LONG(__cdecl* PVECTORED_EXCEPTION_HANDLER)(
        struct _EXCEPTION_POINTERS* ExceptionInfo
    );
    PVOID
    __cdecl
    AddVectoredExceptionHandler(
        ULONG FirstHandler,
        PVECTORED_EXCEPTION_HANDLER VectoredHandler
    );
    ULONG
    __cdecl
    RemoveVectoredExceptionHandler(
        PVOID VectoredHandlerHandle
    );
    typedef struct CRITICAL_SECTION {
        unsigned int LockCount;
        HANDLE OwnerThread;
        HANDLE hCrit;
        DWORD needtrap;
        DWORD dwContentions;
    } CRITICAL_SECTION, *LPCRITICAL_SECTION;
    void
    __cdecl
    EnterCriticalSection(
        LPCRITICAL_SECTION pcsCriticalSection
    );
    void
    __cdecl
    LeaveCriticalSection(
        LPCRITICAL_SECTION pcsCriticalSection
    );
    void
    __cdecl
    InitializeCriticalSection(
        LPCRITICAL_SECTION pcsCriticalSection
    );
    BOOL
    __cdecl
    TryEnterCriticalSection(
        LPCRITICAL_SECTION lpCriticalSection
    );
    void
    __cdecl
    DeleteCriticalSection(
        LPCRITICAL_SECTION pcsCriticalSection
    );
    BOOL
    __cdecl
    WaitForDebugEvent(
        LPDEBUG_EVENT lpDebugEvent,
        DWORD dwMilliseconds
    );
    BOOL
    __cdecl
    ContinueDebugEvent(
        DWORD dwProcessId,
        DWORD dwThreadId,
        DWORD dwContinueStatus
    );
    BOOL
    __cdecl
    DebugActiveProcess(
        DWORD dwProcessId
    );
    BOOL
    __cdecl
    DebugActiveProcessStop(
        DWORD dwProcessId
    );
    BOOL
    __cdecl
    DebugSetProcessKillOnExit(
        BOOL KillOnExit
    );
    BOOL
    __cdecl
    CheckRemoteDebuggerPresent(
        HANDLE hProcess,
        PBOOL  pbDebuggerPresent
    );
    LPVOID
    __cdecl
    MapViewOfFile(
        HANDLE hFileMappingObject,
        DWORD dwDesiredAccess,
        DWORD dwFileOffsetHigh,
        DWORD dwFileOffsetLow,
        DWORD dwNumberOfBytesToMap
    );
    BOOL
    __cdecl
    UnmapViewOfFile(
        LPCVOID lpBaseAddress
    );
    BOOL
    __cdecl
    FlushViewOfFile(
        LPCVOID lpBaseAddress,
        DWORD dwNumberOfBytesToFlush
    );
    HANDLE
    __cdecl
    CreateFileMappingA(
        HANDLE hFile,
        LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
        DWORD flProtect,
        DWORD dwMaximumSizeHigh,
        DWORD dwMaximumSizeLow,
        LPCSTR lpName
    );
    HANDLE
    __cdecl
    CreateFileMappingW(
        HANDLE hFile,
        LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
        DWORD flProtect,
        DWORD dwMaximumSizeHigh,
        DWORD dwMaximumSizeLow,
        LPCWSTR lpName
    );
    HANDLE
    __cdecl
    CreateFileForMappingW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                          LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                          HANDLE hTemplateFile);
    DWORD
    __cdecl
    GetTempPathA(
        DWORD nBufferLength,
        LPSTR lpBuffer
    );
    DWORD
    __cdecl
    GetTempPathW(
        DWORD nBufferLength,
        LPWSTR lpBuffer
    );
    HANDLE
    __cdecl
    CreateThread(
        LPSECURITY_ATTRIBUTES lpsa,
        DWORD   cbStack,
        LPTHREAD_START_ROUTINE lpStartAddr,
        LPVOID lpvThreadParam,
        DWORD fdwCreate,
        LPDWORD lpIDThread
    );
    void
    __cdecl
    ExitThread(
        DWORD   dwExitCode
    );
    BOOL
    __cdecl
    TerminateThread(
        HANDLE hThread,
        DWORD dwExitCode
    );
    typedef void (__cdecl* LPFIBER_START_ROUTINE)(PVOID);
    LPVOID __cdecl ConvertThreadToFiber(LPVOID lpParameter);
    LPVOID __cdecl CreateFiber(DWORD dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);
    void __cdecl DeleteFiber(LPVOID lpFiber);
    LPVOID __cdecl GetFiberData(void);
    LPVOID GetCurrentFiber(void);
    void __cdecl SwitchToFiber(LPVOID lpFiber);
    LPVOID
    __cdecl
    TlsGetValue(
        DWORD dwTlsIndex
    );
    BOOL
    __cdecl
    TlsSetValue(
        DWORD dwTlsIndex,
        LPVOID lpvTlsValue
    );
    typedef struct _PROCESS_INFORMATION {
        HANDLE  hProcess;
        HANDLE  hThread;
        DWORD   dwProcessId;
        DWORD   dwThreadId;
    } PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
    typedef struct _STARTUPINFOA {
        DWORD   cb;
        LPSTR   lpReserved;
        LPSTR   lpDesktop;
        LPSTR   lpTitle;
        DWORD   dwX;
        DWORD   dwY;
        DWORD   dwXSize;
        DWORD   dwYSize;
        DWORD   dwXCountChars;
        DWORD   dwYCountChars;
        DWORD   dwFillAttribute;
        DWORD   dwFlags;
        WORD    wShowWindow;
        WORD    cbReserved2;
        LPBYTE  lpReserved2;
        HANDLE  hStdInput;
        HANDLE  hStdOutput;
        HANDLE  hStdError;
    } STARTUPINFOA, *LPSTARTUPINFOA;
    typedef struct _STARTUPINFOW {
        DWORD   cb;
        LPWSTR  lpReserved;
        LPWSTR  lpDesktop;
        LPWSTR  lpTitle;
        DWORD   dwX;
        DWORD   dwY;
        DWORD   dwXSize;
        DWORD   dwYSize;
        DWORD   dwXCountChars;
        DWORD   dwYCountChars;
        DWORD   dwFillAttribute;
        DWORD   dwFlags;
        WORD    wShowWindow;
        WORD    cbReserved2;
        LPBYTE  lpReserved2;
        HANDLE  hStdInput;
        HANDLE  hStdOutput;
        HANDLE  hStdError;
    } STARTUPINFOW, *LPSTARTUPINFOW;
    typedef STARTUPINFOW STARTUPINFO;
    typedef LPSTARTUPINFOW LPSTARTUPINFO;
    BOOL
    __cdecl
    CreateProcessA(
        LPCSTR pszImageName,
        LPCSTR pszCmdLine,
        LPSECURITY_ATTRIBUTES psaProcess,
        LPSECURITY_ATTRIBUTES psaThread,
        BOOL fInheritHandles,
        DWORD fdwCreate,
        LPVOID pvEnvironment,
        LPSTR pszCurDir,
        LPSTARTUPINFO psiStartInfo,
        LPPROCESS_INFORMATION pProcInfo
    );
    BOOL
    __cdecl
    CreateProcessW(
        LPCWSTR pszImageName,
        LPCWSTR pszCmdLine,
        LPSECURITY_ATTRIBUTES psaProcess,
        LPSECURITY_ATTRIBUTES psaThread,
        BOOL fInheritHandles,
        DWORD fdwCreate,
        LPVOID pvEnvironment,
        LPWSTR pszCurDir,
        LPSTARTUPINFO psiStartInfo,
        LPPROCESS_INFORMATION pProcInfo
    );
    DWORD
    __cdecl
    GetProcessVersion(
        DWORD ProcessId
    );
    DWORD
    __cdecl
    GetProcessId(
        HANDLE hProcess
    );
    __inline DWORD GetDllVersion(HMODULE hMod) {
        return GetProcessVersion((DWORD) hMod);
    }
    LPSTR
    __cdecl
    GetCommandLineA(
        void
    );
    LPWSTR
    __cdecl
    GetCommandLineW(
        void
    );
    typedef struct _WIN32_FIND_DATAA {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
        DWORD dwReserved0;
        DWORD dwReserved1;
        CHAR  cFileName[ 260 ];
        CHAR  cAlternateFileName[ 14 ];
    } WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
    typedef struct _WIN32_FIND_DATAW {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
        DWORD dwOID;
        WCHAR cFileName[ 260 ];
    } WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
    typedef WIN32_FIND_DATAW WIN32_FIND_DATA;
    typedef PWIN32_FIND_DATAW PWIN32_FIND_DATA;
    typedef LPWIN32_FIND_DATAW LPWIN32_FIND_DATA;
    typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
    } WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;
    DWORD
    __cdecl
    GetLastError(
        void
    );
    DWORD
    __cdecl
    GetTickCount(
        void
    );
    DWORD
    __cdecl
    GetIdleTime(
        void
    );
    DWORD
    __cdecl
    FormatMessageA(
        DWORD dwFlags,
        LPCVOID lpSource,
        DWORD dwMessageId,
        DWORD dwLanguageId,
        LPSTR lpBuffer,
        DWORD nSize,
        va_list* Arguments
    );
    DWORD
    __cdecl
    FormatMessageW(
        DWORD dwFlags,
        LPCVOID lpSource,
        DWORD dwMessageId,
        DWORD dwLanguageId,
        LPWSTR lpBuffer,
        DWORD nSize,
        va_list* Arguments
    );
    BOOL
    __cdecl
    CloseHandle(
        HANDLE hObject
    );
    BOOL
    __cdecl
    DuplicateHandle(
        HANDLE hSourceProcessHandle,
        HANDLE hSourceHandle,
        HANDLE hTargetProcessHandle,
        LPHANDLE lpTargetHandle,
        DWORD dwDesiredAccess,
        BOOL bInheritHandle,
        DWORD dwOptions
    );
    HGLOBAL
    __cdecl
    LoadResource(
        HMODULE hModule,
        HRSRC hResInfo
    );
    DWORD
    __cdecl
    SizeofResource(
        HMODULE hModule,
        HRSRC hResInfo
    );
    HRSRC
    __cdecl
    FindResourceA(
        HMODULE hModule,
        LPCSTR lpName,
        LPCSTR lpType
    );
    HRSRC
    __cdecl
    FindResourceW(
        HMODULE hModule,
        LPCWSTR lpName,
        LPCWSTR lpType
    );
    int
    __cdecl
    LoadStringA(
        HINSTANCE hInstance,
        UINT uID,
        LPSTR lpBuffer,
        int nBufferMax);
    int
    __cdecl
    LoadStringW(
        HINSTANCE hInstance,
        UINT uID,
        LPWSTR lpBuffer,
        int nBufferMax);
    ATOM
    __cdecl
    GlobalAddAtomA(
        LPCSTR lpString
    );
    ATOM
    __cdecl
    GlobalAddAtomW(
        LPCWSTR lpString
    );
    ATOM
    __cdecl
    GlobalFindAtomA(
        LPCSTR lpString
    );
    ATOM
    __cdecl
    GlobalFindAtomW(
        LPCWSTR lpString
    );
    ATOM
    __cdecl
    GlobalDeleteAtom(
        ATOM nAtom
    );
    BOOL
    __cdecl
    TerminateProcess(
        HANDLE hProcess,
        DWORD uExitCode
    );
    BOOL
    __cdecl
    FlushInstructionCache(
        HANDLE hProcess,
        LPCVOID lpBaseAddress,
        DWORD dwSize
    );
    BOOL
    __cdecl
    ReadProcessMemory(
        HANDLE hProcess,
        LPCVOID lpBaseAddress,
        LPVOID lpBuffer,
        DWORD nSize,
        LPDWORD lpNumberOfBytesRead
    );
    BOOL
    __cdecl
    WriteProcessMemory(
        HANDLE hProcess,
        LPVOID lpBaseAddress,
        LPVOID lpBuffer,
        DWORD nSize,
        LPDWORD lpNumberOfBytesWritten
    );
    HANDLE
    __cdecl
    OpenProcess(
        DWORD fdwAccess,
        BOOL fInherit,
        DWORD IDProcess
    );
    HANDLE
    __cdecl
    OpenThread(
        DWORD fdwAccess,
        BOOL fInherit,
        DWORD IDThread
    );
    BOOL
    __cdecl
    GetThreadContext(
        HANDLE hThread,
        LPCONTEXT lpContext
    );
    BOOL
    __cdecl
    SetThreadContext(
        HANDLE hThread,
        const CONTEXT* lpContext
    );
    DWORD
    __cdecl
    SuspendThread(
        HANDLE hThread
    );
    DWORD
    __cdecl
    ResumeThread(
        HANDLE hThread
    );
    BOOL
    __cdecl
    GetThreadTimes(
        HANDLE hThread,
        LPFILETIME lpCreationTime,
        LPFILETIME lpExitTime,
        LPFILETIME lpKernelTime,
        LPFILETIME lpUserTime
    );
    int
    __cdecl
    GetThreadPriority(
        HANDLE hThread
    );
    BOOL
    __cdecl
    SetThreadPriority(
        HANDLE hThread,
        int nPriority
    );
    int
    __cdecl
    CeGetThreadPriority(
        HANDLE hThread
    );
    BOOL
    __cdecl
    CeSetThreadPriority(
        HANDLE hThread,
        int nPriority
    );
    DWORD
    __cdecl
    CeGetThreadQuantum(
        HANDLE hThread
    );
    BOOL
    __cdecl
    CeSetThreadQuantum(
        HANDLE hThread,
        DWORD dwTime
    );
    DWORD
    __cdecl
    GetThreadId(
        HANDLE hThread
    );
    DWORD
    __cdecl
    GetProcessIdOfThread(
        HANDLE hThread
    );
    LPVOID
    __cdecl
    CeZeroPointer(
        LPVOID ptr
    );
    void
    __cdecl
    SetLastError(
        DWORD dwErrCode
    );
    UINT
    __cdecl
    GetTempFileNameA(
        LPCSTR lpPathName,
        LPCSTR lpPrefixString,
        UINT uUnique,
        LPSTR lpTempFileName
    );
    UINT
    __cdecl
    GetTempFileNameW(
        LPCWSTR lpPathName,
        LPCWSTR lpPrefixString,
        UINT uUnique,
        LPWSTR lpTempFileName
    );
    DWORD
    __cdecl
    CeGetCanonicalPathNameA(
        LPCSTR lpPathName,
        LPSTR lpCanonicalPathName,
        DWORD cchCanonicalPathName,
        DWORD dwReserved
    );
    DWORD
    __cdecl
    CeGetCanonicalPathNameW(
        LPCWSTR lpPathName,
        LPWSTR lpCanonicalPathName,
        DWORD cchCanonicalPathName,
        DWORD dwReserved
    );
    BOOL GetDiskFreeSpaceExA(LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller,
                             PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes);
    BOOL GetDiskFreeSpaceExW(LPCWSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller,
                             PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes);
    HANDLE
    __cdecl
    CreateFileA(
        LPCSTR lpFileName,
        DWORD dwDesiredAccess,
        DWORD dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        DWORD dwCreationDisposition,
        DWORD dwFlagsAndAttributes,
        HANDLE hTemplateFile
    );
    HANDLE
    __cdecl
    CreateFileW(
        LPCWSTR lpFileName,
        DWORD dwDesiredAccess,
        DWORD dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        DWORD dwCreationDisposition,
        DWORD dwFlagsAndAttributes,
        HANDLE hTemplateFile
    );
    BOOL
    __cdecl
    DeleteFileA(
        LPCSTR lpFileName
    );
    BOOL
    __cdecl
    DeleteFileW(
        LPCWSTR lpFileName
    );
    typedef enum _FINDEX_INFO_LEVELS {
        FindExInfoStandard,
        FindExInfoMaxInfoLevel
    } FINDEX_INFO_LEVELS;
    typedef enum _FINDEX_SEARCH_OPS {
        FindExSearchNameMatch,
        FindExSearchLimitToDirectories,
        FindExSearchLimitToDevices,
        FindExSearchMaxSearchOp
    } FINDEX_SEARCH_OPS;
    HANDLE
    __cdecl
    FindFirstFileExA(
        LPCSTR lpFileName,
        FINDEX_INFO_LEVELS fInfoLevelId,
        LPVOID lpFindFileData,
        FINDEX_SEARCH_OPS fSearchOp,
        LPVOID lpSearchFilter,
        DWORD dwAdditionalFlags
    );
    HANDLE
    __cdecl
    FindFirstFileExW(
        LPCWSTR lpFileName,
        FINDEX_INFO_LEVELS fInfoLevelId,
        LPVOID lpFindFileData,
        FINDEX_SEARCH_OPS fSearchOp,
        LPVOID lpSearchFilter,
        DWORD dwAdditionalFlags
    );
    BOOL
    __cdecl
    DeleteAndRenameFile(
        LPCWSTR lpOldFileName,
        LPCWSTR lpNewFileName
    );
    HANDLE
    __cdecl
    FindFirstFileA(
        LPCSTR lpFileName,
        LPWIN32_FIND_DATAA lpFindFileData
    );
    HANDLE
    __cdecl
    FindFirstFileW(
        LPCWSTR lpFileName,
        LPWIN32_FIND_DATAW lpFindFileData
    );
    BOOL
    __cdecl
    FindNextFileA(
        HANDLE hFindFile,
        LPWIN32_FIND_DATAA lpFindFileData
    );
    BOOL
    __cdecl
    FindNextFileW(
        HANDLE hFindFile,
        LPWIN32_FIND_DATAW lpFindFileData
    );
    BOOL
    __cdecl
    CopyFileA(
        LPCSTR lpExistingFileName,
        LPCSTR lpNewFileName,
        BOOL bFailIfExists
    );
    BOOL
    __cdecl
    CopyFileW(
        LPCWSTR lpExistingFileName,
        LPCWSTR lpNewFileName,
        BOOL bFailIfExists
    );
    typedef
    DWORD
    (__cdecl* LPPROGRESS_ROUTINE)(
        LARGE_INTEGER TotalFileSize,
        LARGE_INTEGER TotalBytesTransferred,
        LARGE_INTEGER StreamSize,
        LARGE_INTEGER StreamBytesTransferred,
        DWORD dwStreamNumber,
        DWORD dwCallbackReason,
        HANDLE hSourceFile,
        HANDLE hDestinationFile,
        LPVOID lpData
    );
    BOOL
    __cdecl
    CopyFileExA(
        LPCSTR lpExistingFileName,
        LPCSTR lpNewFileName,
        LPPROGRESS_ROUTINE lpProgressRoutine,
        LPVOID lpData,
        LPBOOL pbCancel,
        DWORD dwCopyFlags
    );
    BOOL
    __cdecl
    CopyFileExW(
        LPCWSTR lpExistingFileName,
        LPCWSTR lpNewFileName,
        LPPROGRESS_ROUTINE lpProgressRoutine,
        LPVOID lpData,
        LPBOOL pbCancel,
        DWORD dwCopyFlags
    );
    BOOL
    __cdecl
    MoveFileA(
        LPCSTR lpExistingFileName,
        LPCSTR lpNewFileName
    );
    BOOL
    __cdecl
    MoveFileW(
        LPCWSTR lpExistingFileName,
        LPCWSTR lpNewFileName
    );
    BOOL
    __cdecl
    CreateDirectoryA(
        LPCSTR lpPathName,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
    BOOL
    __cdecl
    CreateDirectoryW(
        LPCWSTR lpPathName,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
    BOOL
    __cdecl
    RemoveDirectoryA(
        LPCSTR lpPathName
    );
    BOOL
    __cdecl
    RemoveDirectoryW(
        LPCWSTR lpPathName
    );
    BOOL
    __cdecl
    GetFileSecurityA(
        LPCSTR pFileName,
        SECURITY_INFORMATION RequestedInformation,
        PSECURITY_DESCRIPTOR pSecurityDescriptor,
        DWORD Length,
        DWORD* pLengthNeeded
    );
    BOOL
    __cdecl
    GetFileSecurityW(
        LPCWSTR pFileName,
        SECURITY_INFORMATION RequestedInformation,
        PSECURITY_DESCRIPTOR pSecurityDescriptor,
        DWORD Length,
        DWORD* pLengthNeeded
    );
    BOOL
    __cdecl
    SetFileSecurityA(
        LPCSTR pFileName,
        SECURITY_INFORMATION SecurityInformation,
        PSECURITY_DESCRIPTOR pSecurityDescriptor,
        DWORD Length
    );
    BOOL
    __cdecl
    SetFileSecurityW(
        LPCWSTR pFileName,
        SECURITY_INFORMATION SecurityInformation,
        PSECURITY_DESCRIPTOR pSecurityDescriptor,
        DWORD Length
    );
    BOOL
    __cdecl
    WriteFile(
        HANDLE hFile,
        LPCVOID lpBuffer,
        DWORD nNumberOfBytesToWrite,
        LPDWORD lpNumberOfBytesWritten,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    ReadFile(
        HANDLE hFile,
        LPVOID lpBuffer,
        DWORD nNumberOfBytesToRead,
        LPDWORD lpNumberOfBytesRead,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    ReadFileScatter(
        HANDLE hFile,
        FILE_SEGMENT_ELEMENT aSegmentArray[],
        DWORD nNumberOfBytesToRead,
        LPDWORD lpReserved,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    WriteFileGather(
        HANDLE hFile,
        FILE_SEGMENT_ELEMENT aSegmentArray[],
        DWORD nNumberOfBytesToWrite,
        LPDWORD lpReserved,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    LockFileEx(
        HANDLE hFile,
        DWORD dwFlags,
        DWORD dwReserved,
        DWORD nNumberOfBytesToLockLow,
        DWORD nNumberOfBytesToLockHigh,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    UnlockFileEx(
        HANDLE hFile,
        DWORD dwReserved,
        DWORD nNumberOfBytesToUnlockLow,
        DWORD nNumberOfBytesToUnlockHigh,
        LPOVERLAPPED lpOverlapped
    );
    BOOL __cdecl FlushFileBuffers(HANDLE hFile);
    BOOL __cdecl GetFileTime(HANDLE hFile, LPFILETIME lpCreation, LPFILETIME lpLastAccess, LPFILETIME lpLastWrite);
    BOOL __cdecl SetFileTime(HANDLE hFile, const FILETIME* lpCreation, const FILETIME* lpLastAccess, const FILETIME* lpLastWrite);
    BOOL __cdecl SetEndOfFile(HANDLE hFile);
    BOOL __cdecl DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
    HANDLE
    __cdecl
    FindFirstChangeNotificationA(
        LPCSTR lpPathName,
        BOOL bWatchSubtree,
        DWORD dwNotifyFilter
    );
    HANDLE
    __cdecl
    FindFirstChangeNotificationW(
        LPCWSTR lpPathName,
        BOOL bWatchSubtree,
        DWORD dwNotifyFilter
    );
    BOOL
    __cdecl
    FindNextChangeNotification(
        HANDLE hChangeHandle
    );
    BOOL
    __cdecl
    CeGetFileNotificationInfo(
        HANDLE hChangeHandle,
        DWORD dwFlags,
        LPVOID lpBuffer,
        DWORD nBufferLength,
        LPDWORD lpBytesReturned,
        LPDWORD lpBytesAvailable
    );
    BOOL
    __cdecl
    FindCloseChangeNotification(
        HANDLE hChangeHandle
    );
    BOOL
    CeFsIoControlW(
        LPCWSTR pszRootPath,
        DWORD dwIoctl,
        LPVOID lpInBuf,
        DWORD nInBufSize,
        LPVOID lpOutBuf,
        DWORD nOutBufSize,
        LPDWORD lpBytesReturned,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    CeFsIoControlA(
        LPCSTR pszRootPath,
        DWORD dwIoctl,
        LPVOID lpInBuf,
        DWORD nInBufSize,
        LPVOID lpOutBuf,
        DWORD nOutBufSize,
        LPDWORD lpBytesReturned,
        LPOVERLAPPED lpOverlapped
    );
    BOOL
    __cdecl
    SetFileAttributesA(
        LPCSTR lpFileName,
        DWORD dwFileAttributes
    );
    BOOL
    __cdecl
    SetFileAttributesW(
        LPCWSTR lpFileName,
        DWORD dwFileAttributes
    );
    DWORD
    __cdecl
    SetFilePointer(
        HANDLE hFile,
        LONG lDistanceToMove,
        PLONG lpDistanceToMoveHigh,
        DWORD dwMoveMethod
    );
    BOOL
    __cdecl
    FindClose(
        HANDLE hFindFile
    );
    DWORD
    __cdecl
    GetFileSize(
        HANDLE hFile,
        LPDWORD lpFileSizeHigh
    );
    DWORD
    __cdecl
    GetFileAttributesA(
        LPCSTR lpFileName
    );
    DWORD
    __cdecl
    GetFileAttributesW(
        LPCWSTR lpFileName
    );
    typedef enum _GET_FILEEX_INFO_LEVELS {
        GetFileExInfoStandard,
        GetFileExMaxInfoLevel
    } GET_FILEEX_INFO_LEVELS;
    BOOL
    __cdecl
    GetFileAttributesExA(
        LPCSTR lpFileName,
        GET_FILEEX_INFO_LEVELS fInfoLevelId,
        LPVOID lpFileInformation
    );
    BOOL
    __cdecl
    GetFileAttributesExW(
        LPCWSTR lpFileName,
        GET_FILEEX_INFO_LEVELS fInfoLevelId,
        LPVOID lpFileInformation
    );
    typedef struct _BY_HANDLE_FILE_INFORMATION {
        DWORD dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD dwVolumeSerialNumber;
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
        DWORD nNumberOfLinks;
        DWORD nFileIndexHigh;
        DWORD nFileIndexLow;
        DWORD dwOID;
    } BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
    BOOL
    __cdecl
    GetFileInformationByHandle(
        HANDLE hFile,
        LPBY_HANDLE_FILE_INFORMATION lpFileInformation
    );
    void
    __cdecl
    Sleep(
        DWORD dwMilliseconds
    );
    HANDLE
    __cdecl
    CreateMutexA(
        LPSECURITY_ATTRIBUTES lpsa,
        BOOL bInitialOwner,
        LPCSTR lpName);
    HANDLE
    __cdecl
    CreateMutexW(
        LPSECURITY_ATTRIBUTES lpsa,
        BOOL bInitialOwner,
        LPCWSTR lpName);
    HANDLE
    __cdecl
    CreateEventA(
        LPSECURITY_ATTRIBUTES lpEventAttributes,
        BOOL bManualReset,
        BOOL bInitialState,
        LPCSTR lpName
    );
    HANDLE
    __cdecl
    CreateEventW(
        LPSECURITY_ATTRIBUTES lpEventAttributes,
        BOOL bManualReset,
        BOOL bInitialState,
        LPCWSTR lpName
    );
    HANDLE
    __cdecl
    OpenEventA(
        DWORD dwDesiredAccess,
        BOOL bInheritHandle,
        LPCSTR lpName
    );
    HANDLE
    __cdecl
    OpenEventW(
        DWORD dwDesiredAccess,
        BOOL bInheritHandle,
        LPCWSTR lpName
    );
    HANDLE
    __cdecl
    CreateSemaphoreA(
        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        LONG lInitialCount,
        LONG lMaximumCount,
        LPCSTR lpName
    );
    HANDLE
    __cdecl
    CreateSemaphoreW(
        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        LONG lInitialCount,
        LONG lMaximumCount,
        LPCWSTR lpName
    );
    BOOL
    __cdecl
    ReleaseSemaphore(
        HANDLE hSemaphore,
        LONG lReleaseCount,
        LPLONG lpPreviousCount
    );
    BOOL
    __cdecl
    ReleaseMutex(
        HANDLE hMutex
    );
    DWORD
    __cdecl
    WaitForSingleObject(
        HANDLE hHandle,
        DWORD dwMilliseconds
    );
    DWORD
    __cdecl
    WaitForMultipleObjects(
        DWORD cObjects,
        const HANDLE* lphObjects,
        BOOL fWaitAll,
        DWORD dwTimeout
    );
    BOOL
    __cdecl
    GetExitCodeThread(
        HANDLE hThread,
        LPDWORD lpExitCode
    );
    BOOL
    __cdecl
    GetExitCodeProcess(
        HANDLE hProcess,
        LPDWORD lpExitCode
    );
    DWORD
    __cdecl
    TlsAlloc(
        void
    );
    BOOL
    __cdecl
    IsBadCodePtr(
        FARPROC lpfn
    );
    BOOL
    __cdecl
    IsBadReadPtr(
        const void* lp,
        UINT ucb
    );
    BOOL
    __cdecl
    TlsFree(
        DWORD dwTlsIndex
    );
    BOOL
    __cdecl
    IsBadWritePtr(
        LPVOID lp,
        UINT ucb
    );
    void
    __cdecl
    GetSystemInfo(
        LPSYSTEM_INFO lpSystemInfo
    );
    BOOL
    __cdecl
    QueryInstructionSet(
        DWORD dwInstructionSet,
        LPDWORD lpdwCurrentInstructionSet
    );
    BOOL
    __cdecl
    IsProcessorFeaturePresent(
        DWORD dwProcessorFeature
    );
    void
    __cdecl
    RaiseException(
        DWORD dwExceptionCode,
        DWORD dwExceptionFlags,
        DWORD nNumberOfArguments,
        const DWORD* lpArguments
    );
    BOOL
    __cdecl
    FreeLibrary(
        HMODULE hLibModule
    );
    void
    __cdecl
    FreeLibraryAndExitThread(
        HMODULE hLibModule,
        DWORD dwExitCode
    );
    BOOL
    __cdecl
    DisableThreadLibraryCalls(
        HMODULE hLibModule
    );
    extern "C" {
        void __cdecl NKDbgPrintfW(LPCWSTR lpszFmt, ...);
        void __cdecl WriteDebugLED(WORD wIndex, DWORD dwPattern);
        typedef struct _DBGPARAM {
            WCHAR   lpszName[32];
            WCHAR   rglpszZones[16][32];
            ULONG   ulZoneMask;
        } DBGPARAM, *LPDBGPARAM;
        BOOL RegisterDbgZones(HMODULE hMod, LPDBGPARAM lpdbgparam);
        extern  DBGPARAM    dpCurSettings;
    }
    _inline BOOL IsDebuggerPresent(void) {
        BOOL bIsDebuggerPresent;

        if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &bIsDebuggerPresent)) {
            return bIsDebuggerPresent;
        }

        return 0;
    }
    typedef struct _COMMPROP {
        WORD wPacketLength;
        WORD wPacketVersion;
        DWORD dwServiceMask;
        DWORD dwReserved1;
        DWORD dwMaxTxQueue;
        DWORD dwMaxRxQueue;
        DWORD dwMaxBaud;
        DWORD dwProvSubType;
        DWORD dwProvCapabilities;
        DWORD dwSettableParams;
        DWORD dwSettableBaud;
        WORD wSettableData;
        WORD wSettableStopParity;
        DWORD dwCurrentTxQueue;
        DWORD dwCurrentRxQueue;
        DWORD dwProvSpec1;
        DWORD dwProvSpec2;
        WCHAR wcProvChar[1];
    } COMMPROP, *LPCOMMPROP;
    typedef struct _COMSTAT {
        DWORD fCtsHold : 1;
        DWORD fDsrHold : 1;
        DWORD fRlsdHold : 1;
        DWORD fXoffHold : 1;
        DWORD fXoffSent : 1;
        DWORD fEof : 1;
        DWORD fTxim : 1;
        DWORD fReserved : 25;
        DWORD cbInQue;
        DWORD cbOutQue;
    } COMSTAT, *LPCOMSTAT;
    typedef struct _DCB {
        DWORD DCBlength;
        DWORD BaudRate;
        DWORD fBinary: 1;
        DWORD fParity: 1;
        DWORD fOutxCtsFlow: 1;
        DWORD fOutxDsrFlow: 1;
        DWORD fDtrControl: 2;
        DWORD fDsrSensitivity: 1;
        DWORD fTXContinueOnXoff: 1;
        DWORD fOutX: 1;
        DWORD fInX: 1;
        DWORD fErrorChar: 1;
        DWORD fNull: 1;
        DWORD fRtsControl: 2;
        DWORD fAbortOnError: 1;
        DWORD fDummy2: 17;
        WORD wReserved;
        WORD XonLim;
        WORD XoffLim;
        BYTE ByteSize;
        BYTE Parity;
        BYTE StopBits;
        char XonChar;
        char XoffChar;
        char ErrorChar;
        char EofChar;
        char EvtChar;
        WORD wReserved1;
    } DCB, *LPDCB;
    typedef struct _COMMTIMEOUTS {
        DWORD ReadIntervalTimeout;
        DWORD ReadTotalTimeoutMultiplier;
        DWORD ReadTotalTimeoutConstant;
        DWORD WriteTotalTimeoutMultiplier;
        DWORD WriteTotalTimeoutConstant;
    } COMMTIMEOUTS, *LPCOMMTIMEOUTS;
    typedef struct _COMMCONFIG {
        DWORD dwSize;
        WORD wVersion;
        WORD wReserved;
        DCB dcb;
        DWORD dwProviderSubType;
        DWORD dwProviderOffset;
        DWORD dwProviderSize;
        WCHAR wcProviderData[1];
    } COMMCONFIG, *LPCOMMCONFIG;
    BOOL
    __cdecl
    ClearCommBreak(
        HANDLE hFile
    );
    BOOL
    __cdecl
    ClearCommError(
        HANDLE hFile,
        LPDWORD lpErrors,
        LPCOMSTAT lpStat
    );
    BOOL
    __cdecl
    SetupComm(
        HANDLE hFile,
        DWORD dwInQueue,
        DWORD dwOutQueue
    );
    BOOL
    __cdecl
    EscapeCommFunction(
        HANDLE hFile,
        DWORD dwFunc
    );
    BOOL
    __cdecl
    GetCommMask(
        HANDLE hFile,
        LPDWORD lpEvtMask
    );
    BOOL
    __cdecl
    GetCommProperties(
        HANDLE hFile,
        LPCOMMPROP lpCommProp
    );
    BOOL
    __cdecl
    GetCommModemStatus(
        HANDLE hFile,
        LPDWORD lpModemStat
    );
    BOOL
    __cdecl
    GetCommState(
        HANDLE hFile,
        LPDCB lpDCB
    );
    BOOL
    __cdecl
    GetCommTimeouts(
        HANDLE hFile,
        LPCOMMTIMEOUTS lpCommTimeouts
    );
    BOOL
    __cdecl
    PurgeComm(
        HANDLE hFile,
        DWORD dwFlags
    );
    BOOL
    __cdecl
    SetCommBreak(
        HANDLE hFile
    );
    BOOL
    __cdecl
    SetCommMask(
        HANDLE hFile,
        DWORD dwEvtMask
    );
    BOOL
    __cdecl
    SetCommState(
        HANDLE hFile,
        LPDCB lpDCB
    );
    BOOL
    __cdecl
    SetCommTimeouts(
        HANDLE hFile,
        LPCOMMTIMEOUTS lpCommTimeouts
    );
    BOOL
    __cdecl
    TransmitCommChar(
        HANDLE hFile,
        char cChar
    );
    BOOL
    __cdecl
    WaitCommEvent(
        HANDLE hFile,
        LPDWORD lpEvtMask,
        LPOVERLAPPED lpOverlapped
    );
    typedef struct _SYSTEM_POWER_STATUS_EX {
        BYTE ACLineStatus;
        BYTE BatteryFlag;
        BYTE BatteryLifePercent;
        BYTE Reserved1;
        DWORD BatteryLifeTime;
        DWORD BatteryFullLifeTime;
        BYTE Reserved2;
        BYTE BackupBatteryFlag;
        BYTE BackupBatteryLifePercent;
        BYTE Reserved3;
        DWORD BackupBatteryLifeTime;
        DWORD BackupBatteryFullLifeTime;
    }   SYSTEM_POWER_STATUS_EX, *PSYSTEM_POWER_STATUS_EX, *LPSYSTEM_POWER_STATUS_EX;
    typedef struct _SYSTEM_POWER_STATUS_EX2 {
        BYTE ACLineStatus;
        BYTE BatteryFlag;
        BYTE BatteryLifePercent;
        BYTE Reserved1;
        DWORD BatteryLifeTime;
        DWORD BatteryFullLifeTime;
        BYTE Reserved2;
        BYTE BackupBatteryFlag;
        BYTE BackupBatteryLifePercent;
        BYTE Reserved3;
        DWORD BackupBatteryLifeTime;
        DWORD BackupBatteryFullLifeTime;

        DWORD BatteryVoltage;
        DWORD BatteryCurrent;
        DWORD BatteryAverageCurrent;
        DWORD BatteryAverageInterval;
        DWORD BatterymAHourConsumed;
        DWORD BatteryTemperature;
        DWORD BackupBatteryVoltage;
        BYTE  BatteryChemistry;
    }   SYSTEM_POWER_STATUS_EX2, *PSYSTEM_POWER_STATUS_EX2, *LPSYSTEM_POWER_STATUS_EX2;
    BOOL
    __cdecl
    GetSystemPowerStatusEx(
        PSYSTEM_POWER_STATUS_EX pSystemPowerStatusEx,
        BOOL fUpdate
    );
    DWORD
    __cdecl
    GetSystemPowerStatusEx2(
        PSYSTEM_POWER_STATUS_EX2 pSystemPowerStatusEx2,
        DWORD dwLen,
        BOOL fUpdate
    );
    void
    __cdecl
    BatteryNotifyOfTimeChange(
        BOOL fForward,
        FILETIME* pftDelta
    );
    BOOL CheckPassword(LPWSTR lpszPassword);
    HANDLE RegisterDevice(LPCWSTR lpszName, DWORD index, LPCWSTR lpszLib, DWORD dwInfo);
    BOOL DeregisterDevice(HANDLE hDevice);
    BOOL LoadFSD(HANDLE hDevice, LPCWSTR lpFSDName);
    BOOL LoadFSDEx(HANDLE hDevice, LPCWSTR lpFSDName, DWORD dwFlag);
    HANDLE ActivateDevice(LPCWSTR lpszDevKey, DWORD dwClientInfo);
    HANDLE ActivateDeviceEx(LPCWSTR lpszDevKey, LPCVOID lpRegEnts, DWORD cRegEnts, LPVOID lpvParam);
    BOOL DeactivateDevice(HANDLE hDevice);
    BOOL CeResyncFilesys(HANDLE hDevice);
    typedef struct _DevmgrDeviceInformation_tag {
        DWORD dwSize;
        HANDLE hDevice;
        HANDLE hParentDevice;
        WCHAR szLegacyName[6];
        WCHAR szDeviceKey[260];
        WCHAR szDeviceName[260];
        WCHAR szBusName[260];
    } DEVMGR_DEVICE_INFORMATION, *PDEVMGR_DEVICE_INFORMATION;
    BOOL GetDeviceInformationByDeviceHandle(HANDLE hDevice, PDEVMGR_DEVICE_INFORMATION pdi);
    BOOL GetDeviceInformationByFileHandle(HANDLE hFile, PDEVMGR_DEVICE_INFORMATION pdi);
    typedef enum {
        DeviceSearchByLegacyName,
        DeviceSearchByDeviceName,
        DeviceSearchByBusName,
        DeviceSearchByGuid,
        DeviceSearchByParent
    } DeviceSearchType;
    HANDLE FindFirstDevice(DeviceSearchType searchType, LPCVOID pvSearchParam, PDEVMGR_DEVICE_INFORMATION pdi);
    BOOL FindNextDevice(HANDLE h, PDEVMGR_DEVICE_INFORMATION pdi);
    BOOL EnumDeviceInterfaces(HANDLE h, DWORD dwIndex, GUID* pClass, LPWSTR pszNameBuf, LPDWORD lpdwNameBufSize);
    HKEY RegOpenProcessKey(DWORD hPnp);
    BOOL CeGenRandom(DWORD dwLen, PBYTE pbBuffer);
    BOOL ResourceCreateList(DWORD dwResId, DWORD dwMinimum, DWORD dwCount);
    BOOL ResourceDestroyList(DWORD dwResId);
    BOOL ResourceRequest(DWORD dwResId, DWORD dwId, DWORD dwLen);
    BOOL ResourceRequestEx(DWORD dwResId, DWORD dwId, DWORD dwLen, DWORD dwFlags);
    BOOL ResourceRelease(DWORD dwResId, DWORD dwId, DWORD dwLen);
    BOOL ResourceMarkAsShareable(DWORD dwResId, DWORD dwId, DWORD dwLen, BOOL fShareable);
    HANDLE RequestDeviceNotifications(const GUID* devclass, HANDLE hMsgQ, BOOL fAll);
    BOOL StopDeviceNotifications(HANDLE h);
    BOOL AdvertiseInterface(const GUID* devclass, LPCWSTR name, BOOL fAdd);
    extern "C" {
        HANDLE __cdecl CreateMsgQueue(LPCWSTR lpName, LPMSGQUEUEOPTIONS lpOptions);
        HANDLE __cdecl OpenMsgQueue(HANDLE hSrcProc, HANDLE hMsgQ, LPMSGQUEUEOPTIONS lpOptions);
        BOOL __cdecl ReadMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize,
                                  LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD* pdwFlags);
        BOOL __cdecl ReadMsgQueueEx(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize,
                                    LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD* pdwFlags, PHANDLE phTok);
        BOOL __cdecl WriteMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbDataSize,
                                   DWORD dwTimeout, DWORD dwFlags);
        BOOL __cdecl GetMsgQueueInfo(HANDLE hMsgQ, LPMSGQUEUEINFO lpInfo);
        BOOL __cdecl CloseMsgQueue(HANDLE hMsgQ);
    }
    typedef enum {
        NameUnknown = 0,
        NameFullyQualifiedDN = 1,
        NameSamCompatible = 2,

        NameDisplay = 3,

        NameDomainSimple = 4,

        NameEnterpriseSimple = 5,

        NameUniqueId = 6,
        NameCanonical = 7,
        NameUserPrincipal = 8,


        NameCanonicalEx = 9,
        NameServicePrincipal = 10,

        NameDnsDomain = 12,
        NameWindowsCeLocal = 0x80000001
    } EXTENDED_NAME_FORMAT, * PEXTENDED_NAME_FORMAT;
    BOOLEAN
    __cdecl
    GetUserNameExA(
        EXTENDED_NAME_FORMAT  NameFormat,
        LPSTR lpNameBuffer,
        PULONG nSize
    );
    BOOLEAN
    __cdecl
    GetUserNameExW(
        EXTENDED_NAME_FORMAT NameFormat,
        LPWSTR lpNameBuffer,
        PULONG nSize
    );
    BOOL __cdecl ReportEventW(HANDLE hEventLog, WORD wType, WORD wCategory, DWORD dwEventID,
                              PSID lpUserSid, WORD wNumStrings, DWORD dwDataSize,
                              LPCWSTR* lpStrings, LPVOID lpRawData);
    HANDLE __cdecl RegisterEventSourceW(LPCWSTR lpUNCServerName, LPCWSTR lpSourceName);
    BOOL __cdecl DeregisterEventSource(HANDLE hEventLog);
    BOOL __cdecl ClearEventLogW(HANDLE hEventLog, LPCWSTR lpBackupFileName);
    HANDLE __cdecl OpenEventLogW(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);
    BOOL __cdecl CloseEventLog(HANDLE hEventLog);
    BOOL __cdecl BackupEventLogW(HANDLE hEventLog, LPCTSTR szBackupFileName);
    BOOL __cdecl LockEventLog(HANDLE hEventLog);
    BOOL __cdecl UnLockEventLog(HANDLE hEventLog);
    BOOL __cdecl ReadEventLogRaw(HANDLE hEventLog, BYTE* pReadBuffer, DWORD dwReadBufferSize, DWORD* pdwBytesRead);
    DWORD
    __cdecl
    CeCertVerify(
        HANDLE hFile,
        HANDLE* phHandle,
        LPCWSTR pszFileName,
        LPWSTR pszAccountName,
        DWORD cbAccountNameSize,
        DWORD dwFlags);
    extern "C" {
        typedef DWORD LGRPID;
        typedef DWORD LCTYPE;
        typedef DWORD CALTYPE;
        typedef DWORD CALID;
        typedef struct _cpinfo {
            UINT    MaxCharSize;
            BYTE    DefaultChar[2];
            BYTE    LeadByte[12];
        } CPINFO, *LPCPINFO;
        typedef struct _cpinfoexA {
            UINT    MaxCharSize;
            BYTE    DefaultChar[2];
            BYTE    LeadByte[12];
            WCHAR   UnicodeDefaultChar;
            UINT    CodePage;
            CHAR    CodePageName[260];
        } CPINFOEXA, *LPCPINFOEXA;
        typedef struct _cpinfoexW {
            UINT    MaxCharSize;
            BYTE    DefaultChar[2];
            BYTE    LeadByte[12];
            WCHAR   UnicodeDefaultChar;
            UINT    CodePage;
            WCHAR   CodePageName[260];
        } CPINFOEXW, *LPCPINFOEXW;
        typedef CPINFOEXW CPINFOEX;
        typedef LPCPINFOEXW LPCPINFOEX;
        typedef struct _numberfmtA {
            UINT    NumDigits;
            UINT    LeadingZero;
            UINT    Grouping;
            LPSTR   lpDecimalSep;
            LPSTR   lpThousandSep;
            UINT    NegativeOrder;
        } NUMBERFMTA, *LPNUMBERFMTA;
        typedef struct _numberfmtW {
            UINT    NumDigits;
            UINT    LeadingZero;
            UINT    Grouping;
            LPWSTR  lpDecimalSep;
            LPWSTR  lpThousandSep;
            UINT    NegativeOrder;
        } NUMBERFMTW, *LPNUMBERFMTW;
        typedef NUMBERFMTW NUMBERFMT;
        typedef LPNUMBERFMTW LPNUMBERFMT;
        typedef struct _currencyfmtA {
            UINT    NumDigits;
            UINT    LeadingZero;
            UINT    Grouping;
            LPSTR   lpDecimalSep;
            LPSTR   lpThousandSep;
            UINT    NegativeOrder;
            UINT    PositiveOrder;
            LPSTR   lpCurrencySymbol;
        } CURRENCYFMTA, *LPCURRENCYFMTA;
        typedef struct _currencyfmtW {
            UINT    NumDigits;
            UINT    LeadingZero;
            UINT    Grouping;
            LPWSTR  lpDecimalSep;
            LPWSTR  lpThousandSep;
            UINT    NegativeOrder;
            UINT    PositiveOrder;
            LPWSTR  lpCurrencySymbol;
        } CURRENCYFMTW, *LPCURRENCYFMTW;
        typedef CURRENCYFMTW CURRENCYFMT;
        typedef LPCURRENCYFMTW LPCURRENCYFMT;
        typedef BOOL (__cdecl* LANGUAGEGROUP_ENUMPROCA)(LGRPID, LPSTR, LPSTR, DWORD);
        typedef BOOL (__cdecl* LANGGROUPLOCALE_ENUMPROCA)(LGRPID, LCID, LPSTR);
        typedef BOOL (__cdecl* UILANGUAGE_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* LOCALE_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* CODEPAGE_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* DATEFMT_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* DATEFMT_ENUMPROCEXA)(LPSTR, CALID);
        typedef BOOL (__cdecl* TIMEFMT_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* CALINFO_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* CALINFO_ENUMPROCEXA)(LPSTR, CALID);
        typedef BOOL (__cdecl* UILANGUAGE_ENUMPROCA)(LPSTR);
        typedef BOOL (__cdecl* LANGUAGEGROUP_ENUMPROCW)(LGRPID, LPWSTR, LPWSTR, DWORD);
        typedef BOOL (__cdecl* LANGGROUPLOCALE_ENUMPROCW)(LGRPID, LCID, LPWSTR);
        typedef BOOL (__cdecl* UILANGUAGE_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* LOCALE_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* CODEPAGE_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* DATEFMT_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* DATEFMT_ENUMPROCEXW)(LPWSTR, CALID);
        typedef BOOL (__cdecl* TIMEFMT_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* CALINFO_ENUMPROCW)(LPWSTR);
        typedef BOOL (__cdecl* CALINFO_ENUMPROCEXW)(LPWSTR, CALID);
        typedef BOOL (__cdecl* UILANGUAGE_ENUMPROCW)(LPWSTR);
        BOOL
        __cdecl
        IsValidCodePage(
            UINT  CodePage);
        UINT
        __cdecl
        GetACP(void);
        UINT
        __cdecl
        GetOEMCP(void);
        BOOL
        __cdecl
        GetCPInfo(
            UINT       CodePage,
            LPCPINFO  lpCPInfo);
        BOOL
        __cdecl
        IsDBCSLeadByte(
            BYTE  TestChar);
        BOOL
        __cdecl
        IsDBCSLeadByteEx(
            UINT  CodePage,
            BYTE  TestChar);
        int
        __cdecl
        MultiByteToWideChar(
            UINT     CodePage,
            DWORD    dwFlags,
            LPCSTR   lpMultiByteStr,
            int      cbMultiByte,
            LPWSTR  lpWideCharStr,
            int      cchWideChar);
        int
        __cdecl
        WideCharToMultiByte(
            UINT     CodePage,
            DWORD    dwFlags,
            LPCWSTR  lpWideCharStr,
            int      cchWideChar,
            LPSTR   lpMultiByteStr,
            int      cbMultiByte,
            LPCSTR   lpDefaultChar,
            LPBOOL  lpUsedDefaultChar);
        int
        __cdecl
        CompareStringA(
            LCID     Locale,
            DWORD    dwCmpFlags,
            LPCSTR  lpString1,
            int      cchCount1,
            LPCSTR  lpString2,
            int      cchCount2);
        int
        __cdecl
        CompareStringW(
            LCID     Locale,
            DWORD    dwCmpFlags,
            LPCWSTR  lpString1,
            int      cchCount1,
            LPCWSTR  lpString2,
            int      cchCount2);
        int
        __cdecl
        LCMapStringA(
            LCID     Locale,
            DWORD    dwMapFlags,
            LPCSTR  lpSrcStr,
            int      cchSrc,
            LPSTR  lpDestStr,
            int      cchDest);
        int
        __cdecl
        LCMapStringW(
            LCID     Locale,
            DWORD    dwMapFlags,
            LPCWSTR  lpSrcStr,
            int      cchSrc,
            LPWSTR  lpDestStr,
            int      cchDest);
        int
        __cdecl
        GetLocaleInfoA(
            LCID     Locale,
            LCTYPE   LCType,
            LPSTR  lpLCData,
            int      cchData);
        int
        __cdecl
        GetLocaleInfoW(
            LCID     Locale,
            LCTYPE   LCType,
            LPWSTR  lpLCData,
            int      cchData);
        BOOL
        __cdecl
        SetLocaleInfoA(
            LCID     Locale,
            LCTYPE   LCType,
            LPCSTR  lpLCData);
        BOOL
        __cdecl
        SetLocaleInfoW(
            LCID     Locale,
            LCTYPE   LCType,
            LPCWSTR  lpLCData);
        int
        __cdecl
        GetTimeFormatA(
            LCID             Locale,
            DWORD            dwFlags,
            const SYSTEMTIME* lpTime,
            LPCSTR          lpFormat,
            LPSTR          lpTimeStr,
            int              cchTime);
        int
        __cdecl
        GetTimeFormatW(
            LCID             Locale,
            DWORD            dwFlags,
            const SYSTEMTIME* lpTime,
            LPCWSTR          lpFormat,
            LPWSTR          lpTimeStr,
            int              cchTime);
        int
        __cdecl
        GetDateFormatA(
            LCID             Locale,
            DWORD            dwFlags,
            const SYSTEMTIME* lpDate,
            LPCSTR          lpFormat,
            LPSTR          lpDateStr,
            int              cchDate);
        int
        __cdecl
        GetDateFormatW(
            LCID             Locale,
            DWORD            dwFlags,
            const SYSTEMTIME* lpDate,
            LPCWSTR          lpFormat,
            LPWSTR          lpDateStr,
            int              cchDate);
        int
        __cdecl
        GetNumberFormatA(
            LCID             Locale,
            DWORD            dwFlags,
            LPCSTR          lpValue,
            const NUMBERFMTA* lpFormat,
            LPSTR          lpNumberStr,
            int              cchNumber);
        int
        __cdecl
        GetNumberFormatW(
            LCID             Locale,
            DWORD            dwFlags,
            LPCWSTR          lpValue,
            const NUMBERFMTW* lpFormat,
            LPWSTR          lpNumberStr,
            int              cchNumber);
        int
        __cdecl
        GetCurrencyFormatA(
            LCID               Locale,
            DWORD              dwFlags,
            LPCSTR            lpValue,
            const CURRENCYFMTA* lpFormat,
            LPSTR            lpCurrencyStr,
            int                cchCurrency);
        int
        __cdecl
        GetCurrencyFormatW(
            LCID               Locale,
            DWORD              dwFlags,
            LPCWSTR            lpValue,
            const CURRENCYFMTW* lpFormat,
            LPWSTR            lpCurrencyStr,
            int                cchCurrency);
        BOOL
        __cdecl
        EnumCalendarInfoA(
            CALINFO_ENUMPROCA lpCalInfoEnumProc,
            LCID              Locale,
            CALID             Calendar,
            CALTYPE           CalType);
        BOOL
        __cdecl
        EnumCalendarInfoW(
            CALINFO_ENUMPROCW lpCalInfoEnumProc,
            LCID              Locale,
            CALID             Calendar,
            CALTYPE           CalType);
        BOOL
        __cdecl
        EnumTimeFormatsA(
            TIMEFMT_ENUMPROCA lpTimeFmtEnumProc,
            LCID              Locale,
            DWORD             dwFlags);
        BOOL
        __cdecl
        EnumTimeFormatsW(
            TIMEFMT_ENUMPROCW lpTimeFmtEnumProc,
            LCID              Locale,
            DWORD             dwFlags);
        BOOL
        __cdecl
        EnumDateFormatsA(
            DATEFMT_ENUMPROCA lpDateFmtEnumProc,
            LCID              Locale,
            DWORD             dwFlags);
        BOOL
        __cdecl
        EnumDateFormatsW(
            DATEFMT_ENUMPROCW lpDateFmtEnumProc,
            LCID              Locale,
            DWORD             dwFlags);
        BOOL
        __cdecl
        IsValidLocale(
            LCID   Locale,
            DWORD  dwFlags);
        LCID
        __cdecl
        ConvertDefaultLocale(
            LCID   Locale);
        LANGID
        __cdecl
        GetSystemDefaultLangID(void);
        LANGID
        __cdecl
        GetUserDefaultLangID(void);
        BOOL
        __cdecl
        SetUserDefaultUILanguage(LANGID langid);
        LCID
        __cdecl
        GetSystemDefaultLCID(void);
        LCID
        __cdecl
        GetUserDefaultLCID(void);
        BOOL
        __cdecl
        GetStringTypeExA(
            LCID     Locale,
            DWORD    dwInfoType,
            LPCSTR  lpSrcStr,
            int      cchSrc,
            LPWORD  lpCharType);
        BOOL
        __cdecl
        GetStringTypeExW(
            LCID     Locale,
            DWORD    dwInfoType,
            LPCWSTR  lpSrcStr,
            int      cchSrc,
            LPWORD  lpCharType);
        BOOL
        __cdecl
        GetStringTypeA(
            LCID     Locale,
            DWORD    dwInfoType,
            LPCSTR   lpSrcStr,
            int      cchSrc,
            LPWORD  lpCharType);
        BOOL
        __cdecl
        GetStringTypeW(
            DWORD    dwInfoType,
            LPCWSTR  lpSrcStr,
            int      cchSrc,
            LPWORD  lpCharType);
        int
        __cdecl
        FoldStringA(
            DWORD    dwMapFlags,
            LPCSTR  lpSrcStr,
            int      cchSrc,
            LPSTR  lpDestStr,
            int      cchDest);
        int
        __cdecl
        FoldStringW(
            DWORD    dwMapFlags,
            LPCWSTR  lpSrcStr,
            int      cchSrc,
            LPWSTR  lpDestStr,
            int      cchDest);
        BOOL
        __cdecl
        EnumSystemLocalesA(
            LOCALE_ENUMPROCA lpLocaleEnumProc,
            DWORD            dwFlags);
        BOOL
        __cdecl
        EnumSystemLocalesW(
            LOCALE_ENUMPROCW lpLocaleEnumProc,
            DWORD            dwFlags);
        BOOL
        __cdecl
        EnumSystemCodePagesA(
            CODEPAGE_ENUMPROCA lpCodePageEnumProc,
            DWORD              dwFlags);
        BOOL
        __cdecl
        EnumSystemCodePagesW(
            CODEPAGE_ENUMPROCW lpCodePageEnumProc,
            DWORD              dwFlags);
        BOOL SetSystemDefaultLCID(LCID Locale);
        BOOL SetOEMCP(UINT oemcp);
        BOOL SetACP(UINT acp);
        BOOL SetUserDefaultLCID(LCID Locale);
        BOOL GetNlsTables(LPVOID* lplpTable, DWORD dwFlags, LPDWORD lpOffset, LPDWORD lpSize);
        LANGID
        __cdecl
        GetSystemDefaultUILanguage(void);
        LANGID
        __cdecl
        GetUserDefaultUILanguage(void);
        BOOL
        __cdecl
        EnumUILanguagesA(
            UILANGUAGE_ENUMPROCA lpUILanguageEnumProc,
            DWORD dwFlags);
        BOOL
        __cdecl
        EnumUILanguagesW(
            UILANGUAGE_ENUMPROCW lpUILanguageEnumProc,
            DWORD dwFlags);
    }
    typedef DWORD CEOID;
    typedef CEOID* PCEOID;
    typedef struct  _CEGUID {
        DWORD Data1;
        DWORD Data2;
        DWORD Data3;
        DWORD Data4;
    } CEGUID, *PCEGUID;
    typedef struct _CENOTIFYREQUEST {
        DWORD dwSize;
        HWND  hwnd;
        DWORD dwFlags;
        HANDLE hHeap;
        DWORD  dwParam;
    } CENOTIFYREQUEST, *PCENOTIFYREQUEST;
    typedef struct _CENOTIFICATION {
        DWORD dwSize;
        DWORD dwParam;
        UINT  uType;
        CEGUID guid;
        CEOID  oid;
        CEOID  oidParent;
    } CENOTIFICATION, *PCENOTIFICATION;
    typedef struct _CEFILEINFO {
        DWORD    dwAttributes;
        CEOID    oidParent;
        WCHAR    szFileName[260];
        FILETIME ftLastChanged;
        DWORD    dwLength;
    } CEFILEINFO, *PCEFILEINFO;
    typedef struct _CEDIRINFO {
        DWORD dwAttributes;
        CEOID oidParent;
        WCHAR szDirName[260];
    } CEDIRINFO, *PCEDIRINFO;
    typedef struct STORE_INFORMATION {
        DWORD dwStoreSize;
        DWORD dwFreeSize;
    } STORE_INFORMATION, *LPSTORE_INFORMATION;
    BOOL GetStoreInformation(LPSTORE_INFORMATION lpsi);
    typedef DWORD CEPROPID;
    typedef CEPROPID* PCEPROPID;
    typedef struct _CERECORDINFO {
        CEOID  oidParent;
    } CERECORDINFO, *PCERECORDINFO;
    typedef struct _SORTORDERSPEC {
        CEPROPID  propid;
        DWORD     dwFlags;
    } SORTORDERSPEC, *PSORTORDERSPEC;
    typedef struct __CEDB_SORTORDERSPECEX {
        WORD      wVersion;
        WORD      wNumProps;
        WORD      wKeyFlags;
        WORD      wReserved;
        CEPROPID  rgPropID[3];
        DWORD     rgdwFlags[3];
    } __CEDB_SORTORDERSPECEX;

    typedef struct __EDB_SORTORDERSPECEX {
        WORD      wVersion;
        WORD      wNumProps;
        WORD      wKeyFlags;
        WORD      wReserved;
        CEPROPID  rgPropID[16];
        DWORD     rgdwFlags[16];
    } __EDB_SORTORDERSPECEX;

    typedef __CEDB_SORTORDERSPECEX SORTORDERSPECEX, *PSORTORDERSPECEX;
    typedef struct __CEDB_CEDBASEINFO {
        DWORD    dwFlags;
        WCHAR    szDbaseName[32];
        DWORD    dwDbaseType;
        WORD     wNumRecords;
        WORD     wNumSortOrder;
        DWORD    dwSize;
        FILETIME ftLastModified;
        SORTORDERSPEC rgSortSpecs[4];
    } __CEDB_CEDBASEINFO;

    typedef struct __EDB_CEDBASEINFO {
        DWORD    dwFlags;
        WCHAR    szDbaseName[128];
        DWORD    dwDbaseType;
        WORD     wNumRecords;
        WORD     wNumSortOrder;
        DWORD    dwSize;
        FILETIME ftLastModified;
        SORTORDERSPEC rgSortSpecs[16];
    } __EDB_CEDBASEINFO;

    typedef __CEDB_CEDBASEINFO CEDBASEINFO, *PCEDBASEINFO;
    typedef struct __CEDB_CEDBASEINFOEX {
        WORD     wVersion;
        WORD     wNumSortOrder;
        DWORD    dwFlags;
        WCHAR    szDbaseName[32];
        DWORD    dwDbaseType;
        DWORD    dwNumRecords;
        DWORD    dwSize;
        FILETIME ftLastModified;
        SORTORDERSPECEX rgSortSpecs[4];
    } __CEDB_CEDBASEINFOEX;

    typedef struct __EDB_CEDBASEINFOEX {
        WORD     wVersion;
        WORD     wNumSortOrder;
        DWORD    dwFlags;
        WCHAR    szDbaseName[128];
        DWORD    dwDbaseType;
        DWORD    dwNumRecords;
        DWORD    dwSize;
        FILETIME ftLastModified;
        SORTORDERSPECEX rgSortSpecs[16];
    } __EDB_CEDBASEINFOEX;

    typedef __CEDB_CEDBASEINFOEX CEDBASEINFOEX, *PCEDBASEINFOEX;
    typedef struct __CEDB_BY_HANDLE_DB_INFORMATION {
        WORD     wVersion;
        WORD     wReserved;
        CEGUID   guidVol;
        CEOID    oidDbase;
        __CEDB_CEDBASEINFOEX infDatabase;
    } __CEDB_BY_HANDLE_DB_INFORMATION;

    typedef struct __EDB_BY_HANDLE_DB_INFORMATION {
        WORD     wVersion;
        WORD     wReserved;
        CEGUID   guidVol;
        CEOID    oidDbase;
        __EDB_CEDBASEINFOEX infDatabase;
    } __EDB_BY_HANDLE_DB_INFORMATION;

    typedef __CEDB_BY_HANDLE_DB_INFORMATION BY_HANDLE_DB_INFORMATION, *LPBY_HANDLE_DB_INFORMATION;
    typedef struct _CEBLOB {
        DWORD           dwCount;
        LPBYTE          lpb;
    } CEBLOB, *PCEBLOB;

    typedef union _CEVALUNION {
        short           iVal;
        USHORT          uiVal;
        long            lVal;
        ULONG           ulVal;

        FILETIME        filetime;
        LPWSTR          lpwstr;
        CEBLOB          blob;

        BOOL            boolVal;
        double          dblVal;
    } CEVALUNION, *PCEVALUNION;

    typedef struct _CEPROPVAL {
        CEPROPID   propid;
        WORD       wLenData;
        WORD       wFlags;

        CEVALUNION val;
    } CEPROPVAL, *PCEPROPVAL;

    HANDLE CeFindFirstDatabase(DWORD dwClassID);
    CEOID CeFindNextDatabase(HANDLE hEnum);
    CEOID CeCreateDatabase(LPWSTR lpszname, DWORD dwClassID, WORD wNumSortOrder,
                           SORTORDERSPEC* rgSortSpecs);
    CEOID CeCreateDatabaseEx(PCEGUID pguid, CEDBASEINFO* pInfo);
    BOOL CeSetDatabaseInfo(CEOID oidDbase, CEDBASEINFO* pNewInfo);
    BOOL CeSetDatabaseInfoEx(PCEGUID pguid, CEOID oidDbase, CEDBASEINFO* pNewInfo);
    HANDLE CeOpenDatabase(PCEOID poid, LPWSTR lpszName, CEPROPID propid,
                          DWORD dwFlags, HWND hwndNotify);
    HANDLE CeOpenDatabaseEx(PCEGUID pguid, PCEOID poid, LPWSTR lpszName,
                            CEPROPID propid, DWORD dwFlags, CENOTIFYREQUEST* pReq);
    BOOL CeDeleteDatabase(CEOID oid);
    CEOID CeReadRecordProps(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID,
                            CEPROPID* rgPropID, LPBYTE* lplpBuffer,
                            LPDWORD lpcbBuffer);
    CEOID CeSeekDatabase(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue,
                         LPDWORD lpdwIndex);
    BOOL CeGetDBInformationByHandle(HANDLE hDbase, LPBY_HANDLE_DB_INFORMATION lpDBInfo);
    HANDLE CeFindFirstDatabaseEx(PCEGUID pguid, DWORD dwClassID);
    CEOID CeFindNextDatabaseEx(HANDLE hEnum, PCEGUID pguid);
    CEOID CeCreateDatabaseEx2(PCEGUID pguid, CEDBASEINFOEX* pInfo);
    BOOL CeSetDatabaseInfoEx2(PCEGUID pguid, CEOID oidDbase, CEDBASEINFOEX* pNewInfo);
    HANDLE CeOpenDatabaseEx2(PCEGUID pguid, PCEOID poid, LPWSTR lpszName,
                             SORTORDERSPECEX* pSort, DWORD dwFlags,
                             CENOTIFYREQUEST* pReq);
    BOOL CeDeleteDatabaseEx(PCEGUID pguid, CEOID oid);
    CEOID CeSeekDatabaseEx(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue,
                           WORD wNumVals, LPDWORD lpdwIndex);
    BOOL CeDeleteRecord(HANDLE hDatabase, CEOID oidRecord);
    CEOID CeReadRecordPropsEx(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID,
                              CEPROPID* rgPropID, LPBYTE* lplpBuffer,
                              LPDWORD lpcbBuffer, HANDLE hHeap);
    CEOID CeWriteRecordProps(HANDLE hDbase, CEOID oidRecord, WORD cPropID,
                             CEPROPVAL* rgPropVal);
    BOOL CeMountDBVol(PCEGUID pguid, LPWSTR lpszVol, DWORD dwFlags);
    BOOL CeUnmountDBVol(PCEGUID pguid);
    BOOL CeFlushDBVol(PCEGUID pguid);
    BOOL CeEnumDBVolumes(PCEGUID pguid, LPWSTR lpBuf, DWORD dwSize);
    BOOL CeFreeNotification(PCENOTIFYREQUEST pRequest, PCENOTIFICATION pNotify);
    void CeChangeDatabaseLCID(PCEGUID pguid, DWORD LCID);
    typedef struct __CEDB_CEOIDINFO {
        WORD  wObjType;
        WORD   wPad;
        union {
            CEFILEINFO  infFile;
            CEDIRINFO   infDirectory;
            __CEDB_CEDBASEINFO infDatabase;
            CERECORDINFO infRecord;
        };
    } __CEDB_CEOIDINFO;
    typedef struct __EDB_CEOIDINFO {
        WORD  wObjType;
        WORD   wPad;
        union {
            CEFILEINFO  infFile;
            CEDIRINFO   infDirectory;
            __EDB_CEDBASEINFO infDatabase;
            CERECORDINFO infRecord;
        };
    } __EDB_CEOIDINFO;
    typedef __CEDB_CEOIDINFO CEOIDINFO, *PCEOIDINFO;
    typedef struct __CEDB_CEOIDINFOEX {
        WORD  wVersion;
        WORD  wObjType;
        union {
            CEFILEINFO    infFile;
            CEDIRINFO     infDirectory;
            __CEDB_CEDBASEINFOEX infDatabase;
            CERECORDINFO  infRecord;
        };
    } __CEDB_CEOIDINFOEX;
    typedef struct __EDB_CEOIDINFOEX {
        WORD  wVersion;
        WORD  wObjType;
        union {
            CEFILEINFO    infFile;
            CEDIRINFO     infDirectory;
            __EDB_CEDBASEINFOEX infDatabase;
            CERECORDINFO  infRecord;
        };
    } __EDB_CEOIDINFOEX;
    typedef __CEDB_CEOIDINFOEX CEOIDINFOEX, *PCEOIDINFOEX;
    BOOL CeOidGetInfoEx2(PCEGUID pguid, CEOID oid, CEOIDINFOEX* oidInfo);
    BOOL CeOidGetInfoEx(PCEGUID pguid, CEOID oid, CEOIDINFO* oidInfo);
    BOOL CeOidGetInfo(CEOID oid, CEOIDINFO* oidInfo);
}

extern "C" {
    __declspec(noalias) __declspec(restrict) void* __cdecl _recalloc(void* _Memory, size_t _Count, size_t _Size);
    typedef char __static_assert_t[(sizeof(unsigned int) <= 8)];
    __inline void* _MarkAllocaS(void* _Ptr, unsigned int _Marker) {
        if (_Ptr) {
            *((unsigned int*)_Ptr) = _Marker;
            _Ptr = (char*)_Ptr + 8;
        }

        return _Ptr;
    }
    __declspec(noalias) __inline void __cdecl _freea(void* _Memory) {
        unsigned int _Marker;

        if (_Memory) {
            _Memory = (char*)_Memory - 8;
            _Marker = *(unsigned int*)_Memory;

            if (_Marker == 0xDDDD) {
                free(_Memory);
            } else if (_Marker != 0xCCCC) {
                (void)(((("Corrupted pointer passed to _freea", 0))) || (1 != _CrtDbgReportW(2, L"..\\include\\ARMV4I\\malloc.h", 104, 0, L"(\"Corrupted pointer passed to _freea\", 0)")) || (__debugbreak(), 0));
            }
        }
    }
}

extern "C" long __cdecl InterlockedIncrement(long*);
extern "C" long __cdecl InterlockedDecrement(long*);
extern "C" long __cdecl InterlockedCompareExchange(long*, long, long);
extern "C" long __cdecl InterlockedExchange(long*, long);
extern "C" long __cdecl InterlockedExchangeAdd(long*, long);
#pragma pack(push,8)
extern "C" void _ReadWriteBarrier();
#pragma intrinsic( _ReadWriteBarrier )

