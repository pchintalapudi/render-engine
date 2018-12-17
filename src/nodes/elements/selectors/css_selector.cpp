//
// Created by prem on 12/15/2018.
//

#include "nodes/elements/selectors/css_selector.h"
#include "nodes/elements/element.h"
#include "nodes/documents/document.h"
#include "nodes/documents/shadow_root.h"

using namespace feather::dom::selector;

namespace {
    feather::DOMString toLowercase(feather::DOMString s) {
        feather::DOMString str;
        str.reserve(s.length());
        for (auto c : s) str += static_cast<char>(std::tolower(c));
        return str;
    }
}

CSSAttributeSelector::CSSAttributeSelector(feather::DOMString attr, feather::DOMString val,
                                           feather::dom::selector::CSSAttributeType type,
                                           feather::TriValue caseSensitive)
        : attr(attr), type(type), caseSensitive(caseSensitive),
          val(caseSensitive == TriValue::NO ? toLowercase(val) : val) {}

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

bool CSSAttributeSelector::matches(const feather::StrongPointer<const feather::dom::Element> element) const {
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
    //Lots and lots of pseudoclasses yay! (not :( )

    class ActivePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":active"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> element) const override {
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

        bool matches(const feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isAnyLink(element);
        }

    };

    class BlankPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":blank"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isBlank(element);
        }
    };

    class CheckedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":checked"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isChecked(element);
        }
    };

    class DefaultPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":default"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDefault(element);
        }
    };

    class DefinedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":defined"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDefined(element);
        }
    };

    class DirPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit DirPseudoclass(bool ltr) : ltr(ltr) {}

        feather::DOMString toString() const override { return ":dir(" + feather::DOMString(ltr ? "ltr" : "rtl") + ")"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDir(element, ltr);
        }

    private:
        const bool ltr;
    };

    class DisabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":disabled"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDisabled(element);
        }
    };

    class EmptyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":empty"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isEmpty(element);
        }
    };

    class EnabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":enabled"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isEnabled(element);
        }
    };

    class FirstChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":first-child"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isFirstChild(element);
        }
    };

    class FirstOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit FirstOfTypePseudoclass(feather::DOMString type) : type(type) {}

        feather::DOMString toString() const override { return ":first-of-type"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isFirstOfType(element, type);
        }

    private:
        const feather::DOMString type;
    };

    class FullscreenPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":fullscreen"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isFullscreen(element);
        }
    };

    class FocusPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":focus"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
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

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
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

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
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

        explicit HasPseudoclass(const feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::Vector<feather::DOMString> temp;
            feather::DOMString concat;
            feather::UInt reserve = 5;
            for (auto sel : selectors) {
                auto str = sel.toString();
                reserve += str.length() + 1;
                temp.push_back(str);
            }
            concat.reserve(reserve);
            concat += ":has(";
            for (auto str : temp) concat += str + ",";
            concat[concat.length() - 1] = ')';
            return concat;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (element) for (auto selector : selectors) if (element->querySelectorInternal(selector)) return true;
            return false;
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class HostPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":host"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isHost(element);
        }
    };

    class HostManagedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostManagedPseudoclass(const feather::Vector<CSSSelector> selectors) : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 6;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (auto sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string;
            string.reserve(reserve);
            string += ":host(";
            for (auto str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (auto sel : selectors) if (sel.matches(element->getParentElement())) return true;
            }
            return false;
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class HostContextPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostContextPseudoclass(const feather::Vector<CSSSelector> selectors) : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 6;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (auto sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string;
            string.reserve(reserve);
            string += ":host(";
            for (auto str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (auto sel : selectors) {
                    auto parent = element->getParentElement();
                    while (parent && !sel.matches(parent)) parent = parent->getParentElement();
                    if (parent) return true;
                }
            }
            return false;
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class HoverPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
        feather::DOMString toString() const override { return ":hover"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
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

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isIndeterminate(element);
        }
    };

    class InRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":in-range"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isInRange(element);
        }
    };

    class InvalidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":invalid"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isInvalid(element);
        }
    };

    class IsPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit IsPseudoclass(const feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 4;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (auto sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string = ":is(";
            string.reserve(reserve);
            for (auto str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (element) for (auto sel : selectors) if (sel.matches(element)) return true;
            return false;
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class LangPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LangPseudoclass(feather::DOMString lang = "en-US") : lang(lang) {}

        feather::DOMString toString() const override {
            return ":lang(" + lang + ")";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLang(element, lang);
        }

    private:
        const feather::DOMString lang;
    };

    class LastChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override {
            return ":last-child";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLastChild(element);
        }
    };

    class LastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LastOfTypePseudoclass(feather::DOMString type) : type(type) {}

        feather::DOMString toString() const override {
            return ":last-of-type";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLastOfType(element, type);
        }

    private:
        const feather::DOMString type;
    };

    class LinkPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override {
            return ":link";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLink(element);
        }
    };

    class NotPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit NotPseudoclass(const feather::Vector<feather::dom::selector::CSSSelector> selectors)
                : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 5;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (auto sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string = ":not(";
            string.reserve(reserve);
            for (auto str : temp) (string += str) + ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (!element) return false;
            for (auto sel : selectors) if (sel.matches(element)) return false;
            return true;
        }

    private:
        const feather::Vector<feather::dom::selector::CSSSelector> selectors;
    };

    class NthChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
        NthChildPseudoclass(feather::Long a, feather::Long b) : a(a), b(b) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-child(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-child(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-child(n + " + std::to_string(b) + ")"
                                      : ":nth-child(n)" : ("nth-child(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthChild(element, a, b);
        }

    private:
        const feather::Long a, b;
    };

    class NthOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString type) : a(a), b(b), type(type) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-of-type(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-of-type(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-of-type(n + " + std::to_string(b) + ")"
                                      : ":nth-of-type(n)" : ("nth-of-type(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthOfType(element, a, b, type);
        }

    private:
        const feather::Long a, b;
        const feather::DOMString type;
    };

    class NthLastChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        NthLastChildPseudoclass(feather::Long a, feather::Long b) : a(a), b(b) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-last-child(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-last-child(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-last-child(n + " + std::to_string(b) + ")"
                                      : ":nth-last-child(n)" : ("nth-last-child(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthLastChild(element, a, b);
        }

    private:
        const feather::Long a, b;
    };

    class NthLastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthLastOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString type) : a(a), b(b), type(type) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-last-of-type(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-last-of-type(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-last-of-type(n + " + std::to_string(b) + ")"
                                      : ":nth-last-of-type(n)" : ("nth-last-of-type(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthLastOfType(element, a, b, type);
        }

    private:
        const feather::Long a, b;
        const feather::DOMString type;
    };

    class OnlyChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":only-child"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOnlyChild(element);
        }
    };

    class OnlyOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        OnlyOfTypePseudoclass(feather::DOMString type) : type(type) {}

        feather::DOMString toString() const override { return ":only-of-type"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOnlyOfType(element, type);
        }

    private:
        const feather::DOMString type;
    };

    class OptionalPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":optional"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOptional(element);
        }
    };

    class OutOfRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":out-of-range"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOutOfRange(element);
        }
    };

    class PlaceholderShownPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":placeholder-shown"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isPlaceholderShown(element);
        }
    };

    class ReadOnlyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-only"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isReadOnly(element);
        }
    };

    class ReadWritePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-write"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isReadWrite(element);
        }
    };

    class RequiredPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":required"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isRequired(element);
        }
    };

    class RootPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":root"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isRoot(element);
        }
    };

    class ScopePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit ScopePseudoclass(feather::StrongPointer<const feather::dom::Element> scope) : scope(scope) {}

        feather::DOMString toString() const override { return ":scope"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isScope(element, scope);
        }

    private:
        const feather::StrongPointer<const feather::dom::Element> scope;
    };

    class TargetPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":target"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isTarget(element);
        }
    };

    class ValidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":valid"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isValid(element);
        }
    };

    class VisitedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":visited"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isVisited(element);
        }
    };
}

feather::DOMString CSSToken::toString() const {
    UInt reserve = 0;
    for (auto cls : classes) reserve += cls.length();
    DOMString clazzes;
    clazzes.reserve(reserve);
    for (auto cls : classes) clazzes += cls;
    reserve = 0;
    Vector <DOMString> temp;
    temp.reserve(attributes.size());
    for (auto attr : attributes) {
        auto str = attr.toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString attrs;
    attrs.reserve(reserve);
    for (auto str : temp) attrs += str;
    reserve = 0;
    temp.clear();
    for (auto pclass : pseudoclasses) {
        auto str = pclass->toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString pseudoclasses;
    pseudoclasses.reserve(reserve);
    for (auto str : temp) pseudoclasses += str;
    return (tagName.empty() ? "*" : tagName) + (id.empty() ? "" : ("#" + id)) + attrs + pseudoclasses;
}

bool CSSToken::matches(feather::StrongPointer<const feather::dom::Element> element) const {
    if (!tagName.empty() && element->getTagName() != tagName) return false;
    if (!id.empty() && element->getId() != id) return false;
    auto classList = element->getClassList();
    for (auto cls : classes) if (!classList->contains(cls)) return false;
    for (auto attr : attributes) if (!attr.matches(element)) return false;
    for (auto pclass : pseudoclasses) if (!pclass->matches(element)) return false;
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

bool CSSSelector::matches(feather::StrongPointer<const feather::dom::Element> element) const {
    if (!end.matches(element)) return false;
    feather::StrongPointer<const feather::dom::Element> next = element;
    for (auto pair : extra) {
        if (!next) return false;
        switch (pair.first) {
            case CSSRelation::DESCENDANT:
                next = next->getParentElement();
                while (next && !pair.second.matches(next)) next = next->getParentElement();
                if (!next) return false;
                break;
            case CSSRelation::SIBLING:
                next = next->getPreviousElementSibling();
                while (next && !pair.second.matches(next)) next = next->getPreviousElementSibling();
                if (!next) return false;
                break;
            case CSSRelation::IMMEDIATE_SIBLING:
                if (!pair.second.matches(element->getPreviousElementSibling())) return false;
                break;
            case CSSRelation::IMMEDIATE_DESCENDANT:
                if (!pair.second.matches(element->getParentElement())) return false;
                break;
        }
    }
    return true;
}

feather::DOMString CSSSelector::toString() const {
    Vector <DOMString> temp;
    UInt reserve = 0;
    temp.reserve(extra.size() * 2 + 1);
    for (const auto &pair : extra) {
        auto token = pair.second.toString();
        auto rel = ::toString(pair.first);
        temp.push_back(token);
        temp.push_back(rel);
        reserve += token.length() + rel.length();
    }
    auto token = end.toString();
    reserve += token.length();
    temp.push_back(token);
    DOMString selector;
    selector.reserve(reserve);
    for (auto str : temp) selector += str;
    return selector;
}

namespace {

    //Taken from stackoverflow: https://stackoverflow.com/a/2112111
    constexpr unsigned int hasher(const char *input) {
        return *input ? static_cast<unsigned int>(*input) + 33 * hasher(input + 1) : 5381;
    }

    feather::dom::selector::CSSPseudoclassSelector *getPseudoclass(feather::DOMString psel, feather::DOMString tag,
                                                                   feather::StrongPointer<const feather::dom::Element>
                                                                   scope) {
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
                    feather::StrongPointer<const feather::dom::Element> scope) {
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
                                             tag, scope);
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
}

feather::dom::selector::CSSSelector
CSSSelector::parseDelegate(feather::DOMString::iterator begin, feather::DOMString::iterator end,
                           feather::StrongPointer<const feather::dom::Element> scope) {
    DOMString temp;
    temp.reserve(end - begin);
    DOMString tag;
    DOMString id;
    Vector <DOMString> classes;
    Vector < CSSPseudoclassSelector * > pseudoclasses;
    Vector <CSSAttributeSelector> attributes;
    CSSToken last = CSSToken(tag, id, classes, attributes, pseudoclasses);
    Vector <Pair<CSSRelation, CSSToken>> rels;
    bool spaceFound = false;
    CSSRelation relation = CSSRelation::DESCENDANT;
    auto it = begin;
    while (isspace(*++it));
    it--;
    while (isspace(*end--));
    end++;
    while (it != end) {
        switch (*it) {
            default:
                if (spaceFound) {
                    if (!temp.empty()) {
                        handleTemp(temp, tag, id, classes, pseudoclasses, scope);
                        temp = "";
                    }
                }
                temp += *it++;
                break;
            case '[':
                //Do attr parsing
            {
                if (!temp.empty()) {
                    handleTemp(temp, tag, id, classes, pseudoclasses, scope);
                    temp = "";
                }
                if (spaceFound) {
                    //Do something
                }
                DOMString attr;
                while (isspace(*++it));
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
                    DOMString val;
                    while (quoted ? *++it != '"' && *it != '\'' : ((!isspace(*++it) && *it != ']'))) val += *it;
                    TriValue caseSensitive = TriValue::AUTO;
                    if (*it != ']') {
                        while (isspace(*++it) || *it == '\'' || *it == '"');
                        switch (tolower(*it)) {
                            case 'i':
                                caseSensitive = TriValue::NO;
                                break;
                            case 's':
                                caseSensitive = TriValue::YES;
                                break;
                        }
                        while (*it++ != ']');
                    } else it++;
                    attributes.push_back(CSSAttributeSelector(attr, val, type, caseSensitive));
                } else {
                    TriValue caseSensitive = TriValue::AUTO;
                    //Rarely do people actually stick these in css; im willing to take the cost
                    if (tolower(attr.back()) == 'i' || tolower(attr.back()) == 's') {
                        auto offset = 0;
                        while (isspace(*(it - ++offset)));
                        if (isspace(*(it - offset - 1)))
                            caseSensitive = tolower(attr.back() == 'i') ? TriValue::NO : TriValue::YES;
                    }
                    it++;
                    attributes.push_back(CSSAttributeSelector(attr, "", CSSAttributeType::PRESENT, caseSensitive));
                }
                break;
            }
            case ']':
                //Error: fail me silently
                break;
            case '(':
                //Do special pseudoclass parsing
                switch (hasher(temp.c_str())) {
                    case hasher(":dir"): {
                        DOMString val;
                        while (*++it != ')') if (!isspace(*it)) val += *it;
                        it++;
                        if (val == "rtl") pseudoclasses.push_back(new DirPseudoclass(false));
                        else if (val == "ltr") pseudoclasses.push_back(new DirPseudoclass(true));
                        break;
                    }
                    case hasher(":has"): {
                        auto start = it - begin;
                        while (*++it != ')');
                        auto selectors = parseDelegateList(begin + start, it, scope);
                        pseudoclasses.push_back(new HasPseudoclass(selectors));
                        break;
                    }
                    case hasher(":host"): {
                        auto start = it - begin;
                        while (*++it != ')');
                        auto selectors = parseDelegateList(begin + start, it, scope);
                        pseudoclasses.push_back(new HostManagedPseudoclass(selectors));
                        break;
                    }
                    case hasher(":host-context"): {
                        auto start = it - begin;
                        while (*++it != ')');
                        auto selectors = parseDelegateList(begin + start, it, scope);
                        pseudoclasses.push_back(new HostContextPseudoclass(selectors));
                        break;
                    }
                }
                break;
            case ')':
                //Error: fail me silently
                break;
            case '*':
            case ':':
            case '#':
                if (!temp.empty() && *it != ':' && temp != ":") {
                    handleTemp(temp, tag, id, classes, pseudoclasses, scope);
                }
                if (spaceFound) {
                    //Do something
                }
                temp = *it;
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
}

feather::Vector<feather::dom::selector::CSSSelector> CSSSelector::parseDelegateList(
        feather::DOMString::iterator begin,
        feather::DOMString::iterator end,
        feather::StrongPointer<const feather::dom::Element> scope) {
    Deque <UInt> commas;
    auto start = 0;
    while (begin + ++start != end) if (*(begin + start) == ',') commas.push_back(start);
    Vector <CSSSelector> selectors;
    selectors.reserve(commas.size() + 1);
    for (auto i : commas) {
        selectors.push_back(parseDelegate(start + begin, i + begin, scope));
        start = i;
    }
    selectors.push_back(parseDelegate(start + begin, end, scope));
    return selectors;
}