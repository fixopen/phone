#include "../Util/StringOp.h"
#include "Session.h"
#include "Communicator.h"
#include "../Util/SHA1/sha1.h"
#include "../Util/RSA/rsaeuro/source/rsa.h"
#include "../Util/BigInteger/BigUnsignedInABase.h"
#include "../Util/BigInteger/BigIntegerAlgorithms.h"
#include "../Util/BigInteger/BigIntegerUtils.h"
//#include "../Util/MD5/md5.h"
//#define MD_CTX MD5_CTX
//#define MDInit MD5Init
//#define MDUpdate MD5Update
//#define MDFinal MD5Final

std::string Session::msisdn_ = "13408003013";
int Session::nc_ = 0;

Session::Session(std::string const& server, std::string const& userAgent, std::string const& os, std::string const& version)
: server_(server)
, userAgent_(userAgent)
, os_(os)
, version_(version) {
}

Session::~Session() {
}

void Session::Process(std::string const& body) {
    /*Communicator* */communicator_ = new Communicator();
    //c->ConsultKey("deviceId", "platformPublicKey");
    RequestMessage request;
    //request.type = RequestMessage::tGet;
    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_ + additionParameter_();
    request.HeaderFields.insert(std::make_pair(std::string("Content-Length"), Util::StringOp::ToUTF8(Util::StringOp::FromInt(body.length()))));
    request.HeaderFields.insert(std::make_pair(std::string("x-up-calling-line-id"), msisdn_d_()));
	//x-up-calling-line-id
    request.Body = body;
    //request.version = "HTTP/1.1";
    //request.HeaderFields.insert(std::make_pair("x-device-id", deviceId_()));
    ResponseMessage response = communicator_->Get(request);
    if (response.state == ResponseMessage::sUnauthorized) {
        //WWW-Authenticate: Digest realm="hljt@service.cmcc.cn",qop="auth",nonce="c8a46fb0dec24b7285e1b9516a7de345",opaque="17c91d3e2e9e4a74b6af2cc4c732f2d0"
        std::vector<std::string> bigParts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], " ");
        //bigParts[0]; //method == Digest
        std::vector<std::string> parts = Util::StringOp::Split(bigParts[1], ",");
        std::map<std::string, std::string> values;
        for (size_t i = 0; i < parts.size(); ++i) {
            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
        }
        response = auth_(values, request.uri);
    } else if (response.state == ResponseMessage::sRequestTimeout) {
        response = timeout_();
    } else {
        selfProcess_(response);
    }
    delete communicator_;
}

ResponseMessage const Session::auth_(std::map<std::string, std::string>& values, std::string const& digestUriValue) const {
    ResponseMessage result;
    std::string username = userAgent_ + "&" + os_ + "&" + version_;

    char buffer[9] = {0};
    ++nc_;
    sprintf(buffer, "%08d", nc_);
    std::string nc = buffer;

    std::string cnonceValue = "1234567890";

    std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + "abcd"))
        + ":"
        + unquote_(values["nonce"])
        + ":"
        + nc
        + ":"
        + cnonceValue
        + ":"
        + unquote_(values["qop"])
        + ":"
        + base16_(md5_("GET:" + digestUriValue))));
    
    std::string authInfo = "Digest username=\"" + username
        + "\",realm=" + values["realm"]
        + ",nonce=" + values["nonce"]
        + ",uri=\"" + digestUriValue
        + "\",qop=" + values["qop"]
        + ",nc=" + nc
        + ",cnonce=\"" + cnonceValue
        + "\",response=\"" + responseValue
        + "\",opaque=" + values["opaque"];
    RequestMessage request;
    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_;
    request.HeaderFields.insert(std::make_pair(std::string("Authorization"), authInfo));
    request.HeaderFields.insert(std::make_pair(std::string("x-up-calling-line-id"), msisdn_d_()));
    result = communicator_->Get(request);
    return result;

    //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
    std::vector<std::string> parts = Util::StringOp::Split(result.HeaderFields["WWW-Authenticate"], ",");
    values.clear();
    for (size_t i = 0; i < parts.size(); ++i) {
        std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
        values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    }
    //end of register
    /*
    std::string username = userAgent_ + "&" + os_ + "&" + version_;
    //std::string nc = "00000001";
    char buffer[9] = {0};
    ++nc_;
    sprintf(buffer, "%08d", nc_);
    std::string nc = buffer;

    std::string cnonceValue = "1234567890";

    //std::string digestUriValue = "";
    //std::string a = base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"));
    //std::string b = base16_(md5_("GET:" + request.uri));
    std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"))
        + ":"
        + unquote_(values["nonce"])
        + ":"
        + nc
        + ":"
        + cnonceValue
        + ":"
        + unquote_(values["qop"])
        + ":"
        + base16_(md5_("GET:" + request.uri))));
    
    std::string authInfo = "Digest username=\"" + username
        + "\",realm=" + values["realm"]
        + ",nonce=" + values["nonce"]
        + ",uri=\"" + request.uri
        + "\",qop=" + values["qop"]
        + ",nc=" + nc
        + ",cnonce=\"" + cnonceValue
        + "\",response=\"" + responseValue
        + "\",opaque=" + values["opaque"];
    request.HeaderFields.insert(std::make_pair("Authorization", authInfo));
    ResponseMessage res = c->Get(request);
    //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
    parts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], ",");
    values.clear();
    for (size_t i = 0; i < parts.size(); ++i) {
        std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
        values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    }
    //end of register
    */
    //request
    //WWW-Authenticate: {Digest realm="hljt@service.cmcc.cn",qop={auth|auth-int},nonce="nonceValue",opaque="opaqueValue"}
    //Authorization: {Digest username="ua&os&version",realm="hljt@service.cmcc.cn",nonce="nonceValue",uri="http://[server-url]/register?ua=yd6618&os=99&version=1",qop="{auth|auth-int}",nc=00000001,cnonce="cnonceValue",respone="responeValue",opaque="opaqueValue"}
    //Authentication-Info: {qop="{auth|auth-int}",nextnonce="nextnonceValue",rspauth="rspauthValue",nc=00000001,cnonce="cnonceValue"}
    //Communicator* c = 0;
    //c->Send();
    //http://[server-url]/register?ua=yd6618&os=99&version=1
    //platform-public-key
    //x-device-id: {vendor[3]type[3]assembly[3]serial[8]reserved[1]}
    //nonceValue is a unique string value
    //opaqueValue is a unique string value
    //ncValue is a unique number, increament by one at once
    //cnonceValue is a unique string value
    //responeValue is a value by:
    //  md5(concat(md5(concat(unquote(usernameValue), ":" , unquote(realmValue), ":", password)),
    //          ":", concat(unquote(nonceValue), ":", ncValue, ":", unquote(cnonceValue), ":", unquote(qopValue),
    //              ":", md5(concat(Method, ":", digestUriValue[, ":", md5(entityBody)])))))
    //  md5(concat(md5(password),
    //          ":", concat(unquote(nonceValue), ":", ncValue, ":", unquote(cnonceValue))))
    //nextnonceValue is a unique string value
    //rspauthValue is a value by:
    //  md5(concat(md5(concat(unquote(usernameValue), ":" , unquote(realmValue), ":", password)),
    //          ":", concat(unquote(nextnonceValue), ":", ncValue, ":", unquote(cnonceValue), ":", unquote(qopValue),
    //              ":", md5(concat(":", digestUriValue[, ":", md5(entityBody)])))))
    //  md5(concat(md5(password),
    //          ":", concat(unquote(nextnonceValue), ":", ncValue, ":", unquote(cnonceValue),
    //              ":", md5(concat(":", digestUriValue[, ":", md5(entityBody)])))))
    //password is md5(concat(deviceId, ":", randomKey))
    //unquote(x) remove the endian quote mark
    //concat(x,y,z,...) concat the parameters to one string
    //md5(x) standard md5 algorithm, return the base16 value
    //Method is GET
    //digestUriValue maybe is uriValue's md5
    //entityBody is none
}

ResponseMessage const Session::timeout_() const {
    ResponseMessage result;
    return result;
}

void Session::selfProcess_(ResponseMessage const& response) const {
}

std::string const Session::findContent_(std::string const& whole, std::string const& tagName, size_t& offset) const {
    size_t startPos = whole.find("<" + tagName + ">", offset);
	if (startPos == std::string::npos) {
		return "";
	}
	startPos += tagName.length() + 2;
    size_t stopPos = whole.find("</" + tagName + ">", startPos);
	if (stopPos == std::string::npos) {
		return "";
	}
	offset = stopPos + tagName.length() + 3;
    return whole.substr(startPos, stopPos - startPos);
}

std::string const Session::sha1_(std::string const& value) const {
    std::string result;
    SHA1Context sha;
    int err = SHA1Reset(&sha);
    if (err)
    {
        fprintf(stderr, "SHA1Reset Error %d.\n", err );
    }
    err = SHA1Input(&sha, (const unsigned char *)value.c_str(), value.length());
    if (err)
    {
        fprintf(stderr, "SHA1Input Error %d.\n", err );
    }
    uint8_t Message_Digest[20];
    err = SHA1Result(&sha, Message_Digest);
    if (err)
    {
        fprintf(stderr, "SHA1Result Error %d, could not compute message digest.\n", err );
    }
    else
    {
        //printf("\t");
        for(int i = 0; i < 20 ; ++i)
        {
            char data[3] = {0};
            //printf("%02X ", Message_Digest[i]);
            sprintf(data, "%02X", Message_Digest[i]);
            result += std::string(data, 2);
        }
        //printf("\n");
    }
    return result;
}

std::string const Session::md5_(std::string const& value) const {
    //std::string result;
    MD5_CTX context;
    MD5Init (&context);
    MD5Update (&context, reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str())), value.length());
    unsigned char digest[16];
    MD5Final (digest, &context);
    //return result;
    return std::string(reinterpret_cast<char*>(digest), 16);
}

std::string const Session::base16_(std::string const& value) const {
    std::string result;
    char buffer[3];
    for (size_t i = 0; i < value.length(); ++i) {
        memset(buffer, 0, 3);
        sprintf(buffer, "%02x", static_cast<unsigned char>(value[i]));
        result += buffer;
    }
    return result;
}

std::string const Session::unquote_(std::string const& value) const {
    //std::string result;
    //return result;
    return value.substr(1, value.length() - 2);
}

static R_RANDOM_STRUCT *InitRandomStruct(void)
{
	static unsigned char seedByte = 0;
	unsigned int bytesNeeded;
	static R_RANDOM_STRUCT randomStruct;

	R_RandomInit(&randomStruct);

	/* Initialize with all zero seed bytes, which will not yield an actual
			 random number output. */

	while (1) {
		R_GetRandomBytesNeeded(&bytesNeeded, &randomStruct);
		if(bytesNeeded == 0)
			break;

		R_RandomUpdate(&randomStruct, &seedByte, 1);
	}

	return(&randomStruct);
}

std::string const Session::rsaEncript_(R_RSA_PUBLIC_KEY const* const platformPublicKey, std::string const& deviceId, std::string const& randomKey) const {
    ////int RSAPublicEncrypt(unsigned char *output, unsigned int *outputLen, unsigned char *input, unsigned int inputLen, R_RSA_PUBLIC_KEY *publicKey, R_RANDOM_STRUCT *randomStruct);
    //unsigned char output[1024] = {0};
    //unsigned int outputLen = 0;
    //R_RANDOM_STRUCT* randomStruct = InitRandomStruct();
    //RSAPublicEncrypt(output, &outputLen, (unsigned char*)randomKey.c_str(), randomKey.length(), (R_RSA_PUBLIC_KEY*)platformPublicKey, randomStruct);
    //return std::string((char*)output, outputLen);
    //BigUnsigned modexp(const BigInteger &base, const BigUnsigned &exponent, const BigUnsigned &modulus);
    BigInteger base = stringToBigInteger("1633837924");
    //BigUnsigned b = stringToBigUnsigned("0x61626364");
    //std::string t = std::string(BigUnsignedInABase(b, 10));
    BigUnsigned r = modexp(base, stringToBigUnsigned("65537"), stringToBigUnsigned("92815292272177043786328740327788198709296623087333390559749596942558830396880084121909449325430198837493715038258635088494137469056503973206275416216281664722193690876816800515522675973798782363286066994241785394904839589987843092582789294388241741753908727742984320790061690510603733060968955533872923389687"));
    return std::string(BigUnsignedInABase(r, 16));
}

std::string const Session::msisdn_d_() const {
    return msisdn_;
}

std::string const Session::deviceId_() const {
    return "001001001933333444";
}

R_RSA_PUBLIC_KEY const* const Session::platformPublicKey_() const {
    //92815292272177043786328740327788198709296623087333390559749596942558830396880084121909449325430198837493715038258635088494137469056503973206275416216281664722193690876816800515522675973798782363286066994241785394904839589987843092582789294388241741753908727742984320790061690510603733060968955533872923389687
    //0x842c618d45297e18f0799c674d7915d146b2869ad3d2d22eca8279a243e7e7e8268e3b76a07dd37ec8b83cba1d9d8e4d4d4be3725dcdb8dd57b3c160de7c137b89238984ca87ef2ded51423bd03be513280903117bc554c93ad5ccdc1b80d580baf481d5346683b2d09d7ca937e1fb4242aadaae0f3b4485820251fbbf13aef7
    static R_RSA_PUBLIC_KEY publicKey = {
	    1024,
	    {
            0x84, 0x2c, 0x61, 0x8d, 0x45, 0x29, 0x7e, 0x18, 0xf0, 0x79, 0x9c, 0x67,
            0x4d, 0x79, 0x15, 0xd1, 0x46, 0xb2, 0x86, 0x9a, 0xd3, 0xd2, 0xd2, 0x2e,
            0xca, 0x82, 0x79, 0xa2, 0x43, 0xe7, 0xe7, 0xe8, 0x26, 0x8e, 0x3b, 0x76,
            0xa0, 0x7d, 0xd3, 0x7e, 0xc8, 0xb8, 0x3c, 0xba, 0x1d, 0x9d, 0x8e, 0x4d,
            0x4d, 0x4b, 0xe3, 0x72, 0x5d, 0xcd, 0xb8, 0xdd, 0x57, 0xb3, 0xc1, 0x60,
            0xde, 0x7c, 0x13, 0x7b, 0x89, 0x23, 0x89, 0x84, 0xca, 0x87, 0xef, 0x2d,
            0xed, 0x51, 0x42, 0x3b, 0xd0, 0x3b, 0xe5, 0x13, 0x28, 0x09, 0x03, 0x11,
            0x7b, 0xc5, 0x54, 0xc9, 0x3a, 0xd5, 0xcc, 0xdc, 0x1b, 0x80, 0xd5, 0x80,
            0xba, 0xf4, 0x81, 0xd5, 0x34, 0x66, 0x83, 0xb2, 0xd0, 0x9d, 0x7c, 0xa9,
            0x37, 0xe1, 0xfb, 0x42, 0x42, 0xaa, 0xda, 0xae, 0x0f, 0x3b, 0x44, 0x85,
            0x82, 0x02, 0x51, 0xfb, 0xbf, 0x13, 0xae, 0xf7
        },
	    {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	         0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
        }
    };
    return &publicKey;
}

std::string const Session::resourceName_() const {
    return "";
}

std::string const Session::additionParameter_() const {
    return "";
}