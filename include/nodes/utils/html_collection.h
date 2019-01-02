//
// Created by prem on 12/13/2018.
//

#ifndef FEATHER_HTML_COLLECTION_H
#define FEATHER_HTML_COLLECTION_H

#include "observable/invalidatable.h"
#include "observable/observable_list.h"
#include "../node.h"

namespace feather {
    namespace dom {

        class Element;

        Pair<bool, StrongPointer<Element>> nodeToElement(const StrongPointer<Node> &p);

        class HTMLCollection
                : public observable::SketchyObservableListWrapper<StrongPointer<Node>, NodeList,
                        StrongPointer<Element>, nodeToElement> {
            StrongPointer<Element> getNamedItem(DOMString name) const;

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override;
        };
    }
}
#endif //FEATHER_HTML_COLLECTION_H
