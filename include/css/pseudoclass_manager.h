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
            inline void setActive(StrongPointer<Vector<feather::StrongPointer<dom::Element>>> active) {
                moused = active;
                this->active = true;
                invalidate();
            }

            inline bool isActive(StrongPointer<dom::Element> e) {
                //Have reason to believe most people look for the active element
                //closest to the end, rather than the beginning
                return moused && active && std::find(moused->rbegin(), moused->rend(), e) != moused->rend();
            }

            inline void setHover(StrongPointer<Vector<feather::StrongPointer<dom::Element>>> hover) {
                moused = hover;
                active = false;
                invalidate();
            }

            inline bool isHover(StrongPointer<dom::Element> e) {
                //See reasoning for isActive above
                return moused && !active && std::find(moused->rbegin(), moused->rend(), e) != moused->rend();
            }

            inline void setFocused(StrongPointer<Vector<feather::StrongPointer<dom::Element>>> focused) {
                this->focused = focused;
                invalidate();
            }

            inline bool isFocused(StrongPointer<dom::Element> e) { return focused && focused->back() == e; }

            inline bool containsFocus(StrongPointer<dom::Element> e) {
                return focused && std::find(focused->rbegin(), focused->rend(), e) != focused->rend();
            }

            //document-stateless pseudoclasses
            static bool isAnyLink(StrongPointer<dom::Element> e);

            static bool isBlank(StrongPointer<dom::Element> e);

            static bool isChecked(StrongPointer<dom::Element> e);

            static bool isDefault(StrongPointer<dom::Element> e);

            static bool isDir(StrongPointer<dom::Element> e, bool ltr);

            static bool isDisabled(StrongPointer<dom::Element> e);

            static bool isEmpty(StrongPointer<dom::Element> e);

            static bool isEnabled(StrongPointer<dom::Element> e);

            static bool isFirstChild(StrongPointer<dom::Element> e);

            static bool isFirstOfType(StrongPointer<dom::Element> e, DOMString type);

            static bool isFullscreen(StrongPointer<dom::Element> e);

            static bool has(StrongPointer<dom::Element> e,
                            Vector<Function<bool(feather::StrongPointer<dom::Element>)>> funcs);

            static bool isHost(StrongPointer<dom::Node> n);

            static bool isHost(StrongPointer<dom::Node> n, Function<bool(feather::StrongPointer<dom::Element>)> sel);

            static bool isHostContext(StrongPointer<dom::Node> n,
                                      Function<bool(feather::StrongPointer<dom::Element>)> sel);

            static bool isIndeterminate(StrongPointer<dom::Element> e);

            static bool isInRange(StrongPointer<dom::Element> e);

            static bool isInvalid(StrongPointer<dom::Element> e);

            static bool is(StrongPointer<dom::Element> e,
                           Vector<Function<bool(feather::StrongPointer<dom::Element>)>> funcs);

            static bool isLang(StrongPointer<dom::Element> e, DOMString lang);

            static bool isLastChild(StrongPointer<dom::Element> e);

            static bool isLastOfType(StrongPointer<dom::Element> e, DOMString type);

            static bool isLink(StrongPointer<dom::Element> e);

            inline static bool isNot(StrongPointer<dom::Element> e,
                                     Function<bool(StrongPointer<dom::Element>)> func) { return !func(e); }

            static bool isNthChild(StrongPointer<dom::Element> e, Long a, Long b);

            static bool isNthLastChild(StrongPointer<dom::Element> e, Long a, Long b);

            static bool isNthOfType(StrongPointer<dom::Element> e, Long a, Long b, DOMString type);

            static bool isNthLastOfType(StrongPointer<dom::Element> e, Long a, Long b, DOMString type);

            static bool isOnlyChild(StrongPointer<dom::Element> e);

            static bool isOnlyOfType(StrongPointer<dom::Element> e, DOMString type);

            static bool isOptional(StrongPointer<dom::Element> e);

            static bool isPlaceholderShown(StrongPointer<dom::Element> e);

            static bool isReadOnly(StrongPointer<dom::Element> e);

            static bool isReadWrite(StrongPointer<dom::Element> e);

            static bool isRequired(StrongPointer<dom::Element> e);

            static bool isRoot(StrongPointer<dom::Element> e);

            inline static bool isScope(StrongPointer<dom::Element> e, StrongPointer<dom::Element> scope) {
                return e == scope;
            }

            static bool isTarget(StrongPointer<dom::Element> e);

            static bool isValid(StrongPointer<dom::Element> e);

            static bool isVisited(StrongPointer<dom::Element> e);

            inline static bool isWhere(StrongPointer<dom::Element> e,
                                       Vector<Function<bool(feather::StrongPointer<dom::Element>)>> funcs) {
                return is(e, funcs);
            }

        private:
            StrongPointer<Vector<feather::StrongPointer<dom::Element>>> moused = nullptr;
            bool active = false;
            StrongPointer<Vector<feather::StrongPointer<dom::Element>>> focused = nullptr;

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
