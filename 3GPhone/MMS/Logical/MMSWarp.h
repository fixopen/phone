#ifndef __MMS_LOGICAL_MMSWARP_H__
#define __MMS_LOGICAL_MMSWARP_H__

#include <string>

namespace MMS {
    namespace Logical {
        class MMSWarp {
        public:
            void OnMMSArraived(std::string const& localFilename);
        };
    }
}

#endif // __MMS_LOGICAL_MMSWARP_H__
