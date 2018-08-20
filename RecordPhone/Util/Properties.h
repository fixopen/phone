#ifndef __UTIL_PROPERTIES_H__
#define __UTIL_PROPERTIES_H__

#include "Data.h"

namespace Util {
    class Properties : public DataAccess<Properties> {
    public:
        enum Type {
            tDefault,
            tCurrent,
        };
        static std::wstring const Get(std::wstring const& name);
        static void Set(std::wstring const& name, std::wstring const& value);
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        static void RestoreDefault(std::wstring const& category);
        static std::vector<Type> types;
        static Type currentType;
    private:
        std::wstring name;
        std::wstring value;
        std::wstring category;
        Type type;
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Properties>& item);
    };
}

#endif //__UTIL_PROPERTIES_H__
