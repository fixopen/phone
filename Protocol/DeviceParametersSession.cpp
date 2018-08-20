#include "DeviceParametersSession.h"

    DeviceParametersSession::DeviceParametersSession(void)
    {
    }

    DeviceParametersSession::~DeviceParametersSession(void)
    {
    }

    void DeviceParametersSession::selfProcess_(ResponseMessage const& response) const {
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
    }

    std::string const DeviceParametersSession::resourceName_() const {
        return "ZF3G/tic/parameter";
    }
