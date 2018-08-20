#ifndef __DATA_DATAACCESS_H__
#define __DATA_DATAACCESS_H__

//#pragma warning(disable: 4786)
#include <vector>
#include <string>
#include <algorithm>

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"
#include "../Util/StringOp.h"
#include "../../sqlite/sqlite3.h"

namespace Data {
    extern CRITICAL_SECTION sqliteSetion_;

    enum SeqenceRelation {
        srNull,
        srAsc,
        srDesc,
        srError
    };

    inline int getIndexByName(int count, char* columnNames[], char* columnName) {
        int result = -1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(columnNames[i], columnName) == 0) {
                result = i;
                break;
            }
        }
        return result;
    }

    inline sqlite3* GetDB(void) {
        static sqlite3* result = 0;
        if (!result) {
            sqlite3_open("mmp.sqlite", &result);
            ::InitializeCriticalSection(&sqliteSetion_);
        }
        return result;
    }

    typedef char* DataManipulatorError;
    typedef void (*Indication)(DataManipulatorError& e);
    template<typename T>
    class DataAccess {
    private:
        int id_;
        /*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
        */
        //database access info
        static std::string tableName_;
        static sqlite3* db_;
    public:
        DataAccess()
            : id_(0) {
        }
        void id(int id) {
            id_ = id;
        }

        int id() const {
            return id_;
        }
    public:
        typedef void (*ModifyFieldByDB)(int argc, char** argv, char** columnName, Util::shared_ptr<T> item);
        struct Parameter {
            Parameter(ModifyFieldByDB aModifyFieldByDB, int aOffset, int aPageSize)
                : modifyFieldByDB(aModifyFieldByDB)
                , offset(aOffset)
                , count(aPageSize) {
                    if (count != -1)
                        items.reserve(count);
            }

            std::vector<Util::shared_ptr<T> > items;
            ModifyFieldByDB modifyFieldByDB;
            int offset;
            int count;
        };
    public:
        //database access method
        static bool const ExecCommand(std::string const cmd) {
            bool result = true;
            char* errorMessage = 0;
            ::EnterCriticalSection(&sqliteSetion_);
            int rc = sqlite3_exec(db_, cmd.c_str(), 0, 0, &errorMessage);
            ::LeaveCriticalSection(&sqliteSetion_);
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

        static std::vector<Util::shared_ptr<T> > GetDatasByFilter(std::string const filter, ModifyFieldByDB modifyFieldByDB, std::string const& orderFieldName, SeqenceRelation const sr, int const offset = 0, int const pageSize = -1) {
            std::string cmd = "SELECT * FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "") {
                cmd += " WHERE ";
                cmd += filter;
            }
            if (orderFieldName != "") {
                cmd += " ORDER BY " + orderFieldName;
            }
            switch (sr) {
            case srAsc:
                cmd += " ASC";
                break;
            case srDesc:
                cmd += " DESC";
                break;
            //case srNull:
            //case srError:
            default:
                cmd += " DESC";
                break;
            }
            //cmd += ";COMMIT";
            Parameter p(modifyFieldByDB, offset, pageSize);
            char* errorMessage = 0;
            offset_ = 0;
            count_ = 0;
            int rc = sqlite3_exec(db_, cmd.c_str(), rowProcess_, &p, &errorMessage);
            if (rc != SQLITE_OK) {
                log_(errorMessage);
                if (indication_) {
                    indication_(errorMessage);
                }
            }
            if (errorMessage) {
                sqlite3_free(errorMessage);
            }
            return p.items;
        }


        static Util::shared_ptr<T> const GetDataById(int id, ModifyFieldByDB modifyFieldByDB) {
            std::string filter = "id = " + Util::StringOp::FromInt(id);
            std::vector<Util::shared_ptr<T> > results = GetDatasByFilter(filter, modifyFieldByDB, "", srNull, 0, 1);
            Util::shared_ptr<T> result;
            if (results.size() == 1) {
                result = results[0];
            }
            return result;
        }

        static bool const RemoveDatasByFilter(std::string const filter) {
            std::string cmd = "DELETE FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "") {
                cmd += " WHERE ";
                cmd += filter;
            }
            return ExecCommand(cmd);
        }

        static bool const RemoveDataById(int const id) {
            std::string cmd = "DELETE FROM [";
            cmd += tableName_;
            cmd += "] WHERE id = ";
            cmd += Util::StringOp::FromInt(id);
            return ExecCommand(cmd);
        }

        static unsigned int const GetDataCount(std::string const filter) {
            unsigned int result = -1;
            std::string cmd = "SELECT COUNT(*) AS integerValue FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "") {
                cmd += " WHERE ";
                cmd += filter;
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

        static unsigned int const GetCurrentId() {
            unsigned int result = -1;
            std::string cmd = "SELECT MAX(id) AS integerValue FROM [";
            cmd += tableName_;
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

        static std::string tableName(void) {
            return tableName_;
        }
    private:
        static void log_(DataManipulatorError& e) {
            //write to log file for debug
        }

        static int offset_;
        static int count_;

        static Indication indication_;

        static int rowProcess_(void* parameter, int argc, char** argv, char** columnName) {
            int result = 0;
            Parameter* p = reinterpret_cast<Parameter*>(parameter);
            if (offset_ < p->offset) { //skip header
                //do nothing
                ++offset_;
            } else if ((count_ != 0) && (p->count != -1) && (count_ > p->count)) { //skip footer
                //don't continue via return 1
                result = 1;
            } else {
                Util::shared_ptr<T> item(new T(/*true*/));
                p->modifyFieldByDB(argc, argv, columnName, item);
                p->items.push_back(item);
                ++count_;
            }
            return result;
        }

        static int getIntegerValue_(void* parameter, int argc, char** argv, char** columnName) {
            int* result = reinterpret_cast<int*>(parameter);
            *result = atoi(argv[Data::getIndexByName(argc, columnName, "integerValue")]);
            return 0;
        }
    };
}

#endif //__DATA_DATAACCESS_H__