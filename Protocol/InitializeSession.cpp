#include "../Util/StringOp.h"
#include "InitializeSession.h"
#include "Communicator.h"
//#include "../Util/XMLParser.h"
#include "../Data/Service.h"
#include "../Data/Pack.h"

InitializeSession::InitializeSession(void) {
}

InitializeSession::~InitializeSession(void)
{
}

//void InitializeSession::Process() {
//    Communicator* c = new Communicator();
//    RequestMessage request;
//    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_;
//    ResponseMessage response = c->Get(request);
//    if (response.state == ResponseMessage::sUnauthorized) {
//        std::vector<std::string> bigParts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], " ");
//        std::vector<std::string> parts = Util::StringOp::Split(bigParts[1], ",");
//        std::map<std::string, std::string> values;
//        for (size_t i = 0; i < parts.size(); ++i) {
//            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
//            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
//        }
//        std::string username = userAgent_ + "&" + os_ + "&" + version_;
//        char buffer[9] = {0};
//        ++nc_;
//        sprintf(buffer, "%08d", nc_);
//        std::string nc = buffer;
//
//        std::string cnonceValue = "1234567890";
//
//        //std::string digestUriValue = "";
//        //std::string a = base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"));
//        //std::string b = base16_(md5_("GET:" + request.uri));
//        std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"))
//            + ":"
//            + unquote_(values["nonce"])
//            + ":"
//            + nc
//            + ":"
//            + cnonceValue
//            + ":"
//            + unquote_(values["qop"])
//            + ":"
//            + base16_(md5_("GET:" + request.uri))));
//        
//        std::string authInfo = "Digest username=\"" + username
//            + "\",realm=" + values["realm"]
//            + ",nonce=" + values["nonce"]
//            + ",uri=\"" + request.uri
//            + "\",qop=" + values["qop"]
//            + ",nc=" + nc
//            + ",cnonce=\"" + cnonceValue
//            + "\",response=\"" + responseValue
//            + "\",opaque=" + values["opaque"];
//        request.HeaderFields.insert(std::make_pair("Authorization", authInfo));
//        ResponseMessage res = c->Get(request);
//        //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
//        parts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], ",");
//        values.clear();
//        for (size_t i = 0; i < parts.size(); ++i) {
//            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
//            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
//        }
//        //end of register
//    } else if (response.state == ResponseMessage::sRequestTimeout) {
//        //
//    } else {
//        std::string b = response.Body;
//        //parseBody(b);
//    }
//}
void InitializeSession::selfProcess_(ResponseMessage const& response) const {
    //Util::DocSource* source = new Util::MemDocSource(Util::StringOp::FromUTF8(response.Body));
    //Util::XMLParser parser;
    //Util::shared_ptr<Xml::Document> doc = parser.Parse(source);
    //delete source;
    /*
<?xml version="1.0" encoding="UTF-8"?>
<initialresponse>
	<msisdn>13912345678</msisdn>
	<username>13912345678</username>
	<password>11111111</password>
	<services>
		<service>
			<servicename>������ͥҵ��</servicename>
			<servicecode>001</servicecode>
			<pack>
				<feetype>�����Ʒ��ײ�</feetype>
				<serviceid>001001</serviceid>
				<servicerate>0</servicerate>
				<order>0</order>
			</pack>
			<pack>
				<feetype>5Ԫ�����ײ�</feetype>
				<serviceid>001002</serviceid>
				<servicerate>20</servicerate>
				<order>1</order>
			</pack>
			<welcomeinfo>��ӭʹ�û�����ͥҵ��</welcomeinfo>
		</service>
	</services>
</initialresponse>
    */
	size_t offset = 0;
	std::string serviceStr = findContent_(response.Body, "service", offset);
	while (serviceStr != "") {
		size_t off = 0;
		std::string name = findContent_(serviceStr, "servicename", off);
		off = 0;
		std::string code = findContent_(serviceStr, "servicecode", off);
		off = 0;
		std::string welcomeInfo = findContent_(serviceStr, "welcomeinfo", off);
		Data::Service service;
		service.name(name);
		service.code(code);
		service.welcomeInfo(welcomeInfo);
		service.Insert();
		off = 0;
		std::string packStr = findContent_(serviceStr, "pack", off);
		while (packStr != "") {
			size_t o = 0;
			std::string type = findContent_(packStr, "feetype", o);
			o = 0;
			std::string no = findContent_(packStr, "serviceid", o);
			o = 0;
			std::string rate = findContent_(packStr, "servicerate", o);
			o = 0;
			std::string orderStr = findContent_(packStr, "order", o);
			bool isOrder = !!Util::StringOp::ToInt(orderStr, 10);
			Data::Pack pack;
			pack.serviceId(service.id());
			pack.type(type);
			pack.no(no);
			pack.rate(rate);
			pack.isOrder(isOrder);
			pack.Insert();
			packStr = findContent_(serviceStr, "pack", off);
		}
		serviceStr = findContent_(response.Body, "service", offset);
	}
}

std::string const InitializeSession::resourceName_() const {
    return "ZF3G/tic/initialize";
}
