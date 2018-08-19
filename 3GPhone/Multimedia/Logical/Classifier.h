#ifndef __MULTIMEDIA_LOGICAL_CLASSIFIER_H__
#define __MULTIMEDIA_LOGICAL_CLASSIFIER_H__

#include <string>

namespace Multimedia {
    namespace Logical {
        class Classifier {
        public:
            void OnMultimediaArraived(std::string const& localFilename);
        };
    }
}

#endif //__MULTIMEDIA_LOGICAL_CLASSIFIER_H__
