#ifndef __UIENGINEER_LOGICAL_CREATOR_H__
#define __UIENGINEER_LOGICAL_CREATOR_H__

#include "../Xml/Element.h"

namespace UIEngineer {
    namespace Logical {
        class Creator {
        public:
            static CWnd* const Create(CWnd* const parent, Xml::Element const& element);
        };
    }
}

#endif //__UIENGINEER_LOGICAL_CREATOR_H__
