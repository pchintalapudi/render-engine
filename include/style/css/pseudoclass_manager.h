//
// Created by prem on 12/14/2018.
//

#ifndef FEATHER_PSEUDOCLASS_MANAGER_H
#define FEATHER_PSEUDOCLASS_MANAGER_H

#include "observable/invalidatable.h"

namespace feather {
    namespace dom {
        class Node;

        class Element;

        class ShadowRoot;
    }
    namespace css {
        class PseudoclassManager : observable::Invalidatable {
        public:
            inline void setActive(StrongPointer <Vector<feather::StrongPointer<dom::Element>>> active) {
                moused = active;
                this->active = true;
                invalidate();
            }

            inline bool isActive(StrongPointer <dom::Element> e) {
                //Have reason to believe most people look for the active element
                //closest to the end, rather than the beginning
                return moused && active && std::find(moused->rbegin(), moused->rend(), e) != moused->rend();
            }

            inline void setHover(StrongPointer <Vector<feather::StrongPointer<dom::Element>>> hover) {
                moused = hover;
                active = false;
                invalidate();
            }

            inline bool isHover(StrongPointer <dom::Element> e) {
                //See reasoning for isActive above
                return moused && !active && std::find(moused->rbegin(), moused->rend(), e) != moused->rend();
            }

            inline void setFocused(StrongPointer <Vector<feather::StrongPointer<dom::Element>>> focused) {
                this->focused = focused;
                invalidate();
            }

            inline bool isFocused(StrongPointer <dom::Element> e) { return focused && focused->back() == e; }

            inline bool containsFocus(StrongPointer <dom::Element> e) {
                return focused && std::find(focused->rbegin(), focused->rend(), e) != focused->rend();
            }

        private:
            StrongPointer <Vector<feather::StrongPointer<dom::Element>>> moused;
            bool active;
            StrongPointer <Vector<feather::StrongPointer<dom::Element>>> focused;

            inline void invalidate() const {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>(
                                1u << static_cast<int>(observable::InvEvent::PSEUDOCLASS_CHANGE)
                                | 1u << static_cast<int>(observable::InvEvent::RESTYLE)
                        ), this
                );
            }
        };
    }
}
#endif //FEATHER_PSEUDOCLASS_MANAGER_H
