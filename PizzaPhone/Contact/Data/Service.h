#ifndef __CONTACT_DATA_SERVICE_H__
#define __CONTACT_DATA_SERVICE_H__

#include "../../Util/Data.h"

namespace Contact {
    namespace Data {
        class Service : public Util::DataAccess<Service> {
        public:
            Service(); //create
            //~Service(); //destroy
            static std::vector<Util::shared_ptr<Service> > Select(std::wstring const & filter, std::wstring const & orderFieldName = L"id", SeqenceRelation const sr = srDesc, int const offset = 0, int const pageSize = -1);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const & filter); //delete from database
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Service>& item);
        private:
            std::wstring name_;
            std::wstring code_;
            std::wstring welcomeInfo_;
        };
    }
}

#endif //__CONTACT_DATA_SERVICE_H__
