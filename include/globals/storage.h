//
// Created by prem on 1/10/2019.
//

#ifndef CURL_STORAGE_H
#define CURL_STORAGE_H

#include "typedefs.h"

namespace feather {
    namespace storage {
        class Storage {
            DOMString getKey(UInt idx);

            DOMString getItem(DOMString key);

            void setItem(DOMString key, DOMString val);

            void removeItem(DOMString key);

            void clear();
        };
    }
}
#endif //CURL_STORAGE_H
