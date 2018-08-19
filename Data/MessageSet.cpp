// MessageSet.cpp: implementation of the MessageSet class.

#include "stdafx.h"
#include "multimediaphone.h"
#include "MessageSet.h"

namespace Data {
	std::string MessageSet::tableName_ = "messageSet";
	int MessageSet::rowCount_ = 0;
	int MessageSet::offset_ = 0;
	Indication MessageSet::indication_ = 0;
	sqlite3* MessageSet::db_ = Data::GetDB();
}

namespace Data 
{
	MessageSet::MessageSet()
	{	
	}

	MessageSet::~MessageSet()
	{
	}

	void MessageSet::Update() const
	{
		std::string cmd = "UPDATE ";
		cmd += tableName();
		cmd += " SET [type] = ";
		cmd += Util::StringOp::FromInt(type_);
		cmd += ", [smsCenterAddress] = '";
		cmd += smsCenterAddress_;
		cmd += "', [smsValidity] = ";
		cmd += Util::StringOp::FromInt(smsValidity_);
		cmd += ", [smsReceipt] = ";
		cmd += Util::StringOp::FromInt(smsReceipt_);
		cmd += ", [saveSendMessage] = ";
		cmd += Util::StringOp::FromInt(saveSendMessage_);
		cmd += ", [saveMessageDraft] = ";
		cmd += Util::StringOp::FromInt(saveMessageDraft_);
		cmd += ", [mmsNetSelect] = ";
		cmd += Util::StringOp::FromInt(mmsNetSelect_);
		cmd += ", [isAutoReceive] = ";
		cmd += Util::StringOp::FromInt(isAutoReceive_);
		cmd += ", [mmsSendReport] = ";
		cmd += Util::StringOp::FromInt(mmsSendReport_);
		cmd += ", [mmsReadReport] = ";
		cmd += Util::StringOp::FromInt(mmsReadReport_);
		cmd += ", [recoveryDefSet] = ";
		cmd += Util::StringOp::FromInt(recoveryDefSet_);
		cmd += ", [mmsMaxSize] = ";
		cmd += Util::StringOp::FromInt(mmsMaxSize_);
		cmd += ", [anonymousMMSReceive] = ";
		cmd += Util::StringOp::FromInt(anonymousMMSReceive_);
		cmd += ", [smsSendSave] = ";
		cmd += Util::StringOp::FromInt(smsSendSaveSim_);
		cmd += ", [smsReciveSave] = ";
		cmd += Util::StringOp::FromInt(smsReciveSaveSim_);
		cmd += ", [mmsValidity] = ";
		cmd += Util::StringOp::FromInt(mmsValidity_);
		cmd += " WHERE [id] = ";
		cmd += Util::StringOp::FromInt(id());
		
		ExecCommand(cmd);
	}

	bool MessageSet::Insert()
	{
		std::string cmd = "INSERT INTO ";
		cmd += tableName();
		cmd += " (type, smsCenterAddress, smsValidity, smsReceipt, saveSendMessage, saveMessageDraft, mmsNetSelect, isAutoReceive, mmsSendReport, mmsReadReport, recoveryDefSet, mmsMaxSize, anonymousMMSReceive,smsSendSave,smsReciveSave,mmsValidity) VALUES ( ";
		cmd += Util::StringOp::FromInt(type_);
		cmd += ", '";
		cmd += smsCenterAddress_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(smsValidity_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(smsReceipt_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(saveSendMessage_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(saveMessageDraft_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsNetSelect_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isAutoReceive_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsSendReport_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsReadReport_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(recoveryDefSet_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsMaxSize_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(anonymousMMSReceive_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(smsSendSaveSim_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(smsReciveSaveSim_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsValidity_);
		cmd +=")";
		
		bool bt = ExecCommand(cmd);
		id(GetCurrentId());
		
		return bt ;
	}

	void MessageSet::Remove(const std::string &filter)
	{
		RemoveDatasByFilter(filter);
	}

	void MessageSet::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MessageSet> item)
	{
		item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
		item->type_               = static_cast<SetMessageType>(atoi(argv[getIndexByName(argc,columnName,"type")]));
		item->smsCenterAddress_   = argv[getIndexByName(argc,columnName,"smsCenterAddress")];
		item->smsValidity_		  = atoi(argv[Data::getIndexByName(argc, columnName, "smsValidity")]);
		item->smsReceipt_		  = !!atoi(argv[Data::getIndexByName(argc, columnName, "smsReceipt")]);
		item->saveSendMessage_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "saveSendMessage")]);
		item->saveMessageDraft_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "saveMessageDraft")]);
		item->mmsNetSelect_		  = atoi(argv[getIndexByName(argc,columnName,"mmsNetSelect")]);
		item->isAutoReceive_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAutoReceive")]);
		item->mmsSendReport_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "mmsSendReport")]);
		item->mmsReadReport_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "mmsReadReport")]);
		item->recoveryDefSet_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "recoveryDefSet")]);
		item->mmsMaxSize_		  = atoi(argv[getIndexByName(argc,columnName,"mmsMaxSize")]);
		item->anonymousMMSReceive_ = atoi(argv[getIndexByName(argc,columnName,"anonymousMMSReceive")]);
		item->smsSendSaveSim_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "smsSendSave")]);
		item->smsReciveSaveSim_	  = !!atoi(argv[Data::getIndexByName(argc, columnName, "smsReciveSave")]);
		item->mmsValidity_		  = atoi(argv[Data::getIndexByName(argc, columnName, "mmsValidity")]);
	}

	std::vector<boost::shared_ptr<MessageSet> > MessageSet::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
	{
		return DataAccess<MessageSet>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
	}

	boost::shared_ptr<MessageSet> MessageSet::GetCurrentConfig()	//type = 1
	{
		std::vector<boost::shared_ptr<MessageSet> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(setCurrent));
		return t[0];
	}

	boost::shared_ptr<MessageSet> MessageSet::GetDefaultConfig()		//type = 0
	{
		std::vector<boost::shared_ptr<MessageSet> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(setDefault));
		return t[0];
	}
}