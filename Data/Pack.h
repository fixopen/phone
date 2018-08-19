#ifndef __CONTACT_DATA_PACK_H__
#define __CONTACT_DATA_PACK_H__

#include "Data.h"

//namespace Contact {
    namespace Data {
        class Pack : public DataAccess<Pack> {
        public:
			int const serviceId() const {
				return serviceId_;
			}
			void serviceId(int const serviceId) {
				serviceId_ = serviceId;
			}
			std::string const type() const {
				return type_;
			}
			void type(std::string const type) {
				type_ = type;
			}
			std::string const no() const {
				return no_;
			}
			void no(std::string const no) {
				no_ = no;
			}
			std::string const rate() const {
				return rate_;
			}
			void rate(std::string const rate) {
				rate_ = rate;
			}
			bool const isOrder() const {
				return isOrder_;
			}
			void isOrder(bool const isOrder) {
				isOrder_ = isOrder;
			}
            Pack(void); //create
            //~Pack(); //destroy
            static std::vector<boost::shared_ptr<Pack> > GetFromDatabase(std::string const& filter/*, std::wstring const& orderFieldName = L"id", SeqenceRelation const sr = srDesc, int const offset = 0, int const pageSize = -1*/);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::string const& filter); //delete from database
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Pack> item);
        private:
            int serviceId_;
            std::string type_;
            std::string no_;
            std::string rate_;
            bool isOrder_;
        };
    }
//}

#endif //__CONTACT_DATA_PACK_H__
