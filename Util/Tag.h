#ifndef __UTIL_TAG_H__
#define __UTIL_TAG_H__

#include "Data.h"

namespace Util {
    class ObjectCategory : public DataAccess<ObjectCategory> {
    public:
        static int const GetIdByName(std::wstring const& name);
        static std::vector<shared_ptr<ObjectCategory> > const Select(std::wstring const& filter = L"");
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        std::wstring name;
    };

    class Tag : public DataAccess<Tag> {
    public:
        enum Type {
            tDefault,
            tCurrent,
        };
        static std::wstring const Get(std::wstring const& name);
        static void Set(std::wstring const& name, std::wstring const& value);
        void Update() const; //sync to database
        void Insert(); //insert new instance to database

        int objectCategoryId;
        std::wstring value;
        Type type;
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Tag>& item);
    };
}

#endif //__UTIL_TAG_H__
