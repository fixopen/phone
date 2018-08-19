// MessageSet.h: interface for the CMessageSet class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DATA_MESSAGESET_H__
#define __DATA_MESSAGESET_H__

#include "Data.h"
#include "OTAData.h"

namespace Data
{
	enum SetMessageType
	{
		setDefault,
		setCurrent,
	};
	class MessageSet : public DataAccess<MessageSet>
	{
	public:
		SetMessageType const Type() const
		{
			return type_;
		}
		void Type(SetMessageType const type)
		{
			type_ = type;
		}
		std::string const SmsCenterAddress() const
		{
			return smsCenterAddress_;
		}
		void SmsCenterAddress(std::string const smsCenterAddress)
		{
			smsCenterAddress_ = smsCenterAddress;
		}
		unsigned int const SmsValidity() const
		{
			return smsValidity_;
		}
		void SmsValidity(unsigned int const smsValidity)
		{
			smsValidity_ = smsValidity;
		}
		bool const SmsReceipt() const
        {
            return smsReceipt_;
        }
        void SmsReceipt(bool const smsReceipt)
        {
            smsReceipt_ = smsReceipt;
        }
		bool const SaveSendMessage() const
		{
			return saveSendMessage_;
		}
		void SaveSendMessage(bool const saveSendMessage)
		{
			saveSendMessage_ = saveSendMessage;
		}
		bool const SaveMessageDraft() const
		{
			return saveMessageDraft_;
		}
		void SaveMessageDraft(bool const saveMessageDraft)
		{
			saveMessageDraft_ = saveMessageDraft;
		}
		//MMS

		unsigned int const MmsValidity() const
		{
			return mmsValidity_;
		}
		void MmsValidity(unsigned int const mmsValidity)
		{
			mmsValidity_ = mmsValidity;
		}
		bool const IsAutoReceive() const
        {
            return isAutoReceive_;
        }
        void IsAutoReceive(bool const isAutoReceive)
        {
            isAutoReceive_ = isAutoReceive;
        }
		bool const MmsSendReport() const
		{
			return mmsSendReport_;
		}
		void MmsSendReport(bool const mmsSendReport)
		{
			mmsSendReport_ = mmsSendReport;
		}
		bool const MmsReadReport() const
		{
			return mmsReadReport_;
		}
		void MmsReadReport(bool const mmsReadReport)
		{
			mmsReadReport_ = mmsReadReport;
		}
		bool const RecoveryDefSet() const
		{
			return recoveryDefSet_;
		}
		void RecoveryDefSet(bool const recoveryDefSet)
		{
			recoveryDefSet_ = recoveryDefSet;
		}
		unsigned int const MmsNetSelect() const
		{
			return mmsNetSelect_;
		}
		void MmsNetSelect(unsigned int const mmsNetSelect)
		{
			mmsNetSelect_ = mmsNetSelect;
		}
		unsigned int const MmsMaxSize() const
		{
			return mmsMaxSize_;
		}
		void MmsMaxSize(unsigned int const mmsMaxSize)
		{
			mmsMaxSize_ = mmsMaxSize;
		}
		unsigned int const AnonymousMMSReceive() const
		{
			return anonymousMMSReceive_;
		}
		void AnonymousMMSReceive(unsigned int const anonymousMMSReceive)
		{
			anonymousMMSReceive_ = anonymousMMSReceive;
		}

		bool const SmsSendSaveSim() const //add by qi 20100407
		{
			return smsSendSaveSim_;
		}
		void SmsSendSaveSim(bool const bsmsSim)
		{
			smsSendSaveSim_ = bsmsSim;
		}
		bool const SmsReciveSaveSim() const
		{
			return smsReciveSaveSim_;
		}
		void SmsReciveSaveSim(bool const bsmsSim)
		{
			smsReciveSaveSim_ = bsmsSim;
		}

	public:
		MessageSet();
		virtual ~MessageSet();
	public:
		static std::vector<boost::shared_ptr<MessageSet> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0);
		static boost::shared_ptr<MessageSet> GetCurrentConfig();
		static boost::shared_ptr<MessageSet> GetDefaultConfig();
		void Update() const; //sync to database
        bool Insert(); //insert new instance to database
        static void Remove(std::string const& filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MessageSet> item);
	public:
		SetMessageType type_;
		//sms
		std::string smsCenterAddress_;  //�������ĺ���
		unsigned int smsValidity_;  //������Ч��
		bool smsReceipt_; //���Ż�ִ
		bool saveSendMessage_;  //�Ƿ񱣴��ѷ�����Ϣ
		bool saveMessageDraft_; //�Ƿ���Ϣ�洢���ݸ���
		//mms
		unsigned int mmsValidity_;
		unsigned int mmsNetSelect_; //����Ϣ����ѡ��
		bool isAutoReceive_; //�Ƿ��Զ����ղ���
		bool mmsSendReport_;  //�Ƿ����÷��ͱ���
		bool mmsReadReport_;  //�Ƿ������Ķ�����
		bool recoveryDefSet_; //�Ƿ�ָ���������
		bool smsSendSaveSim_;//������Ϣ�Ƿ񱣴��ڵ�SIM
		bool smsReciveSaveSim_;//���ܵ���Ϣ�Ƿ񱣴浽SIM
		unsigned int mmsMaxSize_; //����Ϣ�������
		unsigned int anonymousMMSReceive_;  //�������ŵĽ��ܷ�ʽ 0-�ܽ�, 1-����, 2-ÿ��ѯ��
	};
}
#endif // __DATA_MESSAGESET_H__
