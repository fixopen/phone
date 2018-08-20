#ifndef __UTIL_COM_COMSESSION_H__
#define __UTIL_COM_COMSESSION_H__

#pragma once

namespace Util {
    namespace Com {
        //user, security, thread, exception, connection-point
        class ComSession {
        public:
            ComSession();
            virtual ~ComSession();
        };
    }
}

#endif //__UTIL_COM_COMSESSION_H__
