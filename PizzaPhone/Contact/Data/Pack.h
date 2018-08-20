#ifndef __CONTACT_DATA_PACK_H__
#define __CONTACT_DATA_PACK_H__

#include "../../Util/Data.h"

namespace Contact {
    namespace Data {
        class Pack : public Util::DataAccess<Pack> {
        public:
            Pack(void); //create
            //~Pack(); //destroy
            static std::vector<Util::shared_ptr<Pack> > Select(std::wstring const & filter, std::wstring const & orderFieldName = L"id", SeqenceRelation const sr = srDesc, int const offset = 0, int const pageSize = -1);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const & filter); //delete from database
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Pack>& item);
        private:
            int serviceId_;
            std::wstring type_;
            std::wstring no_;
            std::wstring rate_;
            bool isOrder_;
        };
    }
}

#endif //__CONTACT_DATA_PACK_H__
