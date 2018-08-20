#pragma once

class SIMDevice {
public:
    static SIMDevice* const Instance();
//private:
	CWnd *m_pOwnerCwd;
	SIMDevice(CWnd *pWnd);
	BYTE    m_chSendBuff[64];
	int     m_nSendLength;

	HANDLE m_hPort;
	BOOL m_bRecvFlag;
	BOOL m_bExit;
	void SetExitTread(){m_bExit = TRUE;}
	void SetRecvFlag(){m_bRecvFlag = TRUE;}	
	void SetOwner(CWnd *pWnd){m_pOwnerCwd = pWnd;}
	void WritePort(const unsigned char * const data, int dataLength, int recvLen);
	BOOL Reset();

	
	int mLength;				//response lenth
	BYTE m_chReadBuff[1024];		//databuff
	int  m_nReadCount;			//datalen
	int  m_nRecvLen;
	int mRecordCount;
};
