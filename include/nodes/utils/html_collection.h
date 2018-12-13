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

        class HTMLCollection : public observable::Invalidatable {
        public:
            explicit HTMLCollection(StrongPointer<observable::ObservableList<StrongPointer<Node>>> childNodes)
                    : childNodes(childNodes) {}

            inline UInt size() const { return getVector().size(); }

            inline StrongPointer<Element> getItem(UInt idx) const { return getVector()[idx].lock(); }

            StrongPointer<Element> getNamedItem(DOMString name) const;

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override;

        private:
            mutable std::vector<WeakPointer<Element>> cached = Vector<WeakPointer<Element>>();
            StrongPointer<observable::ObservableList<StrongPointer<Node>>> childNodes;

            std::vector<WeakPointer<Element>> &getVector() const;
        };
    }
}
#endif //FEATHER_HTML_COLLECTION_H
