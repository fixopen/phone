#include "ContentSession.h"

namespace Protocol {
    ContentSession::ContentSession(void) {
    }

    ContentSession::~ContentSession(void) {
    }

    void ContentSession::SetType(Type const type) {
        type_ = type;
    }

    void ContentSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <contentdellistresponse>
        <count>2</count>
        <pack>
        <fileid>001</fileid>
        <filetype>1<filetype>
        </pack>
        </contentdellistresponse>        
        */
    }

    std::string const ContentSession::resourceName_() const {
        std::string result;
        switch (type_) {
        case tNormal:
            result = "ZF3G/tic/contentlist";
            break;
        case tDel:
            result = "ZF3G/tic/contentdellist";
            break;
        default:
            break;
        }
        return result;
    }
}
