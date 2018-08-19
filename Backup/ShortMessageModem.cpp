#include "ShortMessageModem.h"

//#include "Message.h"
//#include "Customer.h"
//
//ShortMessageModem* ShortMessageModem::GetInstance() {
//    static ShortMessageModem* result = new ShortMessageModem();
//    return result;
//}
//
//void ShortMessageModem::InitSerialPort() {
//}
//
//void ShortMessageModem::InitModem() {
//}
//
//void ShortMessageModem::ConnectServer() {
//}
//
//ShortMessageModem::Result const ShortMessageModem::SendMessage(std::string const& mobileNumber, std::string const& content) {
//    ShortMessageModem::Result result;
//    return result;
//}
//
//void ShortMessageModem::StartMessageReceiverThread(WishDialog* const owner, MessageProcessFunction messageProcessFunction) {
//    receiverThread_ = new ReceiverThread(owner);
//    receiverThread_->start();
//}
//
//void ShortMessageModem::DisconnectServer() {
//}
//
//void ShortMessageModem::UninitModem() {
//}
//
//void ShortMessageModem::UninitSerialPort() {
//}
//
//int const ShortMessageModem::ReceiverThread::run() {
//    while (!quit_) {
//        //read message from short message modem
//        //construct message
//        Message* messagePtr = new Message();
//        CString content;
//        messagePtr->content = content;
//        if (messagePtr->IsFilter()) {
//            messagePtr->timestamp = Util::Timestamp::GetCurrentTime();
//            int customerId = 0;
//            std::wstring customerNumber;
//            std::vector<Util::shared_ptr<Customer> > customers = Customer::Select(L"mobileNumber = '" + customerNumber + L"'");
//            if (customers.size() == 0) { //not in customer table
//                Customer customer;
//                customer.mobileNumber = customerNumber.c_str();
//                customer.Insert();
//                customerId = customer.id();
//            } else if (customers.size() == 1) { //find int table
//                customerId = customers[0]->id();
//            } else { //error find
//                //do nothing
//            }
//            messagePtr->customerId = customerId;
//            //messagePtr->type = type;
//            messagePtr->CalcType();
//            //send to wish dialog
//            //::PostMessage(owner_, UM_SHORT_MESSAGE, messagePtr, 0);
//            owner_->AppendNewMessage(Util::shared_ptr<Message>(messagePtr));
//        }
//        //sleep
//        ::Sleep(1000);
//    }
//    return 0;
//}