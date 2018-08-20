#pragma once

#define CALLID_NUM_LEN		32
#define CALLID_NAME_LEN		32

struct CALLID_INFO {
	BYTE type;  // CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX
	BYTE timestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	BYTE month;  // 1 - 12
	BYTE day;  // 0 - 31
	BYTE hour;  // 0 - 23
	BYTE minute;  // 0 - 59
	BYTE numberstatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char number[CALLID_NUM_LEN];  // string ending with '\0'
	BYTE namestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char name[CALLID_NAME_LEN];  // string ending with '\0'
};

//class CallId {
//public:
//    CallId(char const* const buffer);
//    //~CallId();
//	BYTE type;  // CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX
//	BYTE timestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
//	BYTE month;  // 1 - 12
//	BYTE day;  // 0 - 31
//	BYTE hour;  // 0 - 23
//	BYTE minute;  // 0 - 59
//	BYTE numberstatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
//	char number[CALLID_NUM_LEN];  // string ending with '\0'
//	BYTE namestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
//	char name[CALLID_NAME_LEN];  // string ending with '\0'
//};

//typedef CallId CALLID_INFO;

bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
