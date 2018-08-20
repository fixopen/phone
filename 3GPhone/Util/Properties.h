#pragma once

#include "Util/Data.h"

class Properties : public Util::DataAccess<Properties> {
public:
    enum Type {
        tDefault,
        tCurrent,
    };
    static CString const Get(CString const& name);
    static void Set(CString const& name, CString const& value);
    void Update() const; //sync to database
    void Insert(); //insert new instance to database

    CString name;
    CString value;
    CString catagory;
    Type type;
    static std::vector<Type> types;
    static Type currentType;
private:
    static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Properties> item);
};
