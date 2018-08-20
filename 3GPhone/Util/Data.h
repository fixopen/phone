#ifndef __DATA_DATAACCESS_H__
#define __DATA_DATAACCESS_H__

#include <vector>
#include <string>
#include <algorithm>

#include "Symbols.h"
#include "DateTime.h"
#include "SmartPtr.h"
#include "StringOp.h"
#include "../../sqlite/sqlite3.h"

namespace Util {
    class BaseData {
    public:
        enum SeqenceRelation {
            srNull,
            srAsc,
            srDesc,
            srError
        };

        typedef void (*ErrorProcess)(char* e);

        BaseData();
        static void InitDatabase(std::wstring const& dbName);
        static void UninitDatabase();
        static bool const ExecCommand(std::wstring const& cmd);
        static bool const RemoveDatasByFilter(std::wstring const& tableName, std::wstring const& filter);
        static bool const RemoveDataById(std::wstring const& tableName, int const id);
        static unsigned int const GetDataCount(std::wstring const& tableName, std::wstring const& filter);
        static unsigned int const GetCurrentId(std::wstring const& tableName);
        static int const GetIndexByName(int count, char* columnNames[], char* columnName);

        void id(int const id);
        int const id() const;
    protected:
        static int getIntegerValue_(void* parameter, int argc, char** argv, char** columnName);
        static void log_(char* e);

        static sqlite3* db_;
        static CRITICAL_SECTION* mutex_;
        static ErrorProcess indication_;
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
    };

    template<typename T>
    class DataAccess : public BaseData {
    public:
        typedef void (*ModifyFieldByDB)(int argc, char** argv, char** columnName, Util::shared_ptr<T> item);
        struct Parameter {
            Parameter(ModifyFieldByDB aModifyFieldByDB, int aOffset, int aPageSize)
                : modifyFieldByDB(aModifyFieldByDB)
                , offset(aOffset)
                , count(aPageSize) {
                    if (count != -1) {
                        items.reserve(count);
                    }
            }

            std::vector<Util::shared_ptr<T> > items;
            ModifyFieldByDB modifyFieldByDB;
            int offset;
            int count;
        };

        static std::vector<Util::shared_ptr<T> > const GetDatasByFilter(std::wstring const filter, ModifyFieldByDB modifyFieldByDB, std::wstring const& orderFieldName = L"id", SeqenceRelation const sr = srNull, int const offset = 0, int const pageSize = -1) {
            std::string cmd = "SELECT * FROM [";
            cmd += StringOp::ToUTF8(tableName_);
            cmd += "]";
            if (filter != L"") {
                cmd += " WHERE ";
                cmd += StringOp::ToUTF8(filter);
            }
            switch (sr) {
            case srAsc:
                if (orderFieldName != L"") {
                    cmd += " ORDER BY " + StringOp::ToUTF8(orderFieldName) + " ASC";
                }
                break;
            case srDesc:
                if (orderFieldName != L"") {
                    cmd += " ORDER BY " + StringOp::ToUTF8(orderFieldName) + " DESC";
                }
                break;
            //case srNull:
            //case srError:
            default:
                break;
            }
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
            Util::shared_ptr<T> result;
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id);
            std::vector<Util::shared_ptr<T> > results = GetDatasByFilter(filter, modifyFieldByDB, L"", srNull, 0, 1);
            if (results.size() == 1) {
                result = results[0];
            }
            return result;
        }

        static bool const RemoveDatasByFilter(std::wstring const& filter) {
            return BaseData::RemoveDatasByFilter(tableName_, filter);
        }

        static bool const RemoveDataById(int const id) {
            return BaseData::RemoveDataById(tableName_, id);
        }

        static unsigned int const GetDataCount(std::wstring const& filter) {
            return BaseData::GetDataCount(tableName_, filter);
        }

        static unsigned int const GetCurrentId() {
            return BaseData::GetCurrentId(tableName_);
        }
    protected:
        static std::wstring tableName_;
        static int offset_;
        static int count_;

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
#if 0
                char** args = (char**)malloc((argc + 1) * sizeof(char*));
			    memset(args, 0, (argc + 1) * sizeof(char*));
			    for (int i = 0; i < argc; ++i) {
				    if (argv[i]) {
					    std::string oarg = argv[i];
                        std::string arg = StringOp::FromUnicode(StringOp::FromUTF8(oarg));
					    args[i] = (char*)malloc(arg.length() + 1);
					    strcpy(args[i], arg.c_str());
				    }
			    }
                p->modifyFieldByDB(argc, args, columnName, item);
			    for (int ai = 0; ai < argc; ++ai) {
				    if (args[ai]) {
					    free(args[ai]);
				    }
			    }
			    free(args);
#else
                p->modifyFieldByDB(argc, argv, columnName, item);
#endif
                p->items.push_back(item);
                ++count_;
            }
            return result;
        }
    };

#if 0
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
                    if (count != -1) {
                        items.reserve(count);
                    }
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
            std::string ncmd = Util::StringOp::ToUTF8(Util::StringOp::ToUnicode(cmd));
            ::EnterCriticalSection(sqliteSection());
            int rc = sqlite3_exec(db_, ncmd.c_str(), 0, 0, &errorMessage);
            ::LeaveCriticalSection(sqliteSection());
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

        static std::vector<Util::shared_ptr<T> > const GetDatasByFilter(std::string const filter, ModifyFieldByDB modifyFieldByDB, std::string const& orderFieldName = "id", SeqenceRelation const sr = srNull, int const offset = 0, int const pageSize = -1) {
            std::string cmd = "SELECT * FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "") {
                cmd += " WHERE ";
                cmd += filter;
            }
            switch (sr) {
            case srAsc:
                if (orderFieldName != "") {
                    cmd += " ORDER BY " + orderFieldName + " ASC";
                }
                break;
            case srDesc:
                if (orderFieldName != "") {
                    cmd += " ORDER BY " + orderFieldName + " DESC";
                }
                break;
            //case srNull:
            //case srError:
            default:
                break;
            }
            Parameter p(modifyFieldByDB, offset, pageSize);
            char* errorMessage = 0;
            offset_ = 0;
            count_ = 0;
            std::string ncmd = Util::StringOp::ToUTF8(Util::StringOp::ToUnicode(cmd));
            int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);
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
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
            int rc = sqlite3_exec(db_, ncmd.c_str(), getIntegerValue_, &result, &errorMessage);
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
            std::string ncmd = Util::StringOp::ToUTF8(Util::StringOp::ToUnicode(cmd));
            int rc = sqlite3_exec(db_, ncmd.c_str(), getIntegerValue_, &result, &errorMessage);
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
#if 1
			    char** args = (char**)malloc((argc + 1) * sizeof(char*));
			    memset(args, 0, (argc + 1) * sizeof(char*));
			    for (int i = 0; i < argc; ++i) {
				    if (argv[i]) {
					    std::string oarg = argv[i];
                        std::string arg = StringOp::FromUnicode(StringOp::FromUTF8(oarg));
					    args[i] = (char*)malloc(arg.length() + 1);
					    strcpy(args[i], arg.c_str());
				    }
			    }
                p->modifyFieldByDB(argc, args, columnName, item);
			    for (int ai = 0; ai < argc; ++ai) {
				    if (args[ai]) {
					    free(args[ai]);
				    }
			    }
			    free(args);
#else
                p->modifyFieldByDB(argc, argv, columnName, item);
#endif
                p->items.push_back(item);
                ++count_;
            }
            return result;
        }

        static int getIntegerValue_(void* parameter, int argc, char** argv, char** columnName) {
            int* result = reinterpret_cast<int*>(parameter);
            *result = atoi(argv[getIndexByName(argc, columnName, "integerValue")]);
            return 0;
        }
    };
#endif
}

#endif //__DATA_DATAACCESS_H__
