#ifndef __UTIL_COM_COMCONTAINER_H__
#define __UTIL_COM_COMCONTAINER_H__

#pragma once

namespace Util {
    namespace Com {
        //environment, transaction, lifetime via ComClass, server, host, context
        class ComContainer {
        public:
            ComContainer();
            virtual ~ComContainer();
        };
    }
}

#endif //__UTIL_COM_COMCONTAINER_H__
