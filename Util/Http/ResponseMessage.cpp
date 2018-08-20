#include "ResponseMessage.h"

namespace Util {
    namespace Http {
        ResponseMessage::ResponseMessage(void)
        {
        }

        ResponseMessage::~ResponseMessage(void)
        {
        }

        std::string const ResponseMessage::ToString() const {
            std::string result;
            return result;
        }

        ResponseMessage* const ResponseMessage::Parse(std::string const& value) {
            ResponseMessage* result = 0;
            return result;
        }
    }
}
