//
// Created by prem on 12/15/2018.
//

#include <iostream>
#include "nodes/elements/selectors/css_selector.h"
#include "nodes/elements/element.h"
#include "nodes/documents/document.h"
#include "nodes/documents/shadow_root.h"

using namespace feather::dom::selector;

namespace {
    feather::DOMString toLowercase(const feather::DOMString &s) {
        feather::DOMString str{};
        str.reserve(s.length());
        for (auto c : s) str += static_cast<char>(std::tolower(c));
        return str;
    }
}

CSSAttributeSelector::CSSAttributeSelector(feather::DOMString attr, feather::DOMString val,
                                           feather::dom::selector::CSSAttributeType type,
                                           feather::TriValue caseSensitive)
        : attr(std::move(attr)), type(type), caseSensitive(caseSensitive),
          val(caseSensitive == TriValue::NO ? toLowercase(val) : std::move(val)) {}

feather::DOMString CSSAttributeSelector::toString() const {
    switch (type) {
        case CSSAttributeType::CONTAINS:
            return "[" + attr + " *= '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        case CSSAttributeType::EQUALS:
            return "[" + attr + " = '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        case CSSAttributeType::HYPHEN:
            return "[" + attr + " |= '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        case CSSAttributeType::LIST:
            return "[" + attr + " ~= '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        case CSSAttributeType::PREFIX:
            return "[" + attr + " ^= '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        case CSSAttributeType::PRESENT:
            return "[" + attr + "]";
        case CSSAttributeType::SUFFIX:
            return "[" + attr + " $= '" + val + "'" + (caseSensitive == TriValue::NO ? " i" : "") + "]";
        default:
            return "";
    }
}

bool CSSAttributeSelector::matches(const feather::StrongPointer<const feather::dom::Element> &element) const {
    switch (type) {
        case CSSAttributeType::CONTAINS: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            return val.find(this->val) != DOMString::npos;
        }
        case CSSAttributeType::PRESENT:
            return element->hasAttribute(attr);
        case CSSAttributeType::PREFIX: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            return val.length() >= this->val.length() && val.substr(0, this->val.length()) == this->val;
        }
        case CSSAttributeType::SUFFIX: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            return val.length() >= this->val.length() &&
                   val.substr(val.length() - this->val.length(), this->val.length()) == this->val;
        }
        case CSSAttributeType::LIST: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            auto idx = val.find(this->val);
            auto end = idx + this->val.length();
            return idx != DOMString::npos && (idx == 0 || isspace(val[idx - 1])) &&
                   (end == val.length() || isspace(val[end]));
        }
        case CSSAttributeType::HYPHEN: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            return val == this->val ||
                   (val.length() > this->val.length() && val.substr(0, this->val.length() + 1) == this->val + '-');
        }
        case CSSAttributeType::EQUALS: {
            auto val = element->getAttributeSafe(attr);
            if (caseSensitive == TriValue::NO) val = toLowercase(val);
            return val == this->val;
        }
        default:
            return false;
    }
}

namespace {

    feather::DOMString toString(const feather::DOMString &pclass,
                                const feather::Vector<feather::dom::selector::CSSSelector> &selectors) {
        feather::Vector<feather::DOMString> temp{};
        feather::DOMString concat{};
        feather::UInt reserve = pclass.length() + 1;
        for (const auto &sel : selectors) {
            auto str = sel.toString();
            reserve += str.length() + 1;
            temp.push_back(str);
        }
        concat.reserve(reserve);
        concat += pclass + "(";
        for (const auto &str : temp) concat += str + ',';
        concat.back() = ')';
        return concat;
    }

    feather::DOMString toString(const feather::DOMString &pclass, feather::Long a, feather::Long b) {
        return a && a - 1 ? b ? pclass + "(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                              : pclass + "(" + std::to_string(a) + "n)"
                          : a ? b ? pclass + "(n + " + std::to_string(b) + ")"
                                  : pclass + "(n)" : (pclass + "(" + std::to_string(b) + ")");
    }

    //Lots and lots of pseudoclasses yay! (not :( )

    class ActivePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":active"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            auto doc = element->getRootNode();
            if (doc) {
                switch (doc->getNodeTypeInternal()) {
                    case feather::dom::NodeType::DOCUMENT_NODE:
                        return std::static_pointer_cast<feather::dom::Document>(doc)
                                ->getPseudoclassManager().isActive(element);
                    case feather::dom::NodeType::SHADOW_ROOT:
                        return std::static_pointer_cast<feather::dom::ShadowRoot>(doc)
                                ->getPseudoclassManager().isActive(element);
                    default:
                        break;
                }
            }
            return false;
        }


    };

    class AnyLinkPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":any-link"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isAnyLink(element);
        }


    };

    class BlankPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":blank"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isBlank(element);
        }


    };

    class CheckedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":checked"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isChecked(element);
        }


    };

    class DefaultPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":default"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isDefault(element);
        }


    };

    class DefinedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":defined"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isDefined(element);
        }


    };

    class DirPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit DirPseudoclass(bool ltr) : ltr(ltr) {}

        feather::DOMString toString() const override { return ":dir(" + feather::DOMString(ltr ? "ltr" : "rtl") + ")"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isDir(element, ltr);
        }


    private:
        const bool ltr;
    };

    class DisabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":disabled"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isDisabled(element);
        }


    };

    class EmptyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":empty"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isEmpty(element);
        }


    };

    class EnabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":enabled"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isEnabled(element);
        }


    };

    class FirstChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":first-child"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isFirstChild(element);
        }


    };

    class FirstOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit FirstOfTypePseudoclass(feather::DOMString type) : type(std::move(type)) {}

        feather::DOMString toString() const override { return ":first-of-type"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isFirstOfType(element, type);
        }


    private:
        feather::DOMString type{};
    };

    class FullscreenPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":fullscreen"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isFullscreen(element);
        }


    };

    class FocusPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":focus"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            auto doc = element->getRootNode();
            if (doc) {
                switch (doc->getNodeTypeInternal()) {
                    case feather::dom::NodeType::DOCUMENT_NODE:
                        return std::static_pointer_cast<feather::dom::Document>(doc)
                                ->getPseudoclassManager().isFocused(element);
                    case feather::dom::NodeType::SHADOW_ROOT:
                        return std::static_pointer_cast<feather::dom::ShadowRoot>(doc)
                                ->getPseudoclassManager().isFocused(element);
                    default:
                        break;
                }
            }
            return false;
        }


    };

    class FocusVisiblePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":focus-visible"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            auto doc = element->getRootNode();
            if (doc) {
                switch (doc->getNodeTypeInternal()) {
                    case feather::dom::NodeType::DOCUMENT_NODE:
                        return std::static_pointer_cast<feather::dom::Document>(doc)
                                ->getPseudoclassManager().isFocusVisible(element);
                    case feather::dom::NodeType::SHADOW_ROOT:
                        return std::static_pointer_cast<feather::dom::ShadowRoot>(doc)
                                ->getPseudoclassManager().isFocusVisible(element);
                    default:
                        break;
                }
            }
            return false;
        }


    };

    class FocusWithinPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":focus-within"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            auto doc = element->getRootNode();
            if (doc) {
                switch (doc->getNodeTypeInternal()) {
                    case feather::dom::NodeType::DOCUMENT_NODE:
                        return std::static_pointer_cast<feather::dom::Document>(doc)
                                ->getPseudoclassManager().containsFocus(element);
                    case feather::dom::NodeType::SHADOW_ROOT:
                        return std::static_pointer_cast<feather::dom::ShadowRoot>(doc)
                                ->getPseudoclassManager().containsFocus(element);
                    default:
                        break;
                }
            }
            return false;
        }


    };

    class HasPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HasPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(std::move(selectors)) {}

        feather::DOMString toString() const override {
            return ::toString(":has", selectors);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            if (element) for (const auto &selector : selectors) if (selector.querySelector(element)) return true;
            return false;
        }


    private:
        feather::Vector<CSSSelector> selectors{};
    };

    class HostPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":host"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isHost(element);
        }
    };

    class HostManagedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostManagedPseudoclass(feather::Vector<CSSSelector> selectors) : selectors(std::move(selectors)) {}

        feather::DOMString toString() const override {
            return ::toString(":host", selectors);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (const auto &sel : selectors) if (sel.matches(element->getParentElement())) return true;
            }
            return false;
        }

    private:
        feather::Vector<CSSSelector> selectors{};
    };

    class HostContextPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostContextPseudoclass(feather::Vector<CSSSelector> selectors) : selectors(std::move(selectors)) {}

        feather::DOMString toString() const override {
            return ::toString(":host-context", selectors);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (const auto &sel : selectors) {
                    auto parent = element->getParentElement();
                    while (parent && !sel.matches(parent)) parent = parent->getParentElement();
                    if (parent) return true;
                }
            }
            return false;
        }

    private:
        const feather::Vector<CSSSelector> selectors{};
    };

    class HoverPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
        feather::DOMString toString() const override { return ":hover"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            auto doc = element->getRootNode();
            if (doc) {
                switch (doc->getNodeTypeInternal()) {
                    case feather::dom::NodeType::DOCUMENT_NODE:
                        return std::static_pointer_cast<feather::dom::Document>(doc)
                                ->getPseudoclassManager().isHover(element);
                    case feather::dom::NodeType::SHADOW_ROOT:
                        return std::static_pointer_cast<feather::dom::ShadowRoot>(doc)
                                ->getPseudoclassManager().isHover(element);
                    default:
                        break;
                }
            }
            return false;
        }


    };

    class IndeterminatePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":indeterminate"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isIndeterminate(element);
        }
    };

    class InRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":in-range"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isInRange(element);
        }


    };

    class InvalidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":invalid"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isInvalid(element);
        }


    };

    class IsPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit IsPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(std::move(selectors)) {}

        feather::DOMString toString() const override {
            return ::toString(":is", selectors);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            if (element) for (const auto &sel : selectors) if (sel.matches(element)) return true;
            return false;
        }


    private:
        const feather::Vector<CSSSelector> selectors{};
    };

    class LangPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LangPseudoclass(feather::DOMString lang = "en-US") : lang(std::move(lang)) {}

        feather::DOMString toString() const override {
            return ":lang(" + lang + ")";
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isLang(element, lang);
        }


    private:
        const feather::DOMString lang{};
    };

    class LastChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override {
            return ":last-child";
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isLastChild(element);
        }


    };

    class LastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LastOfTypePseudoclass(feather::DOMString type) : type(std::move(type)) {}

        feather::DOMString toString() const override {
            return ":last-of-type";
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isLastOfType(element, type);
        }


    private:
        const feather::DOMString type{};
    };

    class LinkPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override {
            return ":link";
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isLink(element);
        }


    };

    class NotPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit NotPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(std::move(selectors)) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 5;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (const auto &sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string = ":not(";
            string.reserve(reserve);
            for (const auto &str : temp) (string += str) + ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            if (!element) return false;
            for (const auto &sel : selectors) if (sel.matches(element)) return false;
            return true;
        }


    private:
        feather::Vector<feather::dom::selector::CSSSelector> selectors{};
    };

    class NthChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthChildPseudoclass(feather::Long a, feather::Long b) : a(a), b(b) {}

        feather::DOMString toString() const override {
            return ::toString(":nth-child", a, b);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isNthChild(element, a, b);
        }


    private:
        feather::Long a = 0, b = 0;
    };

    class NthOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString type) : a(a), b(b),
                                                                                          type(std::move(type)) {}

        feather::DOMString toString() const override {
            return ::toString(":nth-of-type", a, b);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isNthOfType(element, a, b, type);
        }


    private:
        feather::Long a = 0, b = 0;
        feather::DOMString type{};
    };

    class NthLastChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        NthLastChildPseudoclass(feather::Long a, feather::Long b) : a(a), b(b) {}

        feather::DOMString toString() const override {
            return ::toString(":nth-last-child", a, b);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isNthLastChild(element, a, b);
        }


    private:
        const feather::Long a = 0, b = 0;
    };

    class NthLastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthLastOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString type)
                : a(a), b(b), type(std::move(type)) {}

        feather::DOMString toString() const override {
            return ::toString(":nth-last-of-type", a, b);
        }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isNthLastOfType(element, a, b, type);
        }


    private:
        feather::Long a = 0, b = 0;
        feather::DOMString type = 0;
    };

    class OnlyChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":only-child"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isOnlyChild(element);
        }


    };

    class OnlyOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        explicit OnlyOfTypePseudoclass(feather::DOMString type) : type(std::move(type)) {}

        feather::DOMString toString() const override { return ":only-of-type"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isOnlyOfType(element, type);
        }


    private:
        feather::DOMString type{};
    };

    class OptionalPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":optional"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isOptional(element);
        }


    };

    class OutOfRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":out-of-range"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isOutOfRange(element);
        }


    };

    class PlaceholderShownPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":placeholder-shown"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isPlaceholderShown(element);
        }
    };

    class ReadOnlyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-only"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isReadOnly(element);
        }


    };

    class ReadWritePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-write"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isReadWrite(element);
        }


    };

    class RequiredPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":required"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isRequired(element);
        }


    };

    class RootPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":root"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isRoot(element);
        }


    };

    class ScopePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit ScopePseudoclass(feather::StrongPointer<const feather::dom::Element> scope)
                : scope(std::move(scope)) {}

        feather::DOMString toString() const override { return ":scope"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isScope(element, scope);
        }


    private:
        feather::StrongPointer<const feather::dom::Element> scope{};
    };

    class TargetPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":target"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isTarget(element);
        }


    };

    class ValidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":valid"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isValid(element);
        }


    };

    class VisitedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":visited"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> &element) const override {
            return feather::css::PseudoclassManager::isVisited(element);
        }


    };
}

feather::DOMString CSSToken::toString() const {
    UInt reserve = 0;
    for (const auto &cls : classes) reserve += cls.length() + 1;
    DOMString clazzes{};
    clazzes.reserve(reserve);
    for (const auto &cls : classes) clazzes += '.' + cls;
    reserve = 0;
    Vector <DOMString> temp{};
    temp.reserve(attributes.size());
    for (const auto &attr : attributes) {
        auto str = attr.toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString attrs{};
    attrs.reserve(reserve);
    for (const auto &str : temp) attrs += str;
    reserve = 0;
    temp.clear();
    for (const auto &pclass : pseudoclasses) {
        auto str = pclass->toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString pseudoclasses{};
    pseudoclasses.reserve(reserve);
    for (const auto &str : temp) pseudoclasses += str;
    return (tagName.empty() ? "*" : tagName) + (id.empty() ? "" : ("#" + id)) + clazzes + attrs + pseudoclasses;
}

bool CSSToken::matches(const feather::StrongPointer<const feather::dom::Element> &element) const {
    if (!tagName.empty() && !compareType(element, tagName)) return false;
    if (!id.empty() && element->getId() != id) return false;
    auto classList = element->getClassList();
    for (const auto &cls : classes) if (!classList->contains(cls)) return false;
    for (const auto &attr : attributes) if (!attr.matches(element)) return false;
    for (const auto &pclass : pseudoclasses) if (!pclass->matches(element)) return false;
    return true;
}

namespace {
    feather::DOMString toString(feather::dom::selector::CSSRelation r) {
        switch (r) {
            case feather::dom::selector::CSSRelation::DESCENDANT:
                return " ";
            case feather::dom::selector::CSSRelation::IMMEDIATE_DESCENDANT:
                return " > ";
            case feather::dom::selector::CSSRelation::IMMEDIATE_SIBLING:
                return " + ";
            case feather::dom::selector::CSSRelation::SIBLING:
                return " ~ ";
            default:
                return "";
        }
    }
}

CSSDescendantToken::CSSDescendantToken(
        feather::Vector<feather::dom::selector::CSSToken> group)
        : group(std::move(group)) {
    length = 0;
    for (auto it = this->group.begin() + 1; it < this->group.end(); it++)
        if (it->getRelation() == CSSRelation::IMMEDIATE_DESCENDANT) length++;
}

feather::DOMString CSSDescendantToken::toString() const {
    if (group.empty()) return "";
    UInt reserve = 0;
    Vector <DOMString> strings;
    strings.reserve(group.size());
    for (auto begin = group.begin(); begin < group.end(); begin++) {
        auto rel = ::toString(begin->getRelation());
        auto str = begin->toString();
        reserve += str.length() + rel.length();
        strings.push_back(std::move(str));
        strings.push_back(std::move(rel));
    }
    DOMString string;
    string.reserve(reserve);
    for (const auto &str : strings) string += str;
    return string;
}

bool CSSDescendantToken::matches(const feather::StrongPointer<const feather::dom::Element> &element) const {
    if (!group.back().matches(element)) return false;
    auto focus = element;
    for (auto begin = group.rbegin() + 1; begin < group.rend(); begin++) {
        switch (begin->getRelation()) {
            case CSSRelation::IMMEDIATE_DESCENDANT:
                focus = focus->getParentElement();
                if (!focus || !begin->matches(focus)) return false;
                break;
            case CSSRelation::IMMEDIATE_SIBLING:
                focus = focus->getPreviousElementSibling();
                if (!focus || !begin->matches(focus)) return false;
                break;
            case CSSRelation::SIBLING:
                while ((focus = focus->getPreviousElementSibling())) if (begin->matches(focus)) break;
                if (!focus) return false;
                break;
            default:
                break;
        }
    }
    return true;
}

feather::DOMString CSSSelector::toString() const {
    Vector <DOMString> temp;
    UInt reserve = 0;
    temp.reserve(descendants.size() * 2 + 1);
    for (const auto &pair : descendants) {
        auto token = pair.toString();
        temp.push_back(token);
        reserve += token.length() + 1;
    }
    DOMString selector;
    selector.reserve(reserve);
    for (const auto &str : temp) selector += str;
    selector.erase(selector.end() - 1);
    return selector;
}

feather::Vector<CSSDescendantToken>::const_iterator CSSSelector::preprocess(
        feather::StrongPointer<const feather::dom::Element> scope) const {
    Deque <StrongPointer<Element>> parents;
    for (auto parent = scope->getThisRef(); (parent = parent->getParentElement()); parents.push_back(parent));
    auto it = descendants.begin();
    for (auto low = parents.begin(); it < descendants.end() - 1 && low < parents.end(); low++)
        if (it->matches(*low)) low += it->size() - 1;
    return it;
}

bool CSSSelector::matches(const feather::StrongPointer<const feather::dom::Element> &element) const {
    auto scope = element->getThisRef();
    auto it = descendants.rbegin();
    while (scope && it < descendants.rend())
        if (it->matches(scope)) for (auto i = it++->size(); scope && i--; scope = scope->getParentElement());
        else scope = scope->getParentElement();
    return it == descendants.rend();
}

feather::StrongPointer<feather::dom::Element> CSSSelector::querySelectorInternal(
        feather::Vector<feather::dom::selector::CSSDescendantToken>::const_iterator begin,
        feather::StrongPointer<const feather::dom::Element> scope) const {
    if (begin->matches(scope)) {
        if (begin == descendants.end() - 1) return scope->getThisRef();
        for (const auto &child : *scope->getChildren()) {
            auto ptr = querySelectorInternal(begin + 1, child);
            if (ptr) return ptr;
        }
    } else {
        for (const auto &child : *scope->getChildren()) {
            auto ptr = querySelectorInternal(begin, child);
            if (ptr) return ptr;
        }
    }
    return nullptr;
}

void CSSSelector::querySelectorAllInternal(
        feather::Vector<feather::dom::selector::CSSDescendantToken>::const_iterator begin,
        feather::StrongPointer<const feather::dom::Element> scope,
        feather::Vector<feather::StrongPointer<feather::dom::Element>> &ref) const {
    if (begin->matches(scope)) {
        if (begin == descendants.end() - 1) { ref.push_back(scope->getThisRef()); }
        else {
            auto children = scope->getChildren();
            for (const auto &child : *scope->getChildren()) querySelectorAllInternal(begin + 1, child, ref);
        }
    } else {
        auto children = scope->getChildren();
        for (const auto &child : *scope->getChildren()) querySelectorAllInternal(begin, child, ref);
    }
}

namespace {

    //Taken from stackoverflow: https://stackoverflow.com/a/2112111
    constexpr unsigned int hasher(const char *input) {
        return *input ? static_cast<unsigned int>(*input) + 33 * hasher(input + 1) : 5381;
    }

    void parseAttrSelector(feather::DOMString::const_iterator &it,
                           feather::Vector<feather::dom::selector::CSSAttributeSelector> &attributes) {
        feather::DOMString attr;
        while (isspace(*++it));
        it--;
        while (*++it != '=' && *it != ']') if (!isspace(*it)) attr += *it;
        if (*it == '=') {
            CSSAttributeType type;
            switch (*(it - 1)) {
                default:
                    type = CSSAttributeType::EQUALS;
                    break;
                case '~':
                    type = CSSAttributeType::LIST;
                    break;
                case '^':
                    type = CSSAttributeType::PREFIX;
                    break;
                case '|':
                    type = CSSAttributeType::HYPHEN;
                    break;
                case '$':
                    type = CSSAttributeType::SUFFIX;
                    break;
                case '*':
                    type = CSSAttributeType::CONTAINS;
                    break;
            }
            if (type != CSSAttributeType::EQUALS) attr.erase(attr.end() - 1);
            while (isspace(*++it));
            bool quoted = *it == '"' || *it == '\'';
            if (!quoted) it--;
            feather::DOMString val;
            while (quoted ? *++it != '"' && *it != '\'' : ((!isspace(*++it) && *it != ']'))) val += *it;
            feather::TriValue caseSensitive = feather::TriValue::AUTO;
            if (*it != ']') {
                while (isspace(*++it) || *it == '\'' || *it == '"');
                switch (tolower(*it)) {
                    case 'i':
                        caseSensitive = feather::TriValue::NO;
                        break;
                    case 's':
                        caseSensitive = feather::TriValue::YES;
                        break;
                    default:
                        break;
                }
                while (*it++ != ']');
            } else it++;
            attributes.push_back(CSSAttributeSelector(std::move(attr), std::move(val), type, caseSensitive));
        } else {
            feather::TriValue caseSensitive = feather::TriValue::AUTO;
            //Rarely do people actually stick these in css; im willing to take the cost
            if (tolower(attr.back()) == 'i' || tolower(attr.back()) == 's') {
                auto offset = 0;
                while (isspace(*(it - ++offset)));
                if (isspace(*(it - offset - 1)))
                    caseSensitive = tolower(attr.back() == 'i') ? feather::TriValue::NO : feather::TriValue::YES;
            }
            it++;
            attributes.push_back(
                    CSSAttributeSelector(std::move(attr), "", CSSAttributeType::PRESENT, caseSensitive));
        }
    }

    feather::Pair<feather::Long, feather::Long> parseNthSel(feather::DOMString::const_iterator &it) {
        feather::Long a = 0, b = 0;
        feather::DOMString str;
        while (*++it != 'n' && *it != ')') if (!isspace(*it)) str += *it;
        if (*it == 'n') {
            a = str.empty() ? 1 : str == "-" ? -1 : str == "eve" ? 2 : std::stoll(str);
            str = "";
            while (*++it != ')') if (!isspace(*it)) str += *it;
        }
        if (str == "odd") {
            a = 2;
            b = 1;
        } else {
            b = str.empty() ? 0 : std::stoll(str);
        }
        return std::make_pair(a, b);
    }

    feather::Vector<CSSSelector>
    parseInternalSelectorList(feather::DOMString::const_iterator &it,
                              const feather::StrongPointer<const feather::dom::Element> &scope) {
        feather::UInt open = 1, count = 0;
        bool unquote = true, undquote = true;
        while (open) {
            switch (*(it + ++count)) {
                case '(':
                    if (unquote && undquote) open++;
                    break;
                case ')':
                    if (unquote && undquote) open--;
                    break;
                case '"':
                    if (unquote) undquote ^= true;
                    break;
                case '\'':
                    if (undquote) unquote ^= true;
                default:
                    break;
            }
        }
        it += count;
        return CSSSelector::parseDelegateList(it - count, it, scope);
    }

    void
    parseSpecialPseudoclass(feather::DOMString::const_iterator &it, const char *temp, const feather::DOMString &tag,
                            feather::Vector<feather::dom::selector::CSSPseudoclassSelector *> &pseudoclasses,
                            const feather::StrongPointer<const feather::dom::Element> &scope) {
        //Do special pseudoclass parsing
        switch (hasher(temp)) {
            case hasher(":dir"): {
                feather::DOMString val;
                while (*++it != ')') if (!isspace(*it)) val += *it;
                it++;
                if (val == "rtl") pseudoclasses.push_back(new DirPseudoclass(false));
                else if (val == "ltr") pseudoclasses.push_back(new DirPseudoclass(true));
                break;
            }
            case hasher(":has"): {
                pseudoclasses.push_back(new HasPseudoclass(parseInternalSelectorList(it, scope)));
                it++;
                break;
            }
            case hasher(":host"): {
                pseudoclasses.push_back(new HostManagedPseudoclass(parseInternalSelectorList(it, scope)));
                it++;
                break;
            }
            case hasher(":host-context"): {
                pseudoclasses.push_back(new HostContextPseudoclass(parseInternalSelectorList(it, scope)));
                it++;
                break;
            }
                //Special case for :where b/c specificity doesn't matter
            case hasher(":where"):
            case hasher(":is"): {
                pseudoclasses.push_back(new IsPseudoclass(parseInternalSelectorList(it, scope)));
                it++;
                break;
            }
            case hasher(":lang"): {
                feather::DOMString str;
                while (*++it != ')') str += *it;
                it++;
                pseudoclasses.push_back(new LangPseudoclass(std::move(str)));
                it++;
                break;
            }
            case hasher(":not"): {
                pseudoclasses.push_back(new NotPseudoclass(parseInternalSelectorList(it, scope)));
                it++;
                break;
            }
            case hasher(":nth-child"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthChildPseudoclass(pair.first, pair.second));
                it++;
                break;
            }
            case hasher(":nth-of-type"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthOfTypePseudoclass(pair.first, pair.second, feather::DOMString(tag)));
                it++;
                break;
            }
            case hasher(":nth-last-child"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthLastChildPseudoclass(pair.first, pair.second));
                it++;
                break;
            }
            case hasher(":nth-last-of-type"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthLastOfTypePseudoclass(pair.first, pair.second, feather::DOMString(tag)));
                it++;
                break;
            }
            default:
                break;
        }
    }

    feather::dom::selector::CSSPseudoclassSelector *
    getPseudoclass(const feather::DOMString &psel, const feather::DOMString &tag,
                   const feather::StrongPointer<const feather::dom::Element> &scope) {
        CSSPseudoclassSelector *pclass = nullptr;
        switch (hasher(psel.c_str())) {
            case hasher("active"):
                pclass = new ActivePseudoclass();
                break;
            case hasher("any-link"):
                pclass = new AnyLinkPseudoclass();
                break;
            case hasher("blank"):
                pclass = new BlankPseudoclass();
                break;
            case hasher("checked"):
                pclass = new CheckedPseudoclass();
                break;
            case hasher("default"):
                pclass = new DefaultPseudoclass();
                break;
            case hasher("defined"):
                pclass = new DefinedPseudoclass();
                break;
            case hasher("disabled"):
                pclass = new DisabledPseudoclass();
                break;
            case hasher("empty"):
                pclass = new EmptyPseudoclass();
                break;
            case hasher("enabled"):
                pclass = new EnabledPseudoclass();
                break;
            case hasher("first-child"):
                pclass = new FirstChildPseudoclass();
                break;
            case hasher("first-of-type"):
                pclass = new FirstOfTypePseudoclass(tag);
                break;
            case hasher("fullscreen"):
                pclass = new FullscreenPseudoclass();
                break;
            case hasher("focus"):
                pclass = new FocusPseudoclass();
                break;
            case hasher("focus-visible"):
                pclass = new FocusVisiblePseudoclass();
                break;
            case hasher("focus-within"):
                pclass = new FocusWithinPseudoclass();
                break;
            case hasher("host"):
                pclass = new HostPseudoclass();
                break;
            case hasher("hover"):
                pclass = new HoverPseudoclass();
                break;
            case hasher("indeterminate"):
                pclass = new IndeterminatePseudoclass();
                break;
            case hasher("in-range"):
                pclass = new InRangePseudoclass();
                break;
            case hasher("invalid"):
                pclass = new InvalidPseudoclass();
                break;
            case hasher("last-child"):
                pclass = new LastChildPseudoclass();
                break;
            case hasher("last-of-type"):
                pclass = new LastOfTypePseudoclass(tag);
                break;
            case hasher("link"):
                pclass = new LinkPseudoclass();
                break;
            case hasher("only-child"):
                pclass = new OnlyChildPseudoclass();
                break;
            case hasher("only-of-type"):
                pclass = new OnlyOfTypePseudoclass(tag);
                break;
            case hasher("optional"):
                pclass = new OptionalPseudoclass();
                break;
            case hasher("out-of-range"):
                pclass = new OutOfRangePseudoclass();
                break;
            case hasher("placeholder-shown"):
                pclass = new PlaceholderShownPseudoclass();
                break;
            case hasher("read-only"):
                pclass = new ReadOnlyPseudoclass();
                break;
            case hasher("read-write"):
                pclass = new ReadWritePseudoclass();
                break;
            case hasher("required"):
                pclass = new RequiredPseudoclass();
                break;
            case hasher("root"):
                pclass = new RootPseudoclass();
                break;
            case hasher("scope"):
                pclass = new ScopePseudoclass(scope);
                break;
            case hasher("target"):
                pclass = new TargetPseudoclass();
                break;
            case hasher("valid"):
                pclass = new ValidPseudoclass();
                break;
            case hasher("visited"):
                pclass = new VisitedPseudoclass();
                break;
            default:
                break;
        }
        return pclass;
    };

    void handleTemp(feather::DOMString temp, feather::DOMString &tag, feather::DOMString &id,
                    feather::Vector<feather::DOMString> &classes,
                    feather::Vector<feather::dom::selector::CSSPseudoclassSelector *> &pseudoclasses,
                    const feather::StrongPointer<const feather::dom::Element> &scope) {
        switch (temp[0]) {
            case '*':
                tag = "";
                break;
            case '#':
                id = temp.substr(1, temp.length() - 1);
                break;
            case ':': {
                auto pclass = getPseudoclass(temp.size() > 1 && temp[1] == ':'
                                             ? temp.substr(2, temp.size() - 2)
                                             : temp.substr(1, temp.size() - 1),
                                             feather::DOMString(tag),
                                             feather::StrongPointer<const feather::dom::Element>(scope));
                if (pclass) pseudoclasses.push_back(pclass);
                break;
            }
            case '.':
                classes.push_back(temp.substr(1, temp.length() - 1));
                break;
            default:
                tag = temp;
                break;
        }
        temp = "";
    }

    void spaceFound(feather::DOMString &temp, feather::DOMString &tag, feather::DOMString &id,
                    feather::Vector<feather::DOMString> &classes,
                    feather::Vector<feather::dom::selector::CSSAttributeSelector> attributes,
                    feather::Vector<feather::dom::selector::CSSPseudoclassSelector *> &pseudoclasses,
                    const feather::StrongPointer<const feather::dom::Element> &scope, CSSRelation &relation,
                    feather::Vector<CSSToken> &rels, feather::Vector<CSSDescendantToken> &descendants, bool &begun) {
        if (!temp.empty()) {
            handleTemp(std::move(temp), tag, id, classes, pseudoclasses, scope);
            temp = "";
        }
        if (begun || !(begun = true)) {
            if (!static_cast<int>(relation)) {
                descendants.emplace_back(std::move(rels));
                rels = feather::Vector<CSSToken>();
            }
        }
        rels.emplace_back(std::move(tag), std::move(id), std::move(classes), std::move(attributes),
                          std::move(pseudoclasses), relation);
        tag = "";
        id = "";
        classes = feather::Vector<feather::DOMString>();
        attributes = feather::Vector<CSSAttributeSelector>();
        pseudoclasses = feather::Vector<CSSPseudoclassSelector *>();
        relation = CSSRelation::DESCENDANT;
    }
}

feather::dom::selector::CSSSelector
CSSSelector::parseDelegate(feather::DOMString::const_iterator begin, feather::DOMString::const_iterator end,
                           feather::StrongPointer<const feather::dom::Element> scope) {
    DOMString temp;
    temp.reserve(static_cast<ULong>(end - begin));
    DOMString tag;
    DOMString id;
    Vector <DOMString> classes;
    Vector <CSSAttributeSelector> attributes;
    Vector < CSSPseudoclassSelector * > pseudoclasses;
    CSSToken last = CSSToken("", "", Vector<DOMString>(), Vector<CSSAttributeSelector>(),
                             Vector<CSSPseudoclassSelector *>(), CSSRelation::DESCENDANT);
    Vector <CSSToken> rels = Vector<CSSToken>();
    Vector <CSSDescendantToken> descendants;
    bool spaceFound = false;
    CSSRelation relation = CSSRelation::DESCENDANT;
    DOMString::const_iterator it = begin;
    bool begun = false;
    while (isspace(*it++));
    it--;
    while (isspace(*end--));
    end++;
    while (it < end) {
        switch (*it) {
            default:
                if (spaceFound && !(spaceFound = false)) {
                    ::spaceFound(temp, tag, id, classes, attributes, pseudoclasses, scope,
                                 relation, rels, descendants, begun);
                }
                temp += *it++;
                break;
            case '[':
                //Do attr parsing
            {
                if (!temp.empty()) {
                    handleTemp(std::move(temp), tag, id, classes, pseudoclasses, scope);
                    temp = "";
                }
                if (spaceFound && !(spaceFound = false)) {
                    ::spaceFound(temp, tag, id, classes, attributes, pseudoclasses, scope,
                                 relation, rels, descendants, begun);
                }
                ::parseAttrSelector(it, attributes);
                break;
            }
            case ']':
                //Error: fail me silently
                break;
            case '(':
                ::parseSpecialPseudoclass(it, temp.c_str(), tag, pseudoclasses, scope);
                it++;
                break;
            case ')':
                //Error: fail me silently
                break;
            case '*':
            case '.':
            case ':':
            case '#':
                if (!temp.empty() && !(*it == ':' && temp == ":")) {
                    handleTemp(std::move(temp), tag, id, classes, pseudoclasses, scope);
                    temp = "";
                }
                if (spaceFound && !(spaceFound = false)) {
                    ::spaceFound(temp, tag, id, classes, attributes, pseudoclasses, scope,
                                 relation, rels, descendants, begun);
                }
                temp = *it++;
                break;
                //Whitespace
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '\v':
            case '\f':
                spaceFound = true;
                it++;
                break;
                //We're going to pretend there was a space to hijack the check to complete tokens
            case '>':
                relation = CSSRelation::IMMEDIATE_DESCENDANT;
                spaceFound = true;
                it++;
                break;
            case '+':
                relation = CSSRelation::IMMEDIATE_SIBLING;
                spaceFound = true;
                it++;
                break;
            case '~':
                relation = CSSRelation::SIBLING;
                spaceFound = true;
                it++;
                break;
        }
    }
    if (!temp.empty()) handleTemp(std::move(temp), tag, id, classes, pseudoclasses, scope);
    if (begun) {
        if (!static_cast<int>(relation)) {
            descendants.emplace_back(std::move(rels));
            rels = feather::Vector<CSSToken>();
        }
    }
    rels.emplace_back(std::move(tag), std::move(id), std::move(classes), std::move(attributes),
                      std::move(pseudoclasses), relation);
    descendants.emplace_back(std::move(rels));
    return CSSSelector(std::move(descendants));
}

feather::Vector<feather::dom::selector::CSSSelector> CSSSelector::parseDelegateList(
        feather::DOMString::const_iterator begin,
        feather::DOMString::const_iterator end,
        feather::StrongPointer<const feather::dom::Element> scope) {
    Deque <UInt> commas;
    UInt start = 0;
    UInt open = 1;
    bool unquote = true;
    bool undquote = true;
    while (open && begin + ++start != end) {
        switch (*(begin + start)) {
            case ',':
                if (unquote && undquote && open == 1) commas.push_back(start);
                break;
            case '(':
                if (unquote && undquote) open++;
                break;
            case ')':
                if (unquote && undquote) open--;
                break;
            case '"':
                if (unquote) undquote ^= true;
                break;
            case '\'':
                if (undquote) unquote ^= true;
                break;
            default:
                break;
        }
    }
    Vector <CSSSelector> selectors;
    selectors.reserve(commas.size() + 1);
    start = 0;
    for (auto i : commas) {
        selectors.push_back(parseDelegate(start + begin, i + begin, scope));
        start = i + 1;
    }
    selectors.push_back(parseDelegate(start + begin, end, scope));
    return selectors;
}