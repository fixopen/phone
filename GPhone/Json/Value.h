#ifndef __JSON_VALUE_H__
#define __JSON_VALUE_H__

#include <string>
#include <map>
#include "../Util/Text/StringOp.h"

namespace Json {
    struct Value {
        enum Type {
            tNull,
            tBool,
            tNumeric,
            tString,
            tObject,
            tArray,
        };
        Type type;
        virtual std::wstring const ToString() const;
    protected:
        Value();
    };

    struct NullValue : Value {
        virtual std::wstring const ToString() const {
            return L"null";
        }
    };

    struct BoolValue : Value {
        bool value;
        virtual std::wstring const ToString() const {
            return value ? L"true" : L"false";
        }
    };

    struct NumericValue : Value {
        double value;
        virtual std::wstring const ToString() const {
            return Util::Text::StringOp::FromDouble(value);
        }
    };

    struct StringValue : Value {
        std::wstring value;
        virtual std::wstring const ToString() const {
            return L"\"" + value + L"\"";
        }
    };

    struct ObjectValue : Value {
        std::map<std::wstring, Value*> value;
        virtual std::wstring const ToString() const {
            std::wstring result = L"{";
            for (std::map<std::wstring, Value*>::const_iterator i = value.begin(); i != value.end(); ++i) {
                result += L"\"" + i->first + L"\" : " + i->second->ToString() + L",";
            }
            result = result.substr(0, result.length() - 1);
            result += L"}";
            return result;
        }
    };

    struct ArrayValue : Value {
        std::vector<Value*> value;
        virtual std::wstring const ToString() const {
            std::wstring result = L"[";
            for (size_t i = 0; i < value.size(); ++i) {
                result += value[i]->ToString() + L",";
            }
            result = result.substr(0, result.length() - 1);
            result += L"]";
            return result;
        }
    };
};

#endif //__JSON_VALUE_H__
