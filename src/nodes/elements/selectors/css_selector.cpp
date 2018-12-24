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
    feather::DOMString toLowercase(feather::DOMString s) {
        feather::DOMString str;
        str.reserve(s.length());
        for (auto c : s) str += static_cast<char>(std::tolower(c));
        return str;
    }
}

CSSAttributeSelector::CSSAttributeSelector(feather::DOMString &&attr, feather::DOMString &&val,
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new ActivePseudoclass(); }
    };

    class AnyLinkPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":any-link"; }

        bool matches(const feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isAnyLink(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new AnyLinkPseudoclass(); }

    };

    class BlankPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":blank"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isBlank(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new BlankPseudoclass(); }
    };

    class CheckedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":checked"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isChecked(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new CheckedPseudoclass(); }
    };

    class DefaultPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":default"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDefault(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new DefaultPseudoclass(); }
    };

    class DefinedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":defined"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDefined(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new DefinedPseudoclass(); }
    };

    class DirPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit DirPseudoclass(bool ltr) : ltr(ltr) {}

        feather::DOMString toString() const override { return ":dir(" + feather::DOMString(ltr ? "ltr" : "rtl") + ")"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDir(element, ltr);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new DirPseudoclass(ltr); }

    private:
        const bool ltr;
    };

    class DisabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":disabled"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isDisabled(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new DisabledPseudoclass(); }
    };

    class EmptyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":empty"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isEmpty(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new EmptyPseudoclass(); }
    };

    class EnabledPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":enabled"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isEnabled(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new EnabledPseudoclass(); }
    };

    class FirstChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":first-child"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isFirstChild(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new FirstChildPseudoclass(); }
    };

    class FirstOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit FirstOfTypePseudoclass(feather::DOMString &&type) : type(type) {}

        feather::DOMString toString() const override { return ":first-of-type"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isFirstOfType(element, type);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new FirstOfTypePseudoclass(feather::DOMString(type));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new FullscreenPseudoclass(); }
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new FocusPseudoclass(); }
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new FocusVisiblePseudoclass(); }
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new FocusWithinPseudoclass(); }
    };

    class HasPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HasPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> &&selectors)
                : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::Vector<feather::DOMString> temp;
            feather::DOMString concat;
            feather::UInt reserve = 5;
            for (const auto &sel : selectors) {
                auto str = sel.toString();
                reserve += str.length() + 1;
                temp.push_back(str);
            }
            concat.reserve(reserve);
            concat += ":has(";
            for (const auto &str : temp) concat += str + ",";
            concat[concat.length() - 1] = ')';
            return concat;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (element) for (const auto &selector : selectors) if (selector.querySelector(element)) return true;
            return false;
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new HasPseudoclass(feather::Vector<CSSSelector>(selectors));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new HostPseudoclass(); }
    };

    class HostManagedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostManagedPseudoclass(const feather::Vector<CSSSelector> &&selectors) : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 6;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (const auto &sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string;
            string.reserve(reserve);
            string += ":host(";
            for (const auto &str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (const auto &sel : selectors) if (sel.matches(element->getParentElement())) return true;
            }
            return false;
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new HostManagedPseudoclass(feather::Vector<CSSSelector>(selectors));
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class HostContextPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit HostContextPseudoclass(feather::Vector<CSSSelector> &&selectors) : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 6;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (const auto &sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string;
            string.reserve(reserve);
            string += ":host(";
            for (const auto &str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (feather::css::PseudoclassManager::isHost(element) && element->getParentElement()) {
                for (const auto &sel : selectors) {
                    auto parent = element->getParentElement();
                    while (parent && !sel.matches(parent)) parent = parent->getParentElement();
                    if (parent) return true;
                }
            }
            return false;
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new HostContextPseudoclass(feather::Vector<CSSSelector>(selectors));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new HoverPseudoclass(); }
    };

    class IndeterminatePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":indeterminate"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isIndeterminate(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *
        clone() const override { return new IndeterminatePseudoclass(); }
    };

    class InRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":in-range"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isInRange(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new InRangePseudoclass(); }
    };

    class InvalidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":invalid"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isInvalid(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new InvalidPseudoclass(); }
    };

    class IsPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit IsPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> &&selectors)
                : selectors(selectors) {}

        feather::DOMString toString() const override {
            feather::UInt reserve = 4;
            feather::Vector<feather::DOMString> temp;
            temp.reserve(selectors.size());
            for (const auto &sel : selectors) {
                auto str = sel.toString();
                temp.push_back(str);
                reserve += str.length() + 1;
            }
            feather::DOMString string = ":is(";
            string.reserve(reserve);
            for (const auto &str : temp) (string += str) += ',';
            string[string.length() - 1] = ')';
            return string;
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (element) for (const auto &sel : selectors) if (sel.matches(element)) return true;
            return false;
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new IsPseudoclass(feather::Vector<CSSSelector>(selectors));
        }

    private:
        const feather::Vector<CSSSelector> selectors;
    };

    class LangPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LangPseudoclass(feather::DOMString &&lang = "en-US") : lang(lang) {}

        feather::DOMString toString() const override {
            return ":lang(" + lang + ")";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLang(element, lang);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new LangPseudoclass(feather::DOMString(lang));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new LastChildPseudoclass(); }
    };

    class LastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit LastOfTypePseudoclass(feather::DOMString &&type) : type(type) {}

        feather::DOMString toString() const override {
            return ":last-of-type";
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isLastOfType(element, type);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new LastOfTypePseudoclass(feather::DOMString(type));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new LinkPseudoclass(); }
    };

    class NotPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit NotPseudoclass(feather::Vector<feather::dom::selector::CSSSelector> &&selectors)
                : selectors(selectors) {}

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

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            if (!element) return false;
            for (const auto &sel : selectors) if (sel.matches(element)) return false;
            return true;
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new NotPseudoclass(feather::Vector<CSSSelector>(selectors));
        }

    private:
        const feather::Vector<feather::dom::selector::CSSSelector> selectors;
    };

    class NthChildPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new NthChildPseudoclass(a, b); }

    private:
        const feather::Long a, b;
    };

    class NthOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString &&type) : a(a), b(b), type(type) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-of-type(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-of-type(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-of-type(n + " + std::to_string(b) + ")"
                                      : ":nth-of-type(n)" : ("nth-of-type(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthOfType(element, a, b, type);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new NthOfTypePseudoclass(a, b, feather::DOMString(type));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new NthLastChildPseudoclass(a, b);
        }

    private:
        const feather::Long a, b;
    };

    class NthLastOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        NthLastOfTypePseudoclass(feather::Long a, feather::Long b, feather::DOMString &&type) : a(a), b(b),
                                                                                                type(type) {}

        feather::DOMString toString() const override {
            return a && a - 1 ? b ? ":nth-last-of-type(" + std::to_string(a) + "n + " + std::to_string(b) + ")"
                                  : ":nth-last-of-type(" + std::to_string(a) + "n)"
                              : a ? b ? ":nth-last-of-type(n + " + std::to_string(b) + ")"
                                      : ":nth-last-of-type(n)" : ("nth-last-of-type(" + std::to_string(b) + ")");
        }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isNthLastOfType(element, a, b, type);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new NthLastOfTypePseudoclass(a, b, feather::DOMString(type));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new OnlyChildPseudoclass(); }
    };

    class OnlyOfTypePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        explicit OnlyOfTypePseudoclass(feather::DOMString &&type) : type(type) {}

        feather::DOMString toString() const override { return ":only-of-type"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOnlyOfType(element, type);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new OnlyOfTypePseudoclass(feather::DOMString(type));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new OptionalPseudoclass(); }
    };

    class OutOfRangePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":out-of-range"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isOutOfRange(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new OutOfRangePseudoclass(); }
    };

    class PlaceholderShownPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":placeholder-shown"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isPlaceholderShown(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *
        clone() const override { return new PlaceholderShownPseudoclass(); }
    };

    class ReadOnlyPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-only"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isReadOnly(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new ReadOnlyPseudoclass(); }
    };

    class ReadWritePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":read-write"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isReadWrite(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new ReadWritePseudoclass(); }
    };

    class RequiredPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":required"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isRequired(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new RequiredPseudoclass(); }
    };

    class RootPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":root"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isRoot(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new RootPseudoclass(); }
    };

    class ScopePseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:

        explicit ScopePseudoclass(feather::StrongPointer<const feather::dom::Element> &&scope) : scope(scope) {}

        feather::DOMString toString() const override { return ":scope"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isScope(element, scope);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override {
            return new ScopePseudoclass(feather::StrongPointer<const feather::dom::Element>(scope));
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

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new TargetPseudoclass(); }
    };

    class ValidPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":valid"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isValid(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new ValidPseudoclass(); }
    };

    class VisitedPseudoclass : public feather::dom::selector::CSSPseudoclassSelector {
    public:
        feather::DOMString toString() const override { return ":visited"; }

        bool matches(feather::StrongPointer<const feather::dom::Element> element) const override {
            return feather::css::PseudoclassManager::isVisited(element);
        }

        feather::dom::selector::CSSPseudoclassSelector *clone() const override { return new VisitedPseudoclass(); }
    };
}

CSSToken::CSSToken(const feather::dom::selector::CSSToken &other) : tagName(other.tagName), id(other.id),
                                                                    classes(other.classes),
                                                                    attributes(other.attributes),
                                                                    pseudoclasses(), relation(other.relation) {
    pseudoclasses.reserve(other.pseudoclasses.size());
    for (auto pclass : other.pseudoclasses) pseudoclasses.push_back(pclass->clone());
}

feather::DOMString CSSToken::toString() const {
    UInt reserve = 0;
    for (const auto &cls : classes) reserve += cls.length() + 1;
    DOMString clazzes;
    clazzes.reserve(reserve);
    for (const auto &cls : classes) clazzes += '.' + cls;
    reserve = 0;
    Vector <DOMString> temp;
    temp.reserve(attributes.size());
    for (const auto &attr : attributes) {
        auto str = attr.toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString attrs;
    attrs.reserve(reserve);
    for (const auto &str : temp) attrs += str;
    reserve = 0;
    temp.clear();
    for (const auto &pclass : pseudoclasses) {
        auto str = pclass->toString();
        temp.push_back(str);
        reserve += str.length();
    }
    DOMString pseudoclasses;
    pseudoclasses.reserve(reserve);
    for (const auto &str : temp) pseudoclasses += str;
    return (tagName.empty() ? "*" : tagName) + (id.empty() ? "" : ("#" + id)) + clazzes + attrs + pseudoclasses;
}

bool CSSToken::matches(feather::StrongPointer<const feather::dom::Element> element) const {
    if (!tagName.empty() && element->getTagName() != tagName) return false;
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
        feather::Vector<feather::dom::selector::CSSToken> &&group)
        : group(group) {
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

bool CSSDescendantToken::matches(feather::StrongPointer<const feather::dom::Element> element) const {
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
        auto children = scope->getChildren();
        for (UInt i = 0; i < children->size(); i++) {
            auto ptr = querySelectorInternal(begin + 1, children->getItem(i));
            if (ptr) return ptr;
        }
    } else {
        auto children = scope->getChildren();
        for (UInt i = 0; i < children->size(); i++) {
            auto ptr = querySelectorInternal(begin, children->getItem(i));
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
            for (UInt i = 0; i < children->size(); querySelectorAllInternal(begin + 1, children->getItem(i++), ref));
        }
    } else {
        auto children = scope->getChildren();
        for (UInt i = 0; i < children->size(); querySelectorAllInternal(begin, children->getItem(i++), ref));
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
        while (*++it != 'n' && *it != ')') if (!isspace(*it)) str += (*it);
        if (*it == 'n') {
            a = str.empty() ? 1 : str == "-" ? -1 : str == "eve" ? 2 : std::stoll(str);
            str = "";
            while (*++it != ')') if (!isspace(*it)) str += (*it);
        }
        if (str == "odd") {
            a = 2;
            b = 1;
        } else {
            b = str.empty() ? 0 : std::stoll(str);
        }
        return std::make_pair(a, b);
    }

    void
    parseSpecialPseudoclass(feather::DOMString::const_iterator &it, const char *temp, const feather::DOMString &tag,
                            feather::Vector<feather::dom::selector::CSSPseudoclassSelector *> &pseudoclasses,
                            const feather::DOMString::const_iterator &begin,
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
                auto start = it - begin;
                while (*++it != ')');
                pseudoclasses.push_back(new HasPseudoclass(CSSSelector::parseDelegateList(begin + start, it, scope)));
                it++;
                break;
            }
            case hasher(":host"): {
                auto start = it - begin;
                while (*++it != ')');
                pseudoclasses.push_back(
                        new HostManagedPseudoclass(CSSSelector::parseDelegateList(begin + start, it, scope)));
                it++;
                break;
            }
            case hasher(":host-context"): {
                auto start = it - begin;
                while (*++it != ')');
                pseudoclasses.push_back(
                        new HostContextPseudoclass(CSSSelector::parseDelegateList(begin + start, it, scope)));
                it++;
                break;
            }
                //Special case for :where b/c specificity doesn't matter
            case hasher(":where"):
            case hasher(":is"): {
                auto start = it - begin;
                while (*++it != ')');
                pseudoclasses.push_back(new IsPseudoclass(CSSSelector::parseDelegateList(begin + start, it, scope)));
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
                auto start = it - begin;
                while (*++it != ')');
                pseudoclasses.push_back(new NotPseudoclass(CSSSelector::parseDelegateList(begin + start, it, scope)));
                it++;
                break;
            }
            case hasher("nth-child"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthChildPseudoclass(pair.first, pair.second));
                it++;
                break;
            }
            case hasher("nth-of-type"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthOfTypePseudoclass(pair.first, pair.second, feather::DOMString(tag)));
                it++;
                break;
            }
            case hasher("nth-last-child"): {
                auto pair = parseNthSel(it);
                pseudoclasses.push_back(new NthLastChildPseudoclass(pair.first, pair.second));
                it++;
                break;
            }
            case hasher("nth-last-of-type"): {
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
    getPseudoclass(feather::DOMString &&psel, feather::DOMString &&tag,
                   feather::StrongPointer<const feather::dom::Element> &&scope) {
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
                pclass = new FirstOfTypePseudoclass(std::move(tag));
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
                pclass = new LastOfTypePseudoclass(std::move(tag));
                break;
            case hasher("link"):
                pclass = new LinkPseudoclass();
                break;
            case hasher("only-child"):
                pclass = new OnlyChildPseudoclass();
                break;
            case hasher("only-of-type"):
                pclass = new OnlyOfTypePseudoclass(std::move(tag));
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
                pclass = new ScopePseudoclass(std::move(scope));
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
    while (it != end) {
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
                ::parseSpecialPseudoclass(it, temp.c_str(), tag, pseudoclasses, begin, scope);
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