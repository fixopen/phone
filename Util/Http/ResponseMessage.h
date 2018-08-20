#ifndef __UTIL_HTTP_RESPONSEMESSAGE_H__
#define __UTIL_HTTP_RESPONSEMESSAGE_H__

#include "Message.h"

namespace Util {
    namespace Http {
        class ResponseMessage : public Message {
        public:
            enum State {
                //Informational 1xx
                sContinue = 100,
                sSwitchingProtocols = 101,
                //Successful 2xx
                sOK = 200,
                sCreated = 201,
                sAccepted = 202,
                sNonAuthoritativeInformation = 203,
                sNoContent = 204,
                sResetContent = 205,
                sPartialContent = 206,
                //Redirection 3xx
                sMultipleChoices = 300,
                sMovedPermanently = 301,
                sFound = 302,
                sSeeOther = 303,
                sNotModified = 304,
                sUseProxy = 305,
                //306 (Unused)
                sTemporaryRedirect = 307,
                //Client Error 4xx
                sBadRequest = 400,
                sUnauthorized = 401,
                sPaymentRequired = 402,
                sForbidden = 403,
                sNotFound = 404,
                sMethodNotAllowed = 405,
                sNotAcceptable = 406,
                sProxyAuthenticationRequired = 407,
                sRequestTimeout = 408,
                sConflict = 409,
                sGone = 410,
                sLengthRequired = 411,
                sPreconditionFailed = 412,
                sRequestEntityTooLarge = 413,
                sRequestURITooLong = 414,
                sUnsupportedMediaType = 415,
                sRequestedRangeNotSatisfiable = 416,
                sExpectationFailed = 417,
                //Server Error 5xx
                sInternalServerError = 500,
                sNotImplemented = 501,
                sBadGateway = 502,
                sServiceUnavailable = 503,
                sGatewayTimeout = 504,
                sHTTPVersionNotSupported = 505,
            };
            ResponseMessage(void);
            ~ResponseMessage(void);
            std::string const ToString() const;
            static ResponseMessage* const Parse(std::string const& value);
            std::string version;
            State state;
            std::string description;
            //HTTP/1.1 426 Upgrade Required
        };
    }
}

#endif //__UTIL_HTTP_RESPONSEMESSAGE_H__
