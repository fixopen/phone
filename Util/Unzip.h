#ifndef __UTIL_UNZIP_H__
#define __UTIL_UNZIP_H__

#include <winnt.h>
DECLARE_HANDLE(HZIP);
#include <windef.h>
typedef DWORD ZRESULT;

struct ZIPENTRY {
    int index;                 // index of this file within the zip
    TCHAR name[MAX_PATH];      // filename within the zip
    DWORD attr;                // attributes, as in GetFileAttributes.
    FILETIME atime,ctime,mtime;// access, create, modify filetimes
    long comp_size;            // sizes of item, compressed and uncompressed. These
    long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
};


HZIP OpenZip(const TCHAR *fn, const char *password);
HZIP OpenZip(void *z, unsigned int len, const char *password);
HZIP OpenZipHandle(HANDLE h, const char *password);

ZRESULT GetZipItem(HZIP hz, int index, ZIPENTRY *ze);

ZRESULT FindZipItem(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);

ZRESULT UnzipItem(HZIP hz, int index, const TCHAR *fn);
ZRESULT UnzipItem(HZIP hz, int index, void *z, unsigned int len);
ZRESULT UnzipItemHandle(HZIP hz, int index, HANDLE h);

ZRESULT SetUnzipBaseDir(HZIP hz, const TCHAR *dir);

ZRESULT CloseZip(HZIP hz);

unsigned int FormatZipMessage(ZRESULT code, TCHAR *buf, unsigned int len);

// These are the result codes:
#define ZR_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define ZR_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define ZR_GENMASK    0x0000FF00
#define ZR_NODUPH     0x00000100     // couldn't duplicate the handle
#define ZR_NOFILE     0x00000200     // couldn't create/open the file
#define ZR_NOALLOC    0x00000300     // failed to allocate some resource
#define ZR_WRITE      0x00000400     // a general error writing to the file
#define ZR_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define ZR_MORE       0x00000600     // there's still more data to be unzipped
#define ZR_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define ZR_READ       0x00000800     // a general error reading the file
#define ZR_PASSWORD   0x00001000     // we didn't get the right password to unzip the file
// The following come from mistakes on the part of the caller
#define ZR_CALLERMASK 0x00FF0000
#define ZR_ARGS       0x00010000     // general mistake with the arguments
#define ZR_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define ZR_MEMSIZE    0x00030000     // the memory size is too small
#define ZR_FAILED     0x00040000     // the thing was already failed when you called this function
#define ZR_ENDED      0x00050000     // the zip creation has already been closed
#define ZR_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define ZR_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define ZR_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define ZR_BUGMASK    0xFF000000
#define ZR_NOTINITED  0x01000000     // initialisation didn't work
#define ZR_SEEK       0x02000000     // trying to seek in an unseekable file
#define ZR_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define ZR_FLATE      0x05000000     // an internal error in the de/inflation code

ZRESULT CloseZipU(HZIP hz);
unsigned int FormatZipMessageU(ZRESULT code, TCHAR *buf, unsigned int len);
bool IsZipHandleU(HZIP hz);

#ifdef _zip_H
#define CloseZip(hz) (IsZipHandleU(hz) ? CloseZipU(hz) : CloseZipZ(hz))
#else
#define CloseZip CloseZipU
#define FormatZipMessage FormatZipMessageU
#endif

#endif //__UTIL_UNZIP_H__
