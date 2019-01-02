//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATION_EVENT_H
#define FEATHER_INVALIDATION_EVENT_H

#include "typedefs.h"

namespace feather {
    namespace observable {
        typedef Long EventCountSize;

        enum class InvEvent {
            INVALIDATE_THIS,
            CONTINUE_PROPAGATION,
            REBOUND,
            PROPAGATE_UPWARDS,
            PARENT_CHANGE,
            CHILDREN_CHANGE,
            NODE_INDEX_CHANGE,
            NODE_VALUE_CHANGE,
            ATTRIBUTE_INTERNAL_CHANGE,
            ATTRIBUTE_SIZE_CHANGE,
            CLASS_CHANGE,
            SHADOW_ROOT_CHANGE,
            ELEMENT_INDEX_CHANGE,
            CONTENTEDITABLE_CHANGE,
            PSEUDOCLASS_CHANGE,
            __MANGLE__LOCAL__,
            LOCAL_PARENT_CHANGE,
            LOCAL_CHILDREN_CHANGE,
            LOCAL_NODE_INDEX_CHANGE,
            LOCAL_NODE_VALUE_CHANGE,
            LOCAL_ATTRIBUTE_INTERNAL_CHANGE,
            LOCAL_ATTRIBUTE_SIZE_CHANGE,
            LOCAL_CLASS_CHANGE,
            LOCAL_SHADOW_ROOT_CHANGE,
            LOCAL_ELEMENT_INDEX_CHANGE,
            LOCAL_CONTENTEDITABLE_CHANGE,
            __COUNT__
        };
    }
}
#endif //FEATHER_INVALIDATION_EVENT_H
