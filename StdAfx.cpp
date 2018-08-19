// stdafx.cpp : source file that includes just the standard includes
//  MultimediaPhone.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

const TCHAR* ptcFileCall = _T("/NandFlash/Resource/testcall.log");
const TCHAR* ptcFileDbTest = _T("/NandFlash/Resource/testdb.log");
const TCHAR* ptcFileMem = _T("/NandFlash/Resource/testMem.log");

// copyfileDlg dialog

const PTCHAR ptchLocalDb = _T("");
const PTCHAR ptchLocalMp3 = _T("/NandFlash/resource/my_music/");
const PTCHAR ptchLocalPhoto = _T("/NandFlash/resource/my_photo/");
const PTCHAR ptchLocalBell = _T("/NandFlash/resource/my_ring/");
const PTCHAR ptchLocalShow = _T("/NandFlash/resource/my_show/");
const PTCHAR ptcPlayList[] = {
    _T("/UsbDisk/mediaphone/my_music/playlist.pls"), _T("/StorageCard/mediaphone/my_music/playlist.pls")
};

TCHAR* _3g_mms_dir = L"/NandFlash/resource/3g/mms/" ;
char* nandflash_resource_dir = "/NandFlash/resource";
TCHAR* _3g_photo_dir = L"/NandFlash/resource/3G/mms/my_photo/" ;
const TCHAR* SABTD_dir = _T("/NandFlash/Resource/res_dat/SABTD_CEDLL.dll");
char* sa_licen_file = "/NandFlash/Resource/res_dat/sa_license.dat";
char* my_ring_dir_a = "/NandFlash/resource/my_ring/";
char* hive_tip_wav = "/NandFlash/Resource/tip1.wav";
char* hive_res_mobile_txt = "/NandFlash/Resource/res/mobile.txt" ;
TCHAR* hive_tip2_wav = _T("/NandFlash/Resource/tip2.wav");
const char* mmp_db_a = "/NandFlash/resource/res_dat/mmp.db";
const char* res_hl_file = "/NandFlash/Resource/res/res_hl.dat";
char* hive_res_dir = "/NandFlash/Resource/res/%c.wav";
TCHAR* hive_res_dial_file = L"/NandFlash/Resource/res/dial.wav";
char* usbdisk_dir = "/usbdisk";
TCHAR* poweron_gif = L"/NandFlash/resource/¿ª»ú1.gif";
const CString csFlashRecordPath = _T("/NandFlash/resource/MY_RECORD/");
const CString csStorageCardRecordPath = _T("/StorageCard/MY_RECORD/");
const std::string ssFlashRecordPath = "/NandFlash/resource/MY_RECORD/";
const std::string ssStorageCardRecordPath = "/StorageCard/MY_RECORD/";
const CString csStarageCard = _T("/StorageCard/");
const std::string ssStarageCard = "/StorageCard/";
const CString csUsbDisk = _T("/UsbDisk/");
char* log_file = "/NandFlash/resource/log.txt";
char* log_dat_file = "/NandFlash/resource/log.dat";

TCHAR* common_dll = L"/NandFlash/resource/tcpmp/common.dll";
TCHAR* mplayer_exe = L"/NandFlash/resource/tcpmp/mplayer.exe";
TCHAR* mplayer_exe_1 = L"/NandFlash/resource/tcpmp/mplayer1.exe";
TCHAR* mplayer_exe_2 = L"/NandFlash/resource/tcpmp/mplayer2.exe";
TCHAR* iesample_exe = L"/NandFlash/resource/tcpmp/iesample.exe";

TCHAR* win_comm_dll = L"/windows/common.dll";
TCHAR* win_mplayer_exe = L"/windows/mplayer.exe";
TCHAR* win_mplayer_exe_1 = L"/windows/mplayer1.exe";
TCHAR* win_mplayer_exe_2 = L"/windows/mplayer2.exe";
TCHAR* win_iesample_exe = L"/windows/iesample.exe";

TCHAR* sabtd_cedll_dll = L"/NandFlash/resource/res_dat/SABTD_CEDLL.dll";
TCHAR* short_sabtd_cedll_dll = L"/SABTD_CEDLL.dll";

TCHAR* sa_lice_dat = L"/NandFlash/resource/res_dat/sa_license.dat";
TCHAR* short_sa_lice_dat = L"/sa_license.dat";

TCHAR* usb_mediaphone_dir = L"/UsbDisk/mediaphone";
TCHAR* mmp_db_file = L"/NandFlash/resource/res_dat/mmp.db";
TCHAR* mmp_db_file_usb = L"/UsbDisk/mediaphone/mmp.db";

char* playlist_pls_file = "/NandFlash/resource/my_music/playlist.pls";

const TCHAR* ptchExtDb[] = {
    _T("/UsbDisk/mediaphone/mmp.db"), _T("/StorageCard/mediaphone/mmp.db")
};
const TCHAR* ptchExtMp3[] = {
    _T("/UsbDisk/mediaphone/my_music"), _T("/StorageCard/mediaphone/my_music")
};
const TCHAR* ptchExtPhoto[] = {
    _T("/UsbDisk/mediaphone/my_photo"), _T("/StorageCard/mediaphone/my_photo")
};
const TCHAR* ptchExtBell[] = {
    _T("/UsbDisk/mediaphone/my_ring"), _T("/StorageCard/mediaphone/my_ring")
};
const TCHAR* ptchExtShow[] = {
    _T("/UsbDisk/mediaphone/my_show"), _T("/StorageCard/mediaphone/my_show")
};

const TCHAR* mediaphone = _T("/UsbDisk/mediaphone");
const TCHAR* mediaphone_backup = _T("/UsbDisk/mediaphone/backup");
const TCHAR* mediaphone_backup_flashdrv = _T("/UsbDisk/mediaphone/backup/flashdrv");
const TCHAR* sdcard = _T("/UsbDisk/mediaphone/backup/SDcard");
const TCHAR* my_record_f = _T("/UsbDisk/mediaphone/backup/flashdrv/my_record");
const TCHAR* my_record_sd = _T("/UsbDisk/mediaphone/backup/SDcard/my_record");
const TCHAR* my_record_nd = _T("/NandFlash/resource/my_record");

const TCHAR* mediaphone_sd = L"/StorageCard/mediaphone";
const TCHAR* mediaphone_backup_sd = L"/StorageCard/mediaphone/backup";
const TCHAR* mediaphone_backup_flashdrv_stor = L"/StorageCard/mediaphone/backup/flashdrv";
const TCHAR* sdcard_sd = L"/StorageCard/mediaphone/backup/SDcard";
const TCHAR* my_record_bk = L"/StorageCard/mediaphone/backup/flashdrv/my_record";
const TCHAR* my_record_sd_sd = L"/StorageCard/mediaphone/backup/SDcard/my_record";
char* my_photo_dir_list = "/NandFlash/Resource/my_photo/screensave.pls";
TCHAR* my_video_dir = _T("/NandFlash/resource/MY_VIDEO/");
char* my_video_ply_list = "/NandFlash/resource/my_video/playlist.pls";
TCHAR* hive_res_firstchar = _T("/NandFlash/Resource/res/firstchar.txt");
char* my_note_dir = "/NandFlash/Resource/my_note/";
TCHAR* hive_mynote_1_bmp = _T("/NandFlash/Resource/my_note/1.bmp");
const char* my_phone_dir = "/NandFlash/Resource/my_photo/";
TCHAR* my_ring_dir = L"/NandFlash/Resource/my_ring/";    
const char* flashdrv_3g_mms = "/NandFlash/resource/3g/mms/%s/";
char* weblist_files = "/NandFlash/resource/my_web/weblist.txt";



void Dprintf(CString s) {
#ifdef _DEBUG
    wprintf(s);
    printf("\r\n");
#else
    //  wprintf(s);
    //  printf("\r\n");
#endif
}

int Dprintf(const char* format, ...) {
#ifdef _DEBUG
    char buf[4096] = {0};   
    va_list ap;   

    va_start(ap, format);   
    //int len = vsprintf_s(buf, 4096, format, ap);
    int len = vsprintf(buf, format, ap);
    va_end(ap);   

    printf(buf); 

#else
    //    char   buf[4096];   
    //    va_list   ap;   
    //    
    //    va_start(ap,   format);   
    //    int   len   =   vsprintf(buf,   format,   ap);   
    //    va_end(ap);   
    //    
    //    printf(buf); 


#endif

    return   0;
}   

int DMemprintf(char* title) {
    MEMORYSTATUS mem;     
    mem.dwLength = sizeof(mem);     
    GlobalMemoryStatus(&mem);  
#ifdef _DEBUG
    Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
    return mem.dwAvailPhys; //mem.dwAvailPhys;
#else
#endif
    return mem.dwAvailPhys;
}

int DMemprintf(char* title, DWORD& total, DWORD& avalid) {
#ifdef _DEBUG
    MEMORYSTATUS mem;     
    mem.dwLength = sizeof(mem);     
    GlobalMemoryStatus(&mem);     
    Dprintf("%s: T/A:  %x B /%x B V T/A: %x B /%x B\r\n", title, mem.dwTotalPhys, mem.dwAvailPhys, mem.dwTotalVirtual, mem.dwAvailVirtual);//mem.dwTotalPhys, mem.dwAvailPhys); 
    total = mem.dwTotalPhys;
    avalid = mem.dwAvailPhys;
    return mem.dwAvailPhys; //mem.dwAvailPhys;
#else
#endif
    return 0;
}

void DWriteLog(char* tile, unsigned char* pData, int len) {
    //FILE* file = 0;
    //fopen_s(&file, "/NandFlash/resource/test.log", "a+");
    FILE* file = fopen("/NandFlash/resource/test.log", "a+");
    if (file) {
        if (strlen(tile)) {
            fwrite(tile, sizeof(char), strlen(tile), file);
        }
    }
    for (int i = 0; i < len; i++) {
        char txt[6] = {
            0
        };
        //sprintf_s(txt, 6, "%x ", *(pData + i));
        sprintf(txt, "%x ", *(pData + i));
        fwrite(txt, sizeof(char), strlen(txt), file);
    }
    char* gTxt = "\r\n";
    fwrite(gTxt, sizeof(char), strlen(gTxt), file); 
    fclose(file);
}

void DWriteDat(char* tile, unsigned char* pData, int len) {
    //FILE* file = 0;
    //fopen_s(&file, "/NandFlash/resource/test.dat", "a+");
    FILE* file = fopen("/NandFlash/resource/test.dat", "a+");
    if (file) {
        if (strlen(tile)) {
            fwrite(tile, sizeof(char), strlen(tile), file);
        }
    }
    fwrite(pData, sizeof(char), len, file); 
    fclose(file);
}
