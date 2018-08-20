#ifndef MMSWarp_h
#define MMSWarp_h

#include "../Data/MMSData.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace MMS
{
	class MMSWarp 
	{
	public:
		MMSWarp();
		~MMSWarp();
		MMSWarp(const MMSWarp& mms);
		static MMSWarp* GetMMSWarp();

		typedef struct
		{
			unsigned int width;
			unsigned int height;
		} MMS_SIZE;

		typedef enum
		{
			rtImage,
			rtText,
			rtVideo,
		}MMS_REGION_TYPE;

		typedef enum
		{
			rfHidden,
			rfFill,
			rfMeet,
			rfScroll,
			rfSlice,
		}MMS_REGION_FIT;

		typedef struct
		{
			string id;
			unsigned int left;
			unsigned int top;
			unsigned int width;
			unsigned int height;
			string fit;
		} MMS_REGION;

		typedef struct
		{
			MMS_SIZE root_layout;
			map<string, MMS_REGION> regions;
		}MMS_LAYOUT;

		typedef enum
		{
			stImge,
			stText,
			stAudio,
			stVideo
		}MMS_SRCTYPE;

		typedef struct
		{
			MMS_SRCTYPE type;
			string name;
			string region;
			unsigned int begin;
			unsigned int end;
			unsigned int dur;
			string alt;
		} MMS_SRC;

		typedef struct
		{
			unsigned int duration;
			vector<MMS_SRC> srcs;
		} MMS_PAR;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-send-req = <Octet 128>
			string TransactionID;
			unsigned char MMSVersion;//MMS-version-value = Short-integer
			string RecipientAddress;//To-value = Encoded-string-value
			string ContentType;
			string SenderAddress;//Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
			//	char MessageClass;//Message-class-value = Class-identifier | Token-text	Class-identifier = Personal | Advertisement | Informational | Auto		Personal = <Octet 128>		Advertisement = <Octet 129>		Informational = <Octet 130>		Auto = <Octet 131>
			//	unsigned int DateAndTime;//Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
			unsigned int TimeOfExpiry;//Expiry-value = Value-length (Absolute-token Date-value | Relative-token Delta-seconds-value)Absolute-token = <Octet 128>	Relative-token = <Octet 129>
			//	char DeliveryReport;//Delivery-report-value = Yes | No	Yes = <Octet 128>		No = <Octet 129>
			//	char MMState;//Status-value = Expired | Retrieved | Rejected | Deferred | Unrecognised	Expired = <Octet 128>		Retrieved = <Octet 129>		Rejected = <Octet 130>		Deferred = <Octet 131>		Unrecognised = <Octet 132>		The value Unrecognized is reserved for version management purpose only.
			string Subject;//Encoded-string-value

			//body
			MMS_LAYOUT layout;
			vector<MMS_PAR> pars;
		} MMS_SubmitRequest;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-send-conf = <Octet 129>
			string TransactionID;
			unsigned char MMSVersion;
			unsigned char RequestStatus;//Response-status-value = Ok |
			//Error-unspecified |
			//Error-service-denied |
			//Error-message-format-corrupt |
			//Error-sending-address-unresolved |
			//Error-message-not-found |
			//Error-network-problem |
			//Error-content-not-accepted |
			//Error-unsupported-message
			//Ok = <Octet 128>
			//Error-unspecified = <Octet 129>
			//Error- service-denied = <Octet 130>
			//Error-message-format-corrupt = <Octet 131>
			//Error-sending-address-unresolved = <Octet 132>
			//Error-message-not-found = <Octet 133>
			//Error-network-problem = <Octet 134>
			//Error- content-not-accepted = <Octet 135>
			//Error-unsupported-message = <Octet 136>
			//Any other values SHALL NOT be used. They are reserved for future use. The value Error-unsupported-message is
			//reseved for version management purpose only.
			string RequestStatusText;//Encoded-string-value
			string MessageID;//Optional Message-ID-value = Text -string	Encoded as in email address as per [RFC822]. The characters "<" and ">" are not included.
		} MMS_SubmitResponse;
		
		typedef struct
		{
			//head
			unsigned char MessageType;//m-notification-ind = <Octet 130>
			string TransactionID;
			unsigned char MMSVersion;
			unsigned char MessageClass;
			unsigned int MessageSize;//Message-size-value = Long-integer
			unsigned int TimeOfExpiry;
			string MessageReference;//uri
			//	string Subject;//Optional
			string SenderAddress;
			//	char DeliveryReport;
		} MMS_NotificationRequest;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-notifyresp-ind = <Octet 131>
			string TransactionID;
			unsigned char MMSVersion;
			unsigned char MMStatus;//Status-value = Expired | Retrieved | Rejected | Deferred | Unrecognised | Indeterminate | Forwarded | Unreachable
									//Expired = <Octet 128>
									//Retrieved = <Octet 129>
									//Rejected = <Octet 130>
									//Deferred = <Octet 131>
									//Unrecognised = <Octet 132>
									//Indeterminate = <Octet 133>
									//Forwarded = <Octet 134>
									//Unreachable = <Octet 135>
			unsigned char ReportAllowed;//Report-allowed-value = Yes | No	Yes = <Octet 128>		No = <Octet 129>
		} MMS_NotificationResponse;

		typedef struct
		{
			string MessageReference;//uri
		} MMS_RetrieveRequest;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-retrieve-conf = <Octet 132>
			string TransactionID;//Optional
			unsigned char MMSVersion;
			string MessageID;//Message-ID-value = Text -string
			string SenderAddress;//Optional
			unsigned char ContentType;
			string MultipartRelatedType;
			string ContentID;
			string RecipientAddress;//Optional
			unsigned char MessageClass;
			unsigned int DateAndTime;
			string Subject;
			unsigned char Priority;
			unsigned char DeliveryReport;

			unsigned char RequestStatus;
			//unsigned char MMState;

			MMS_LAYOUT layout;
			vector<MMS_PAR> pars;
			vector<string> content;
		} MMS_RetrieveResponse;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-acknowledge-ind = <Octet 133>
			string TransactionID;
			unsigned char MMSVersion;
			unsigned char ReportAllowed;//Optional
		} MMS_AcknowledgementRequest;

		typedef struct
		{
			//head
			unsigned char MessageType;//m-delivery-ind = <Octet 134>
			unsigned char MMSVersion;
			string MessageID;
			string RecipientAddress;
			unsigned int DateAndTime;
			unsigned char MMStatus;
		} MMS_DeliveryReportingRequest;

		int EncodeSubmitRequest(const MMS_SubmitRequest* pSrc, char* pDst);
		int DecodeSubmitResponse(const unsigned char* pSrc, int srcLen, MMS_SubmitResponse* pDst);
		int DecodeNotificationRequest(const unsigned char* pSrc, int srcLen, MMS_NotificationRequest* pDst);
		int EncodeNotificationResponse(const MMS_NotificationResponse* pSrc, char* pDst);
		int DecodeRetrieveResponse(const unsigned char* pSrc, int srcLen, MMS_RetrieveResponse* pDst);
		int EncodeAcknowledgementRequest(const MMS_AcknowledgementRequest* pSrc, char* pDst);
		int DecodeDeliveryReportingRequest(const unsigned char* pSrc, int srcLen, MMS_DeliveryReportingRequest* pDst);

		Data::MMSData* DecodeMessage(const unsigned char* pSrc, int srcLen);
		unsigned int DecodeSubmitResponse(const unsigned char* pSrc, int srcLen);
		unsigned int EncodeNotificationResponse(std::string transactionId, unsigned char status, char* pDst);

		void DecodeSmil(std::string file, vector<MMS_PAR>& vpars, MMS_LAYOUT& layout);
		void EncodeSmil(vector<MMS_PAR> vpars, MMS_LAYOUT layout, std::string path);
		void AddRootLayout(MMS_SubmitRequest* req, unsigned int width,	unsigned int height);
		void AddRegion(MMS_SubmitRequest* req, MMS_REGION_TYPE id, unsigned int left, unsigned int top, unsigned int width, unsigned int height, MMS_REGION_FIT fit = rfHidden);
		void AddPar(MMS_SubmitRequest* req, string imageFile, string textFile, string audioFile, unsigned int dur = 10);
		int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
		std::wstring ToUnicode(std::string content);
		std::string FromUnicode(std::wstring content);
		string Convert(string str, int sourceCodepage, int targetCodepage);
	};
}


#endif // MMSWarp_h
