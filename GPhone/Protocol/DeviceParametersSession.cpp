#include "DeviceParametersSession.h"

namespace Protocol {
    DeviceParametersSession::DeviceParametersSession(void) {
    }

    DeviceParametersSession::~DeviceParametersSession(void) {
    }

    void DeviceParametersSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <parameterresponse>
        <pack>
        <paraname>syncfrequency</paraname>
        <paravalue>5</paravalue>
        <paraname>xxx</paraname>
        <paravalue>5</paravalue>
        </pack>
        <parameter>
        */
        std::map<std::string, std::string> parameters;
        size_t offset = 0;
        std::string name = findContent_(response.Body, "paraname", offset);
        std::string value = findContent_(response.Body, "paravalue", offset);
        while ((name != "") && (value != "")) {
            parameters.insert(std::make_pair(name, value));
            name = findContent_(response.Body, "paraname", offset);
            value = findContent_(response.Body, "paravalue", offset);
        }
    }

    std::string const DeviceParametersSession::resourceName_() const {
        return "ZF3G/tic/parameter";
    }
}
