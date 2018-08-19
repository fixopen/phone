// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_)
#define AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#if defined(_WIN32_WCE) && (_WIN32_WCE >= 211) && (_AFXDLL)
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <string>
int Dprintf(const char* format, ...);
int DMemprintf(char* title, DWORD& t, DWORD& a);
int DMemprintf(char* title);
void DWriteLog(char* tile, unsigned char* pData, int len);
void DWriteDat(char* tile, unsigned char* pData, int len);
void Dprintf(CString s);

extern char* my_note_dir;
extern TCHAR* hive_mynote_1_bmp;
extern const TCHAR* ptcFileCall;
extern const TCHAR* ptcFileDbTest;
extern const TCHAR* ptcFileMem;

extern const PTCHAR ptchLocalDb;
extern const PTCHAR ptchLocalMp3;
extern const PTCHAR ptchLocalPhoto;
extern const PTCHAR ptchLocalBell;
extern const PTCHAR ptchLocalShow;

extern TCHAR* _3g_mms_dir ;
extern TCHAR* mmp_db_file ;
extern const char* mmp_db_a;
extern const char* res_hl_file;
extern char* nandflash_resource_dir;
extern char* hive_res_dir;
extern TCHAR* hive_res_dial_file;
extern char* usbdisk_dir;
extern TCHAR* poweron_gif;
extern TCHAR* _3g_photo_dir;
extern const TCHAR* SABTD_dir;
extern char* sa_licen_file ;
extern char* my_ring_dir_a;
extern char* hive_tip_wav ;
extern char* hive_res_mobile_txt;
extern TCHAR* hive_tip2_wav;
extern const CString csFlashRecordPath;
extern const CString csStorageCardRecordPath;
extern const std::string ssFlashRecordPath;
extern const std::string ssStorageCardRecordPath;
extern const CString csStarageCard;
extern const std::string ssStarageCard;
extern const CString csUsbDisk;
extern char* log_file;
extern char* log_dat_file;

extern  TCHAR* common_dll;
extern  TCHAR* mplayer_exe;
extern  TCHAR* mplayer_exe_1;
extern  TCHAR* mplayer_exe_2;
extern  TCHAR* iesample_exe;

extern TCHAR* win_comm_dll;
extern TCHAR* win_mplayer_exe;
extern TCHAR* win_mplayer_exe_1;
extern TCHAR* win_mplayer_exe_2;
extern TCHAR* win_iesample_exe;

extern TCHAR* sabtd_cedll_dll;
extern TCHAR* short_sabtd_cedll_dll;

extern TCHAR* sa_lice_dat ;
extern TCHAR* short_sa_lice_dat;
extern TCHAR* usb_mediaphone_dir;
extern TCHAR* mmp_db_file;
extern TCHAR* mmp_db_file_usb;
extern char* playlist_pls_file;

extern const TCHAR* mediaphone;
extern const TCHAR* mediaphone_backup;
extern const TCHAR* mediaphone_backup_flashdrv;
extern const TCHAR* sdcard;
extern const TCHAR* my_record_f;
extern const TCHAR* my_record_sd;
extern const TCHAR* my_record_nd;

extern const TCHAR* mediaphone_sd;
extern const TCHAR* mediaphone_backup_sd;
extern const TCHAR* mediaphone_backup_flashdrv_stor;
extern const TCHAR* sdcard_sd;
extern const TCHAR* my_record_bk;
extern const TCHAR* my_record_sd_sd;
extern char* my_photo_dir_list;
extern  TCHAR* my_video_dir;
extern char* my_video_ply_list;
extern TCHAR* hive_res_firstchar;
extern char* my_note_dir;
extern TCHAR* hive_mynote_1_bmp;
extern const char* my_phone_dir;
extern TCHAR* my_ring_dir;    
extern const char* flashdrv_3g_mms;
extern char* weblist_files;

extern const TCHAR* ptchExtDb[];
extern const TCHAR* ptchExtMp3[];
extern const TCHAR* ptchExtPhoto[];
extern const TCHAR* ptchExtBell[];
extern const TCHAR* ptchExtShow[];
extern const PTCHAR ptcPlayList[];



//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FF64B944_C14C_470D_BDF3_384C9F10FDD3__INCLUDED_)
