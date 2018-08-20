#pragma once

#include "Util/Thread.h"
#include "Util/SerialPort.h"
#include "WishDialog.h"

class Account {
public:
    enum Status {
        sNull,
        sOffline,
        sOnline,
        sPauseService,
        sError,
    };
	Status const GetAccountStatus() const;
    void Charge(std::string const& cardNo, std::string const& password);
    double const GetBalance() const;
};

class ShortMessageModem {
public:
    class ReceiverThread : public Util::Thread {
    public:
        ReceiverThread(WishDialog* const owner)
        : quit_(false)
        , owner_(owner) {
        }
        int const run();
        void willStop() {
            quit_ = true;
        }
    private:
        volatile bool quit_;
        WishDialog* owner_;
    };
	static ShortMessageModem* GetInstance();
	void InitSerialPort();
	void InitModem();
	void ConnectServer();

	enum Result {
		rNull,
		rError,
	};
	Result const SendMessage(std::string const& mobileNumber, std::string const& content);
	typedef void (*MessageProcessFunction)(std::string const& mobileNumber, std::string const& content);
	void StartMessageReceiverThread(WishDialog* const owner, MessageProcessFunction messageProcessFunction);

	void DisconnectServer();
	void UninitModem();
	void UninitSerialPort();

    Account* account;
private:
    ShortMessageModem(){}
    ShortMessageModem(ShortMessageModem const&);
    ReceiverThread* receiverThread_;
};
