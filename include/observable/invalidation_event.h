//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATION_EVENT_H
#define FEATHER_INVALIDATION_EVENT_H

#include "typedefs.h"

namespace feather {
    namespace observable {
        enum class InvEvent {
            INVALIDATE_THIS,
            STOP_PROPAGATION,
            REBOUND,
            PROPAGATE_DOWNWARD,
            __MANGLE_NONLOCAL__,
            PARENT_CHANGE,
            CHILDREN_CHANGE,
            NODE_INDEX_CHANGE,
            NODE_VALUE_CHANGE,
            ATTRIBUTE_INTERNAL_CHANGE,
            ATTRIBUTE_SIZE_CHANGE,
            CLASS_CHANGE,
            SHADOW_ROOT_CHANGE,
            CONTENTEDITABLE_CHANGE,
            STYLE_PROPERTY_CHANGE,
            __MANGLE__LOCAL__,
            LOCAL_PARENT_CHANGE,
            LOCAL_CHILDREN_CHANGE,
            LOCAL_NODE_INDEX_CHANGE,
            LOCAL_NODE_VALUE_CHANGE,
            LOCAL_ATTRIBUTE_INTERNAL_CHANGE,
            LOCAL_ATTRIBUTE_SIZE_CHANGE,
            LOCAL_CLASS_CHANGE,
            LOCAL_SHADOW_ROOT_CHANGE,
            LOCAL_CONTENTEDITABLE_CHANGE,
            LOCAL_STYLE_PROPERTY_CHANGE,
            __COUNT__
        };
    }
}
#endif //FEATHER_INVALIDATION_EVENT_H
