#include "JsonParser.h"

namespace Util {
    namespace Text {
        JsonParser::JsonParser() {
            registerRule(sIdle, eObjectStart, sObject/*, &JsonParser::startObject_*/);
            registerRule(sIdle, eSpace, sIdle);
            setStartState(sIdle);
        }

        Json::Value* const JsonParser::Parse(std::wstring const& content) {
            Json::Value* result = 0;
            for (size_t i = 0; i < content.size(); ++i) {
                fireEvent(getEventByChar_(content[i]), 0);
            }
            //std::wstring c = Util::StringOp::Trim(content);
            //if (c.empty()) { //empty string
            //    //do noting
            //} else if (c == L"null") {
            //    Value* v = new Field();
            //    v->type = tNull;
            //    result.push_back(v);
            //} else if (c == L"true") {
            //    Value* v = new Field();
            //    v->type = Field::tBool;
            //    v->boolValue = true;
            //    result.push_back(v);
            //} else if (c == L"false") {
            //    Value* v = new Field();
            //    v->type = Field::tBool;
            //    v->boolValue = false;
            //    result.push_back(v);
            //} else if (StringOp::FromDouble(StringOp::ToDouble(c)) == c) {
            //    Value* v = new Field();
            //    v->type = Field::tNumeric;
            //    v->numericValue = StringOp::ToDouble(c);
            //    result.push_back(v);
            //} else if (c[0] == L"\"") {
            //    Value* v = new Field();
            //    v->type = Field::tString;
            //    v->value = c;
            //    result.push_back(v);
            //} else if (c[0] != L'{') { //single value
            //    Value* v = new Field();
            //    v->type = Value::tObject;
            //    v->objectValue = c;
            //    result.push_back(v);
            //} else if (c[0] != L'[') { //single value
            //    Value* v = new Field();
            //    v->type = Value::tArray;
            //    v->arrayValue = c;
            //    result.push_back(v);
            //}
            return result;
        }

        JsonParser::Event const JsonParser::getEventByChar_(wchar_t const c) const {
            JsonParser::Event result;
            return result;
        }
    }
}
