#ifndef __TELEPHONE_LOGICAL_CALLID_H__
#define __TELEPHONE_LOGICAL_CALLID_H__

namespace Telephone {
    namespace Logical {
        int const CALLID_NUM_LEN = 32;
        int const CALLID_NAME_LEN = 32;

        //struct CALLID_INFO {
        //    unsigned char type;  // CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX
        //    unsigned char timestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
        //    unsigned char month;  // 1 - 12
        //    unsigned char day;  // 0 - 31
        //    unsigned char hour;  // 0 - 23
        //    unsigned char minute;  // 0 - 59
        //    unsigned char numberstatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
        //    char number[CALLID_NUM_LEN];  // string ending with '\0'
        //    unsigned char namestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
        //    char name[CALLID_NAME_LEN];  // string ending with '\0'
        //};

        struct CallId {
            //FSK 包类型
            enum PacketType {
                //CALLID_TYPE_NONE = 0x00,
                CALLID_TYPE_DTMF = 0x01,
                CALLID_TYPE_FSK_SIMPLE = 0x04, // YDN069-1997 标准规定
                CALLID_TYPE_FSK_COMPLEX = 0x80, // YDN069-1997 标准规定
            };

            //不能显示来电原因（用于应用）
            enum Status {
                CALLID_OK = 0,
                CALLID_NONE = 1,
                CALLID_FORBID = 2,
                CALLID_NOTGET = 3,
                CALLID_ERROR = 4,
            };
            CallId(unsigned char const* const buffer = 0);
            static CallId const Parse(unsigned char const* const buffer);
            //~CallId();
        	PacketType type;
        	Status timeStatus;
        	unsigned char month;  // 1 - 12
        	unsigned char day;  // 0 - 31
        	unsigned char hour;  // 0 - 23
        	unsigned char minute;  // 0 - 59
        	Status numberStatus;
        	char number[CALLID_NUM_LEN];  // string ending with '\0'
        	Status nameStatus;
        	char name[CALLID_NAME_LEN];  // string ending with '\0'
        private:
            bool decodeDTMFCallIDPackage_(unsigned char const* const buffer);
            bool decodeFSKCallIDPackage_(unsigned char const* const buffer);
            void decodeFSKSimpleParam_(unsigned char const* const data, int const dataLength);
            void decodeFSKComplexParam_(unsigned char const* const parameter);
        };

        typedef CallId CALLID_INFO;

        //bool DecodeCallIDPackage(unsigned char* buf, CALLID_INFO* pcallid);
    }
}

#endif //__TELEPHONE_LOGICAL_CALLID_H__
