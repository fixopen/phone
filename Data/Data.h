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
	extern CRITICAL_SECTION sqliteSetion_;

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
			::InitializeCriticalSection(&sqliteSetion_);
		}
		return result;
	}

	inline sqlite3 * OpenDB(void)
	{
		static sqlite3 * mp_db=0;

		if(!mp_db)
		{
			sqlite3_open("\\flashdrv\\mmp.db", &mp_db);
			::InitializeCriticalSection(&sqliteSetion_);
		}

		return mp_db;
	}
	inline void CloseDB(sqlite3 * mp_db)
	{
		if(mp_db)
		{
			sqlite3_close(mp_db);

			mp_db=NULL;
		}
	}

	typedef char* DataManipulatorError;
    typedef void (*Indication)(DataManipulatorError& e);
	template<typename T>
	class DataAccess
	{
	private:
		int id_;
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
				, offset(0)
                , pageSize(aPageSize)
            {
                if (pageSize != 0)
                items.reserve(pageSize);
            }

            std::vector<boost::shared_ptr<T> > items;
            ModifyFieldByDB modifyFieldByDB;
			int offset;
            int pageSize;
        };
	public:
		//database access method

        static bool const ExecCommand(std::string const cmd)
        {
            bool result = true;
            char* errorMessage = 0;
			::EnterCriticalSection(&sqliteSetion_);
			std::string ncmd;
			Util::StringOp::gb2312TOutf8(cmd,ncmd);
            int rc = sqlite3_exec(db_, ncmd.c_str(), 0, 0, &errorMessage);
			::LeaveCriticalSection(&sqliteSetion_);
            if (rc != SQLITE_OK)
            {	
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
                result = false;
            }
			if (errorMessage) {

				sqlite3_free(errorMessage);
			}
            return result;
        }

		static bool const Compression()
		{
			bool result = true;
            char* errorMessage = 0;
			::EnterCriticalSection(&sqliteSetion_);
            int rc = sqlite3_exec(db_, "vacuum", 0, 0, &errorMessage);
			::LeaveCriticalSection(&sqliteSetion_);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
                result = false;
            }
			if (errorMessage) {

				sqlite3_free(errorMessage);
			}
            return result;
		}

		static bool const startBegin()
		{
			bool result = true;
			
			char* errorMessage = 0;	
			
			if(db_)
			{   
				int rc = sqlite3_exec(db_, "BEGIN", 0, 0, &errorMessage);
				
				if (rc != SQLITE_OK)
				{
					log_(errorMessage);
					
					if (indication_)
						
						indication_(errorMessage);
					
					result = false;
				}
			}
			
			return result; 
		}
		static bool const endCommit()
		{
			bool result = true;
			
			char* errorMessage = 0;	
			
			if(db_)
			{  
				int rc = sqlite3_exec(db_, "COMMIT", 0, 0, &errorMessage);
				
				if (rc != SQLITE_OK)
				{
					log_(errorMessage);
					
					if (indication_)
						
						indication_(errorMessage);
					
					result = false;
				}
			}
			
			return result;
		}

		static std::vector<boost::shared_ptr<T> > GetDatasByFilterOrderBy(std::string const filter, ModifyFieldByDB modifyFieldByDB, std::string order, Direction const dir = dUp)
        {
			std::string cmd = "SELECT";
			cmd += " * FROM [";
			cmd += tableName_;
			cmd += "]";
			if (filter != "")
			{
				cmd += " WHERE ";
				cmd += filter;
			}
			
			cmd += " ORDER BY " + order;
			if (dir == dUp)
			{
				cmd += " ASC";
			}
			
			else
			{
				cmd += " DESC";
			}
			
			Parameter p(modifyFieldByDB, 0);
			char* errorMessage = 0;
			rowCount_ = 0;
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
			int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);
			if (rc != SQLITE_OK)
			{
				log_(errorMessage);
				if (indication_)
					indication_(errorMessage);
			}
			
			if (errorMessage) {
				sqlite3_free(errorMessage);
				
			}    //20090221
			return p.items;
        }
		
        static std::vector<boost::shared_ptr<T> > GetDatasByFilter(std::string const filter, ModifyFieldByDB modifyFieldByDB, Direction const dir = dNull, int const id = 0, int const pageSize = 0) //int const maxCount = -1, 
        {
			std::string cmd = "SELECT";
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
					appendCond += " > ";    //20100123
					break;
				case dDown:
					appendCond += " < ";	//20100123
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
			std::string ncmd;
			Util::StringOp::gb2312TOutf8(cmd,ncmd);
			int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);
			if (rc != SQLITE_OK)
			{
				log_(errorMessage);
				if (indication_)
					indication_(errorMessage);
			}
			if (errorMessage) {
				sqlite3_free(errorMessage);
			}
			std::vector<boost::shared_ptr<T> >(p.items).swap(p.items);
			if (dir == dUp)
			{
				std::reverse(p.items.begin(), p.items.end());
			}
			return p.items;
        }

		static std::vector<boost::shared_ptr<T> > GetDatasByFilterAndLimit(\
			std::string const filter, ModifyFieldByDB modifyFieldByDB, \
			int const offset, int const count,std::string order,bool isAsc) 
        {
			std::string cmd = "SELECT * FROM [";
            cmd += tableName_;
            cmd += "]";
            if (filter != "")
            {
                cmd += " WHERE ";
                cmd += filter;
            }
			
			if (order != "")
			{
				cmd += " ORDER BY " ;
				cmd += order ;
				
				if (isAsc)
				{
					cmd += " ASC ";
				}
				else
				{
					cmd += " DESC";
				}
			}
			
			//偏移
			cmd += " Limit ";
			cmd += Util::StringOp::FromInt(offset);
			cmd += ",";
			cmd += Util::StringOp::FromInt(count);
			//
			
            Parameter p(modifyFieldByDB, count);
			p.offset = 0;
            char* errorMessage = 0;
			offset_ = 0;
            rowCount_ = 0;
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
            int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
			if (errorMessage) {
				sqlite3_free(errorMessage);
			}
			std::vector<boost::shared_ptr<T> >(p.items).swap(p.items);
            return p.items;
        }
		
		static std::vector<boost::shared_ptr<T> > GetDatasByFilterAndPageLimit(\
			std::string const filter, ModifyFieldByDB modifyFieldByDB,\
			int const offset, int const count)
		{
			std::string cmd = "SELECT * FROM [";
			cmd += tableName_;
			cmd += "]";
			if (filter != "")
			{
				cmd += " WHERE ";
				cmd += filter;
			}
			cmd += " ORDER BY id DESC";
			
			//偏移
			cmd += " Limit ";
			cmd += Util::StringOp::FromInt(offset);
			cmd += ",";
			cmd += Util::StringOp::FromInt(count);
			//
			
			Parameter p(modifyFieldByDB, count);
			p.offset = 0;
			char* errorMessage = 0;
			offset_ = 0;
			rowCount_ = 0;
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
			int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);
			if (rc != SQLITE_OK)
			{
				log_(errorMessage);
				if (indication_)
					indication_(errorMessage);
			}
			if (errorMessage) {
				sqlite3_free(errorMessage);
			}
			std::vector<boost::shared_ptr<T> >(p.items).swap(p.items);
			return p.items;
        }

        static std::vector<boost::shared_ptr<T> > GetDatasByFilterAndPageInfo(std::string const filter, ModifyFieldByDB modifyFieldByDB, int const offset, int const count)
        {
			std::string cmd = "SELECT * FROM [";
			cmd += tableName_;
			cmd += "]";
			if (filter != "")
			{
				cmd += " WHERE ";
				cmd += filter;
			}
			cmd += " ORDER BY id DESC";
			Parameter p(modifyFieldByDB, count);
			p.offset = offset;
			char* errorMessage = 0;
			offset_ = 0;
			rowCount_ = 0;
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
			int rc = sqlite3_exec(db_, ncmd.c_str(), rowProcess_, &p, &errorMessage);

			if (rc != SQLITE_OK)
			{
				log_(errorMessage);
				if (indication_)
					indication_(errorMessage);
			}
			if (errorMessage) {
				sqlite3_free(errorMessage);
			}
			std::vector<boost::shared_ptr<T> >(p.items).swap(p.items);
			return p.items;
        }
		
		static boost::shared_ptr<T> const GetDataById(int id, ModifyFieldByDB modifyFieldByDB)
        {
			std::string filter = "id = " + Util::StringOp::FromInt(id);
			std::vector<boost::shared_ptr<T> > results = GetDatasByFilter(filter, modifyFieldByDB);
			return results[0];
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
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
            int rc = sqlite3_exec(db_, ncmd.c_str(), getIntegerValue_, &result, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
			if (errorMessage) {
				sqlite3_free(errorMessage);
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
			std::string ncmd = Util::StringOp::ToUTF8(cmd);
            int rc = sqlite3_exec(db_, ncmd.c_str(), getIntegerValue_, &result, &errorMessage);
            if (rc != SQLITE_OK)
            {
                log_(errorMessage);
                if (indication_)
                    indication_(errorMessage);
            }
			if (errorMessage) {
				sqlite3_free(errorMessage);
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
        }

		static int offset_;
        static int rowCount_;

        static Indication indication_;

        static int rowProcess_(void* parameter, int argc, char** argv, char** columnName)
        {
            int result = 0;
            Parameter* p = reinterpret_cast<Parameter*>(parameter);
			if (p->offset != 0) {
				++offset_;
				if (offset_ < p->offset)
					return result;
			}
            boost::shared_ptr<T> item(new T(/*true*/));
            
			char** args = (char**)malloc((argc + 1) * sizeof(char*));
			memset(args, 0, (argc + 1) * sizeof(char*));
			for (int i = 0; i < argc; ++i) {
				if (argv[i]) {
					std::string oarg = argv[i];
					std::string arg = Util::StringOp::FromUTF8(oarg);
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

            p->items.push_back(item);
            ++rowCount_;
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
			int i = Data::getIndexByName(argc, columnName, "integerValue");
			if(argv[i] != NULL)
				*result = atoi(argv[i]);
			else
				*result = 0;
            return 0;
        }
	public:
	};
#if 0
id_: 程序分配 = 程序自增和处理，临时对象和永久对象的差异
     数据库序列 = Insert以后从数据库中取出，那时才有效
#endif
}

#endif //__DATA_DATAACCESS_H__