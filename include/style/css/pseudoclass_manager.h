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
            inline void setActive(Vector<feather::StrongPointer<const dom::Element>> active) {
                moused = std::move(active);
                this->active = true;
                invalidate();
            }

            inline bool isActive(const StrongPointer<const dom::Element> &e) const {
                //Have reason to believe most people look for the active element
                //closest to the end, rather than the beginning
                return !moused.empty() && active && std::find(moused.rbegin(), moused.rend(), e) != moused.rend();
            }

            inline void clearActive() { moused.clear(); }

            inline void setHover(Vector<feather::StrongPointer<const dom::Element>> hover) {
                moused = std::move(hover);
                active = false;
                invalidate();
            }

            inline bool isHover(const StrongPointer<const dom::Element> &e) const {
                //See reasoning for isActive above
                return !moused.empty() && !active && std::find(moused.rbegin(), moused.rend(), e) != moused.rend();
            }

            inline void clearHover() { moused.clear(); }

            inline void setFocused(Vector<feather::StrongPointer<const dom::Element>> focused) {
                this->focused = std::move(focused);
                invalidate();
            }

            inline bool isFocused(const StrongPointer<const dom::Element> &e) const {
                return !focused.empty() && focused.front() == e;
            }

            inline bool isFocusVisible(const StrongPointer<const dom::Element> &e) const {
                return isFocused(e) && false;
            }

            inline bool containsFocus(const StrongPointer<const dom::Element> &e) const {
                return !focused.empty() && std::find(focused.begin(), focused.end(), e) != focused.end();
            }

            inline void clearFocused() { focused.clear(); }

            //document-stateless pseudoclasses
            static bool isAnyLink(const StrongPointer<const dom::Element> &e);

            static bool isBlank(const StrongPointer<const dom::Element> &e);

            static bool isChecked(const StrongPointer<const dom::Element> &e);

            static bool isDefault(const StrongPointer<const dom::Element> &e);

            static bool isDefined(const StrongPointer<const dom::Element> &e);

            static bool isDir(const StrongPointer<const dom::Element> &e, bool ltr);

            static bool isDisabled(const StrongPointer<const dom::Element> &e);

            static bool isEmpty(const StrongPointer<const dom::Element> &e);

            static bool isEnabled(const StrongPointer<const dom::Element> &e);

            static bool isFirstChild(const StrongPointer<const dom::Element> &e);

            static bool isFirstOfType(const StrongPointer<const dom::Element> &e, const DOMString &type);

            static bool isFullscreen(const StrongPointer<const dom::Element> &e);

            static bool isHost(const StrongPointer<const dom::Element> &e);

            static bool isIndeterminate(const StrongPointer<const dom::Element> &e);

            static bool isInRange(const StrongPointer<const dom::Element> &e);

            static bool isInvalid(const StrongPointer<const dom::Element> &e);

            static bool isLang(const StrongPointer<const dom::Element> &e, const DOMString &lang);

            static bool isLastChild(const StrongPointer<const dom::Element> &e);

            static bool isLastOfType(const StrongPointer<const dom::Element> &e, const DOMString &type);

            static bool isLink(const StrongPointer<const dom::Element> &e);

            static bool isNthChild(const StrongPointer<const dom::Element> &e, Long a, Long b);

            static bool isNthLastChild(const StrongPointer<const dom::Element> &e, Long a, Long b);

            static bool isNthOfType(const StrongPointer<const dom::Element> &e, Long a, Long b, const DOMString &type);

            static bool
            isNthLastOfType(const StrongPointer<const dom::Element> &e, Long a, Long b, const DOMString &type);

            static bool isOnlyChild(const StrongPointer<const dom::Element> &e);

            static bool isOnlyOfType(const StrongPointer<const dom::Element> &e, const DOMString &type);

            static bool isOptional(const StrongPointer<const dom::Element> &e);

            static bool isOutOfRange(const StrongPointer<const dom::Element> &e);

            static bool isPlaceholderShown(const StrongPointer<const dom::Element> &e);

            static bool isReadOnly(const StrongPointer<const dom::Element> &e);

            static bool isReadWrite(const StrongPointer<const dom::Element> &e);

            static bool isRequired(const StrongPointer<const dom::Element> &e);

            static bool isRoot(const StrongPointer<const dom::Element> &e);

            inline static bool
            isScope(const StrongPointer<const dom::Element> &e, const StrongPointer<const dom::Element> &scope) {
                return e == scope;
            }

            static bool isTarget(const StrongPointer<const dom::Element> &e);

            static bool isValid(const StrongPointer<const dom::Element> &e);

            static bool isVisited(const StrongPointer<const dom::Element> &e);

        private:
            Vector<feather::StrongPointer<const dom::Element>> moused = Vector<StrongPointer<const dom::Element>>();
            bool active = false;
            Vector<feather::StrongPointer<const dom::Element>> focused = Vector<StrongPointer<const dom::Element>>();

            inline void invalidate() const {
                //TODO: actually invShort
            }
        };
    }
}
#endif //FEATHER_PSEUDOCLASS_MANAGER_H
