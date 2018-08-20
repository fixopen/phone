#ifndef __UTIL_TEXT_JSONPARSER_H__
#define __UTIL_TEXT_JSONPARSER_H__

#include <vector>
#include "StringOp.h"
#include "../Fsm.h"
#include "../../Json/Value.h"

namespace Util {
    namespace Text {
        class JsonParser : public FSM<JsonParser> {
        public:
            enum State {
                sIdle,
                sObject,
                sName,
                sNameEnd,
                sValue,
                sValueEnd,
                sObjectEnd,
                sArray,
                sArrayEnd,
                sN,
                sNU,
                sNUL,
                sNULL,
                sF,
                sFA,
                sFAL,
                sFALS,
                sFALSE,
                sT,
                sTR,
                sTRU,
                sTRUE,
                sNumeric,
                sString,
            };
            enum Event {
                eChr,
                eSpace,
                eDigital,
                eStringMark = L'"',
                eObjectStart = L'{',
                eNameValue = L':',
                eFieldDeli = L',',
                eObjectEnd = L'}',
                eArrayStart = L'[',
                eArrayEnd = L']',
                eLetterN = L'n',
                eLetterU = L'u',
                eLetterL = L'l',
                eLetterF = L'f',
                eLetterA = L'a',
                eLetterS = L's',
                eLetterE = L'e',
                eLetterT = L't',
                eLetterR = L'r',
            };
            JsonParser();
            Json::Value* const Parse(std::wstring const& content);
        private:
            Event const getEventByChar_(wchar_t const c) const;
        };
    }
}

#endif //__UTIL_TEXT_JSONPARSER_H__
