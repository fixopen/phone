#ifndef __DATA_DATAACCESS_H__
#define __DATA_DATAACCESS_H__

#pragma warning(disable: 4786)
#include <vector>
#include <string>
#include <algorithm>

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"
#include "../Util/StringOp.h"
#include "../inc/sqlite3.h"

namespace Data
{
    enum Direction
    {
        dNull,
        dUp,
        dDown,
    };

	inline int getIndexByName(int count, char* columnNames[], char* columnName)
	{
		int result = -1;
		for (int i = 0; i < count; ++i)
		{
			if (strcmp(columnNames[i], columnName) == 0)
			{
				result = i;
				break;
			}
		}
		return result;
	}

	inline sqlite3* GetDB(void)
	{
		static sqlite3* result = 0;
		if (!result)
		{
			sqlite3_open("\\flashdrv\\mmp.db", &result);
		}
		return result;
	}

	typedef char* DataManipulatorError;
    typedef void (*Indication)(DataManipulatorError& e);
	template<typename T>
		class DataAccess
	{
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
			: id_(0)
		{
		}
		void id(int id)
		{
			id_ = id;
		}

		int id(void) const
		{
			return id_;
		}
	public:
        typedef void (*ModifyFieldByDB)(int argc, char** argv, char** columnName, boost::shared_ptr<T> item);
        struct Parameter
        {
            Parameter(ModifyFieldByDB modifyFieldByDBinst, int aPageSize)
                : modifyFieldByDB(modifyFieldByDBinst)
                , pageSize(aPageSize)
            {
                if (pageSize != 0)
                    items.reserve(pageSize);
            }

            std::vector<boost::shared_ptr<T> > items;
            ModifyFieldByDB modifyFieldByDB;
            int pageSize;
        };
	public:
		//database access method
        static bool const ExecCommand(std::string const cmd)
        {
            bool result = true;
            char* errorMessage = 0;
            int rc = sqlite3_exec(db_, cmd.c_str(), 0, 0, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
                result = false;
            }
            return result;
        }

        static std::vector<boost::shared_ptr<T> > GetDatasByFilter(std::string const filter, ModifyFieldByDB modifyFieldByDB, Direction const dir = dNull, int const id = 0, int const pageSize = 0) //int const maxCount = -1, 
        {
            std::string cmd = "SELECT";
            //if (maxCount != -1)
            //    cmd += " TOP " + Util::StringOp::FromInt(maxCount);
            cmd += " * FROM [";
            cmd += tableName_;
            cmd += "]";
            bool hasCond = false;
            if (filter != "")
            {
                cmd += " WHERE ";
                cmd += filter;
                hasCond = true;
            }
			std::string appendCond;
            if ((dir == dUp) || (dir == dDown))
            {
                if (hasCond)
                {
                    appendCond += " AND ";
                }
                else
                {
                    appendCond += " WHERE ";
                }
                appendCond += "id";
                switch (dir)
                {
                case dUp:
                    appendCond += " > ";
                    break;
                case dDown:
                    appendCond += " < ";
                    break;
                }
                appendCond += Util::StringOp::FromInt(id);
            }
            cmd += appendCond;
            cmd += " ORDER BY id ";
            if (dir == dUp)
            {
                cmd += "ASC";
            }
            else
            {
                cmd += "DESC";
            }
            Parameter p(modifyFieldByDB, pageSize);
            char* errorMessage = 0;
            rowCount_ = 0;
            int rc = sqlite3_exec(db_, cmd.c_str(), rowProcess_, &p, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
			std::vector<boost::shared_ptr<T> >(p.items).swap(p.items);
            if (dir == dUp)
            {
                std::reverse(p.items.begin(), p.items.end());
            }
            return p.items;
        }

        static boost::shared_ptr<T> const GetDataById(int id, ModifyFieldByDB modifyFieldByDB)
        {
			std::string filter = "id = " + Util::StringOp::FromInt(id);
			std::vector<boost::shared_ptr<T> > results = GetDatasByFilter(filter, modifyFieldByDB, 1);
            if (results.size() == 1)
                return results[0];
            return 0;
        }

        static bool const RemoveDatasByFilter(std::string const filter)
        {
            std::string cmd = "DELETE FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "")
            {
                cmd += " WHERE ";
                cmd += filter;
            }
            return ExecCommand(cmd);
        }

        static bool const RemoveDataById(int const id)
        {
			std::string cmd = "DELETE FROM [";
            cmd += tableName_;
            cmd += "] WHERE id = ";
            cmd += Util::StringOp::FromInt(id);
            return ExecCommand(cmd);
        }

        static unsigned int const GetDataCount(std::string const filter)
        {
            unsigned int result = -1;
            std::string cmd = "SELECT COUNT(*) AS integerValue FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "")
            {
                cmd += " WHERE ";
                cmd += filter;
            }
            char* errorMessage = 0;
            int rc = sqlite3_exec(db_, cmd.c_str(), getIntegerValue_, &result, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
            return result;
        }

        static unsigned int const GetCurrentId()
        {
            unsigned int result = -1;
			std::string cmd = "SELECT MAX(id) AS integerValue FROM [";
            cmd += tableName_;
            cmd += "]";
            char* errorMessage = 0;
            int rc = sqlite3_exec(db_, cmd.c_str(), getIntegerValue_, &result, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
            return result;
        }

        static std::string tableName(void)
        {
            return tableName_;
        }
    private:
        static void log_(DataManipulatorError& e)
        {
			/*
            Util::Log log;
            log.who = Util::Environment::getUserInfo();
            log.what = e; //errorMessage
            log.when = Util::Environment::getSystemInfo()->getCurrentTime();
            log.module = Util::Module::getCurrentModule(); //moduleInfo;
            log.writeLog();
			*/
        }

        static int rowCount_;

        static Indication indication_;

        static int rowProcess_(void* parameter, int argc, char** argv, char** columnName)
        {
            Parameter* p = reinterpret_cast<Parameter*>(parameter);
            boost::shared_ptr<T> item(new T(/*true*/));
            //item->setId(id);
            p->modifyFieldByDB(argc, argv, columnName, item);
            p->items.push_back(item);
            ++rowCount_;
            int result = 0;
            if (p->pageSize != 0)
            {
                if (rowCount_ >= p->pageSize)
                {
                    result = 1;
                }
            }
            return result;
        }

        static int getIntegerValue_(void* parameter, int argc, char** argv, char** columnName)
        {
            int* result = reinterpret_cast<int*>(parameter);
            *result = atoi(argv[Data::getIndexByName(argc, columnName, "integerValue")]);
            return 0;
        }
	public:
		/*
		std::vector<SmartPtr<T> > GetDatasByFilter(TADOQuery* const qry, std::string const tableName, std::string const filter, void (*modifyFieldByDB)(TADOQuery* const qry, ItemType* const item))
		{
			std::vector<SmartPtr<T> > result;
			while (!qry->Eof)
			{
				T* item = new T();
				modifyFieldByDB(qry, item);
				SmartPtr smartItem(item);
				result.push_back(smartItem);
				qry->Next();
			}
			return result;
		}

		void RemoveDatasByFilter(TADOQuery* const qry, std::string const tableName, std::string const filter)
		{
			string cmd = "DELETE FROM [";
			cmd += tableName;
			cmd += "]";
			if (filter != "")
			{
				cmd += " WHERE ";
				cmd += filter;
			}
			ExecCommand(qry, cmd);
		}

		size_t const GetDataCount(TADOQuery* const qry, std::string const tableName)
		{
			unsigned int result = 0;
			string cmd = "SELECT COUNT(*) AS recordCount FROM [";
			cmd += tableName;
			cmd += "]";
			qry->Close();
			qry->SQL->Clear();
			qry->SQL->Add(cmd.c_str());
			qry->Open();
			result = qry->FieldByName("recordCount")->AsInteger;
			qry->Close();
			return result;
		}
		*/
	};
#if 0
id_: 程序分配 = 程序自增和处理，临时对象和永久对象的差异
     数据库序列 = Insert以后从数据库中取出，那时才有效
#endif
}

#endif //__DATA_DATAACCESS_H__