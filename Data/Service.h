#ifndef __CONTACT_DATA_SERVICE_H__
#define __CONTACT_DATA_SERVICE_H__

#include "Data.h"

//namespace Contact {
    namespace Data {
        class Service : public DataAccess<Service> {
        public:
			std::string const name() const {
				return name_;
			}
			void name(std::string const name) {
				name_ = name;
			}
			std::string const code() const {
				return code_;
			}
			void code(std::string const code) {
				code_ = code;
			}
			std::string const welcomeInfo() const {
				return welcomeInfo_;
			}
			void welcomeInfo(std::string const welcomeInfo) {
				welcomeInfo_ = welcomeInfo;
			}
            Service(); //create
            //~Service(); //destroy
            static std::vector<boost::shared_ptr<Service> > Select(std::string const& filter/*, std::wstring const& orderFieldName = L"id", SeqenceRelation const sr = srDesc, int const offset = 0, int const pageSize = -1*/);
			static std::vector<boost::shared_ptr<Service> > GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::string const& filter); //delete from database
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Service> item);
        private:
            std::string name_;
            std::string code_;
            std::string welcomeInfo_;
        };
    }
//}

#endif //__CONTACT_DATA_SERVICE_H__
