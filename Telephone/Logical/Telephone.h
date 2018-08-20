#ifndef __LOGICAL_TELEPHONE_H__
#define __LOGICAL_TELEPHONE_H__

#pragma once

namespace Logical {
	typedef unsigned char BYTE;
	typedef struct ManufactureInfo {
		BYTE deviceID[32];
		BYTE manuInfo[32];
		BYTE flashID[128];
		BYTE serialNum[32];
		BYTE model[32];
		BYTE date[32];
		BYTE osVer[32];
		BYTE bootVer[4];
		BYTE appVer[32];
		BYTE hardVer[32];
		BYTE MD5Key[64];
		BYTE bootCkSum[16];
		BYTE res[12];
	} MANUINFO, *PMANUINFO;

	class Telephone {
	public:
		Telephone();
		~Telephone();
	};
}

#endif //__LOGICAL_TELEPHONE_H__
