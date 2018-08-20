#include "Data.h"
#include "Log.h"

namespace Util {
    sqlite3* BaseData::db_ = 0;
    CRITICAL_SECTION* BaseData::mutex_ = 0;
    BaseData::ErrorProcess BaseData::indication_ = 0;

    BaseData::BaseData()
        : id_(0) {
    }
    
    void BaseData::InitDatabase(std::wstring const& dbName) {
        int rc = sqlite3_open(StringOp::ToUTF8(dbName).c_str(), &db_);
        if (rc == SQLITE_OK) {
            mutex_ = new CRITICAL_SECTION();
            ::InitializeCriticalSection(mutex_);
            indication_ = 0;
        } else {
            //log_(static_cast<char*>("open database failure!"));
            //abort(); //not support by WinCE
        }
    }
    
    void BaseData::UninitDatabase() {
        DeleteCriticalSection(mutex_);
        delete mutex_;
        mutex_ = 0;
        sqlite3_close(db_);
    }

    bool const BaseData::ExecCommand(std::wstring const& cmd) {
        bool result = true;
        char* errorMessage = 0;
        std::string ncmd = StringOp::ToUTF8(cmd);
        ::EnterCriticalSection(mutex_);
        int rc = sqlite3_exec(db_, ncmd.c_str(), 0, 0, &errorMessage);
        ::LeaveCriticalSection(mutex_);
        if (rc != SQLITE_OK) {
            log_(errorMessage);
            if (indication_) {
                indication_(errorMessage);
            }
            result = false;
        }
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        return result;
    }

    bool const BaseData::RemoveDatasByFilter(std::wstring const& tableName, std::wstring const& filter) {
        std::wstring cmd = L"DELETE FROM [";
        cmd += tableName;
        cmd += L"]";
        if (filter != L"") {
            cmd += L" WHERE ";
            cmd += filter;
        }
        return ExecCommand(cmd);
    }

    bool const BaseData::RemoveDataById(std::wstring const& tableName, int const id) {
        std::wstring filter = L"id = " + StringOp::FromInt(id);
        return RemoveDatasByFilter(tableName, filter);
    }

    unsigned int const BaseData::GetDataCount(std::wstring const& tableName, std::wstring const& filter) {
        unsigned int result = -1;
        std::string cmd = "SELECT COUNT(*) AS integerValue FROM [";
        cmd += StringOp::ToUTF8(tableName);
        cmd += "]";
        if (filter != L"") {
            cmd += " WHERE ";
            cmd += StringOp::ToUTF8(filter);
        }
        char* errorMessage = 0;
        int rc = sqlite3_exec(db_, cmd.c_str(), getIntegerValue_, &result, &errorMessage);
        if (rc != SQLITE_OK) {
            log_(errorMessage);
            if (indication_) {
                indication_(errorMessage);
            }
        }
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        return result;
    }

    unsigned int const BaseData::GetCurrentId(std::wstring const& tableName) {
        unsigned int result = -1;
        std::string cmd = "SELECT MAX(id) AS integerValue FROM [";
        cmd += StringOp::ToUTF8(tableName);
        cmd += "]";
        char* errorMessage = 0;
        int rc = sqlite3_exec(db_, cmd.c_str(), getIntegerValue_, &result, &errorMessage);
        if (rc != SQLITE_OK) {
            log_(errorMessage);
            if (indication_) {
                indication_(errorMessage);
            }
        }
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        return result;
    }

    int const BaseData::GetIndexByName(int count, char* columnNames[], char* columnName) {
        int result = -1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(columnNames[i], columnName) == 0) {
                result = i;
                break;
            }
        }
        return result;
    }

    void BaseData::log_(char* e) {
        Log::Log(e);
    }

    int BaseData::getIntegerValue_(void* parameter, int argc, char** argv, char** columnName) {
        int* result = reinterpret_cast<int*>(parameter);
        *result = atoi(argv[GetIndexByName(argc, columnName, "integerValue")]);
        return 0;
    }

    void BaseData::id(int const id) {
        id_ = id;
    }

    int const BaseData::id() const {
        return id_;
    }
}