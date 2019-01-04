//
// Created by binhe on 12/16/2018.
//

#ifndef FEATHER_STYLE_SHEET_H
#define FEATHER_STYLE_SHEET_H

#include "nodes/node.h"

namespace feather {
    namespace css {
        typedef Vector<DOMString> MediaList;
        class StyleSheet {
        public:
            StyleSheet();
            bool getDisabled();
            DOMString getHref();
            MediaList getMedia();

            StrongPointer<feather::dom::Node> getOwnerNode();
            StrongPointer<StyleSheet> getParentStyleSheet();
            DOMString getTitle();
            DOMString getType();
        };
    }
}

#endif //FEATHER_STYLE_SHEET_H
