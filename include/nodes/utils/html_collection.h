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
        public:
            explicit HTMLCollection(StrongPointer<NodeList> nodeList);

            StrongPointer<Element> getNamedItem(DOMString name) const;
        };

        Pair<bool, StrongPointer<Element>> formFilter(const StrongPointer<Element> &p);

        class HTMLFormControlsCollection
                : public observable::SketchyObservableListWrapper<StrongPointer<Element>,
                        HTMLCollection, StrongPointer<Element>, formFilter> {
        public:
            StrongPointer<Element> getNamedItem(DOMString name) const;
        };

        class RadioNodeListFilter {
        public:
            bool operator()(const feather::StrongPointer<const Element> &p,
                            feather::Vector<feather::StrongPointer<Element>> &addTo);

        private:
            DOMString filter;
        };

        class RadioNodeList
                : public observable::RiskyFilteredList<StrongPointer<Element>, Element, RadioNodeListFilter> {
        public:
            explicit RadioNodeList(DOMString filter);
        };
    }
}
#endif //FEATHER_HTML_COLLECTION_H
