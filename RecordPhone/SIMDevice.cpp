#include "StdAfx.h"
#include "SIMDevice.h"

#include "Util/ControlCode.h"

#define SIM_IOCODE_BASE 2050
#define SIM_IO_OFFSET 100
//cmdreset
#define	SIM_RESET CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x40,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
//cmdgetinfo
#define SIM_GETSIMCARDINFO CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x45,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define SIM_WRITE CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x48,METHOD_IN_DIRECT,FILE_ANY_ACCESS)                       

#define WM_SIMDATA_OK 9067
#define WM_SIMDATA_ERROR 9068

BYTE gSIMRS232[1024];
int gSIMLength;

SIMDevice* const SIMDevice::Instance() {
    static SIMDevice* result = new SIMDevice(new CWnd());
    return result;
}

SIMDevice::SIMDevice(CWnd* pWnd) {
	mLength = 0;				//response lenth
	m_nReadCount = 0;			//datalen
	mRecordCount = 0;

	m_pOwnerCwd = pWnd;
	m_bRecvFlag = FALSE;
	m_bExit = TRUE;

	m_hPort = CreateFile (_T("SIM1:"), // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		CREATE_ALWAYS,// Create New File as any case
		0,            // Port attributes
		NULL);
    if (m_hPort == INVALID_HANDLE_VALUE) {
	}
}

void ParseSIMData(unsigned char const* const data, unsigned int const length) {
	for (int i=0; i < length; ++i) {
        if (data[i] == 0x9F) {
            SIMDevice::Instance()->mLength = data[i + 1];
        }
	}
	memcpy(SIMDevice::Instance()->m_chReadBuff, data, length);
	SendMessage(SIMDevice::Instance()->m_pOwnerCwd->m_hWnd, WM_SIMDATA_OK, (WPARAM)length, (LPARAM)(SIMDevice::Instance()->m_chReadBuff));
	memset(SIMDevice::Instance()->m_chReadBuff, 0, 1024);
	SIMDevice::Instance()->m_nReadCount = 0;
}

void ReadThreadPrc() {
	int gIndex = 0;
	while (!SIMDevice::Instance()->m_bExit && gIndex++ < 3) {
		if (SIMDevice::Instance()->m_bRecvFlag) {
			for (int i = 0; i < SIMDevice::Instance()->m_nSendLength; ++i) {
			}
			DWORD size;
			DeviceIoControl(SIMDevice::Instance()->m_hPort, SIM_WRITE, SIMDevice::Instance()->m_chSendBuff, SIMDevice::Instance()->m_nSendLength, gSIMRS232 , SIMDevice::Instance()->m_nRecvLen, &size, NULL);
			if (size == SIMDevice::Instance()->m_nRecvLen) {
				ParseSIMData(gSIMRS232, size);
				SIMDevice::Instance()->m_bRecvFlag = FALSE;
				SIMDevice::Instance()->m_bExit = TRUE;
			} else if (size != SIMDevice::Instance()->m_nRecvLen) {
				SendMessage(SIMDevice::Instance()->m_pOwnerCwd->m_hWnd, WM_SIMDATA_ERROR, 0, 0);
				SIMDevice::Instance()->m_bRecvFlag = FALSE;
				SIMDevice::Instance()->m_bExit = TRUE;
			}
			::Sleep(10);
        } else {
			::Sleep(10);
        }
	}
}

void SIMDevice::WritePort(const unsigned char * const data, int dataLength, int recvLen) {
	if (m_hPort != INVALID_HANDLE_VALUE) {
        if (m_bExit) { //创建线程
			m_bExit = FALSE;
			HANDLE hSimReadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadThreadPrc, NULL, 0, NULL);
		}
		memcpy(m_chSendBuff, data, dataLength);
	    m_nSendLength = dataLength;
		m_nRecvLen = recvLen;
		m_bRecvFlag = TRUE;
	}
}

BOOL SIMDevice::Reset() {
	if (m_hPort != INVALID_HANDLE_VALUE) {
		DWORD size;
		BOOL Reset_OK = FALSE;
		int nCount = 0;
		while (!Reset_OK && nCount++ < 2) {
			DeviceIoControl(m_hPort, SIM_RESET, NULL, 0, &Reset_OK , sizeof(BOOL), &size, NULL);
			Sleep(5);
		}
		return Reset_OK;
	}
	return FALSE;
}
