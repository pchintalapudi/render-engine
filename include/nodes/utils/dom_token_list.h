//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_DOM_TOKEN_LIST_H
#define FEATHER_DOM_TOKEN_LIST_H

#include "observable/invalidatable.h"

namespace feather {
    namespace dom {
        class DOMTokenList : public observable::Invalidatable {
        public:
            inline UInt getLength() const { return source.size(); }

            DOMString getValue() const;

            void setValue(DOMString value);

            inline DOMString getItem(UInt idx) const { return source[idx]; }

            void add(DOMString value);

            void remove(DOMString value);

            bool toggle(DOMString value);

            bool toggle(DOMString value, bool force);

            bool replace(DOMString old, DOMString replace);

            bool supports(DOMString feature) const {
                return features.get() && features->find(feature) != features->end();
            }

        private:
            mutable DOMString cached = "";
            Vector<DOMString> source = Vector<DOMString>();
            StrongPointer<Set<DOMString>> features;
        };
    }
}
#endif //FEATHER_DOM_TOKEN_LIST_H