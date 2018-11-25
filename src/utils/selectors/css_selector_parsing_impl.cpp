//
// Created by prem on 11/24/2018.
//
#include <sstream>
#include "include/utils/selectors/css_selector_parsing_impl.h"
#include "include/nodes/element.h"
#include "include/nodes/document.h"

struct NthSel {
    NthSel() : A(0), b(0) {}

    NthSel(long A, long b) : A(A), b(b) {}

    long A;
    long b;
};
namespace {
    const NthSel odd = NthSel(2, 1);
    const NthSel even = NthSel(2, 0);
}

css::CSSSelector getSubselector(long long *i, long long j, DOMString selector) {
    long long k = *i + 1, openCount = 1;
    std::stringstream output;
    for (; k < j; output << selector[k++]) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                if (!openCount) {
                    goto closed;
                }
        }
    }
    closed:
    *i = k;
    return css::parse(output.str());
}

std::vector<css::CSSSelector>
getSubselectors(long long *i, long long j, DOMString selector) {
    long long k = *i + 1, openCount = 1;
    for (; k < j && openCount; k++) {
        switch (selector[k]) {
            case '(':
                openCount++;
                break;
            case ')':
                openCount--;
                break;
        }
    }
    *i = k;
    return css::parseList(selector.substr(*i + 1, k - *i + 2));
}

const NthSel parseFormula(long long *i, long long j, DOMString selector) {
    long long k = *i + 1;
    NthSel ret;
    std::stringstream stream;
    while (k < j && selector[k] != 'n' && selector[k] != ')') if (!isspace(selector[k++])) stream << selector[k - 1];
    if (selector[k] == 'n') {
        if (stream.str() == "eve") return even;
        else stream >> ret.A;
    } else {
        auto str = stream.str();
        if (str == "odd") return odd;
        stream >> ret.b;
        *i = k;
        return ret;
    }
    k++;
    stream.clear();
    while (k < j && selector[k] != ')') if (selector[k] != '+' && !isspace(selector[k++])) stream << selector[k - 1];
    if (!stream.str().empty())
        stream >> ret.b;
    *i = k + 1;
    return ret;
}

enum class AttrSelectorType {
    MATCH, CONTAIN, END, LIST, HYPHEN, BEGIN
};

bool equalsIgnoreCase(DOMString s1, DOMString s2) {
    if (s1.length() != s2.length()) return false;
    for (unsigned long i = 0; i < s1.length(); i++) if (std::tolower(s1[i]) != std::tolower(s2[i])) return false;
    return true;
}

bool containsIgnoreCase(DOMString s1, DOMString s2) {
    std::stringstream stream;
    for (unsigned long i = 0; i < s1.length(); stream << s1[i++]);
    s1 = stream.str();
    stream.clear();
    for (unsigned long i = 0; i < s2.length(); stream << s2[i++]);
    s2 = stream.str();
    return s1.find(s2) != DOMString::npos;
}

bool inList(DOMString list, DOMString val) {
    std::stringstream stream;
    for (unsigned long i = 0; i < list.length(); i++) {
        if (list[i] == ' ') {
            auto str = stream.str();
            if (str == val) return true;
            stream.clear();
        } else {
            stream << list[i];
        }
    }
    return stream.str() == val;
}

bool inListIgnoreCase(DOMString list, DOMString val) {
    std::stringstream stream;
    for (unsigned long i = 0; i < list.length(); i++) {
        if (list[i] == ' ') {
            auto str = stream.str();
            if (equalsIgnoreCase(str, val)) return true;
            stream.clear();
        } else {
            stream << list[i];
        }
    }
    return stream.str() == val;
}

std::pair<std::function<bool(dom::Element *)>, DOMString>
parseAttrSelector(long long *i, unsigned long j, DOMString selector) {
    unsigned long k = *i + 1;
    std::stringstream stream, combined;
    for (; k < j && selector[k] != '=' && selector[k] != ']'; stream << selector[k++]);
    if (selector[k] == '=' && k > 0) {
        AttrSelectorType type;
        DOMString attr;
        switch (selector[k - 1]) {
            case '*':
                type = AttrSelectorType::CONTAIN;
                attr = stream.str().substr(0, k - 1);
                break;
            case '$':
                type = AttrSelectorType::END;
                attr = stream.str().substr(0, k - 1);
                break;
            case '^':
                type = AttrSelectorType::BEGIN;
                attr = stream.str().substr(0, k - 1);
                break;
            case '~':
                type = AttrSelectorType::LIST;
                attr = stream.str().substr(0, k - 1);
                break;
            case '|':
                type = AttrSelectorType::HYPHEN;
                attr = stream.str().substr(0, k - 1);
                break;
            default:
                type = AttrSelectorType::MATCH;
                attr = stream.str().substr(0, k);
        }
        combined << '[' << attr << '=' << '"';
        while (isspace(selector[k++]));
        for (; selector[k] != '"' && selector[k] != ']'; stream << selector[k++]);
        auto val = stream.str();
        combined << val << '"';
        stream.clear();
        while (isspace(selector[k++]));
        k--;
        bool caseSensitive;
        if (caseSensitive = (selector[k] == 'i' || selector[k] == 'I')) combined << ' ' << 'i';
        combined << ']';
        while (selector[k++] != ']');
        *i = k;
        switch (type) {
            case AttrSelectorType::MATCH:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && attribute->getValue() == val;
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && equalsIgnoreCase(attribute->getValue(), val);
                            }, combined.str());
            case AttrSelectorType::CONTAIN:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && attribute->getValue().find(val) != DOMString::npos;
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                return attribute && containsIgnoreCase(attribute->getValue(), val);
                            }, combined.str());
            case AttrSelectorType::END:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       value.substr(value.length() - val.length(), val.length()) == val;
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       equalsIgnoreCase(value.substr(value.length() - val.length(), val.length()), val);
                            }, combined.str());
            case AttrSelectorType::BEGIN:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() && value.substr(0, val.length()) == val;
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return value.length() > val.length() &&
                                       equalsIgnoreCase(value.substr(0, val.length()), val);
                            }, combined.str());
            case AttrSelectorType::HYPHEN:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return (value == val) ||
                                       (value.length() > val.length() &&
                                        (val + "-") == value.substr(0, val.length() + 1));
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute)return false;
                                auto value = attribute->getValue();
                                return equalsIgnoreCase(value, val)
                                       || (value.length() > val.length()
                                           && equalsIgnoreCase(value.substr(0, val.length() + 1), val + "-"));
                            }, combined.str());
            case AttrSelectorType::LIST:
                if (!caseSensitive)
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return inList(value, val);
                            }, combined.str());
                else
                    return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [attr, val](dom::Element *element) {
                                auto attribute = element->getAttributes().getNamedItem(attr);
                                if (!attribute) return false;
                                auto value = attribute->getValue();
                                return inListIgnoreCase(value, val);
                            }, combined.str());
        }
    } else {
        auto attr = stream.str();
        combined << '[' << attr << ']';
        *i = k;
        return std::pair<std::function<bool(dom::Element *)>, DOMString>(
                [attr](dom::Element *element) { return element->getAttributes().getNamedItem(attr) != nullptr; },
                combined.str());
    }
}

template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

bool isNthChild(NthSel &formula, dom::Element *element) {
    if (element->getParentNode()) {
        long long idx = 0;
        if (element->getParentElement()) {
            idx = static_cast<long long>(element->getParentElement()->getChildren().indexOf(element));
        } else {
            for (unsigned long i = 0; i < element->getParentNode()->getChildNodes().size(); i++) {
                auto child = element->getParentNode()->getChildNodes().get(i);
                if (child == element) break;
                if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) idx++;
            }
        }
        idx -= formula.b;
        return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
    } else return formula.b == 0;
}

bool isNthLastChild(NthSel &formula, dom::Element *element) {
    if (element->getParentNode()) {
        long long idx = 0;
        if (element->getParentElement()) {
            idx = static_cast<long long>(element->getParentElement()->getChildren().getLength())
                  - static_cast<long long>(element->getParentElement()->getChildren().indexOf(element));
        } else {
            for (long long i = element->getParentNode()->getChildNodes().size() - 1; i > -1; i--) {
                auto child = element->getParentNode()->getChildNodes().get(i);
                if (child == element) break;
                if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) idx++;
            }
        }
        idx -= formula.b;
        return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
    } else return formula.b == 0;
}

bool isNthOfType(NthSel &formula, DOMString type, dom::Element *element) {
    if (type == "*") return isNthChild(formula, element);
    long long idx = 0;
    if (element->getParentElement()) {
        auto parent = element->getParentElement();
        for (unsigned long i = 0; i < parent->getChildren().getLength(); i++) {
            auto child = parent->getChildren().getItem(i);
            if (child == element) break;
            if (child->getTagName() == type) idx++;
        }
    } else if (element->getParentNode()) {
        auto parent = element->getParentNode();
        for (unsigned long i = 0; i < parent->getChildNodes().size(); i++) {
            auto child = parent->getChildNodes().get(i);
            if (child == element) break;
            if (child->getNodeType() == dom::NodeType::ELEMENT_NODE
                && static_cast<dom::Element *>(child)->getTagName() == type)
                idx++;
        }
    } else return formula.b == 0;
    idx -= formula.b;
    return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
}

bool isNthLastOfType(NthSel &formula, DOMString type, dom::Element *element) {
    if (type == "*") return isNthLastChild(formula, element);
    long long idx = 0;
    if (element->getParentElement()) {
        auto parent = element->getParentElement();
        for (long long i = parent->getChildren().getLength() - 1; i > -1; i--) {
            auto child = parent->getChildren().getItem(i);
            if (child == element) break;
            if (child->getTagName() == type) idx++;
        }
    } else if (element->getParentNode()) {
        auto parent = element->getParentNode();
        for (long long i = parent->getChildNodes().size() - 1; i > -1; i--) {
            auto child = parent->getChildNodes().get(i);
            if (child == element) break;
            if (child->getNodeType() == dom::NodeType::ELEMENT_NODE &&
                static_cast<dom::Element *>(child)->getTagName() == type)
                idx++;
        }
    } else return formula.b == 0;
    idx -= formula.b;
    return (!idx || (sgn(idx) == sgn(formula.A) && !(idx % formula.A)));
}

bool isActive(dom::Element *element) {
    return element->getOwner() ? element->getOwner()->getPseudoclassManager().isActive(element) : false;
}

bool isAnyLink(dom::Element *element) {
    DOMString tag = element->getTagName();
    dom::Attr *attr = element->getAttributes().getNamedItem("href");
    return (tag == "a" || tag == "area" || tag == "link") && (attr && !attr->getValue().empty());
}

bool isFirstChild(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        if (children.get(i) == element) return true;
        else if (children.get(i)->getNodeType() == dom::NodeType::ELEMENT_NODE) break;
    }
    return false;
}

bool isFirstOfType(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        if (children.get(i) == element) return true;
        else if (children.get(i)->getNodeType() == dom::NodeType::ELEMENT_NODE
                 && static_cast<dom::Element *>(children.get(i))->getTagName() == element->getTagName())
            break;
    }
    return false;
}

bool isFocus(dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().isFocused(element);
}

bool containsFocus(dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().hasFocus(element);
}

bool isHover(dom::Element *element) {
    return element->getOwner() && element->getOwner()->getPseudoclassManager().isHover(element);
}

bool isLastChild(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = children.size(); i > 0; i--) {
        auto child = children.get(i - 1);
        if (child == element) return true;
        if (child->getNodeType() == dom::NodeType::ELEMENT_NODE) break;
    }
    return false;
}

bool isLastOfType(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child == element) return true;
        if (child->getNodeType() == dom::NodeType::ELEMENT_NODE
            && static_cast<dom::Element *>(child)->getTagName() == element->getTagName())
            break;
    }
    return false;
}

bool isOnlyChild(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child != element && child->getNodeType() == dom::NodeType::ELEMENT_NODE) return false;
    }
    return true;
}

bool isOnlyOfType(dom::Element *element) {
    if (!element->getParentNode()) return true;
    auto children = element->getParentNode()->getChildNodes();
    for (unsigned long i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child != element && child->getNodeType() == dom::NodeType::ELEMENT_NODE
            && static_cast<dom::Element *>(child)->getTagName() == element->getTagName())
            return false;
    }
    return true;
}

bool isRoot(dom::Element *element) {
    return element->getOwner() && element->getOwner()->getDocumentElement() == element;
}

bool isTarget(dom::Element *element) {
    return false;//TODO: somehow determine current url fragment from the element; maybe it's a property of Document
}

void interpretBuffer(DOMString *tagName, DOMString *id, std::vector<DOMString> &classes,
                     std::vector<std::pair<std::function<bool(dom::Element *)>, DOMString>> &weirdFuncs,
                     DOMString str) {
    if (str.empty()) return;
    switch (str[0]) {
        default:
            *tagName = str;
            break;
        case '#':
            *id = str.substr(1, str.length() - 1);
            break;
        case '.':
            classes.push_back(str.substr(1, str.length() - 1));
            break;
        case ':':
            str = str.length() > 2 ? str[1] == ':' ? str.substr(2, str.length() - 2)
                                                   : str.substr(1, str.length() - 1) : str.substr(1, str.length() - 1);
            if (str == "active") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isActive, ":active"));
            } else if (str == "any-link" || str == "link") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isAnyLink, ":any-link"));
            } else if (str == "first-child") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isFirstChild, ":first-child"));
            } else if (str == "first-of-type") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isFirstOfType, ":first-of-type"));
            } else if (str == "focus") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isFocus, ":focus"));
            } else if (str == "focus-within") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (containsFocus, ":focus-within"));
            } else if (str == "hover") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isHover, ":hover"));
            } else if (str == "last-child") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isLastChild, ":last-child"));
            } else if (str == "last-of-type") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isLastOfType, ":last-of-type"));
            } else if (str == "only-child") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isOnlyChild, ":only-child"));
            } else if (str == "only-of-type") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                             (isOnlyOfType, ":only-of-type"));
            } else if (str == "root") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isRoot, ":root"));
            } else if (str == "target") {
                weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(isTarget, ":target"));
            } else if (str == "")
                break;
    }
}

css::CSSSelector css::parse(DOMString selector) {
    //Plan of action:
    //Iterate over the selector one char at a time
    //If a special character is encountered stop and interpret

    //Set up start and end indeces to skip over arbitrary whitespace
    long long j = selector.length(), i = -1;
    while (j > 0 && isspace(selector[--j]));
    j++;
    while (i < j && isspace(++i));
    //Set up char buffer
    std::stringstream buffer;
    //Set up selector vector
    std::vector<CSSSelectorTokenGroup> selVector;
    //Set up token group relations
    std::vector<std::pair<CSSSelectorToken, CSSSelectorRelation>> relations;
    //Set up token
    DOMString tagName = "*";
    DOMString id;
    std::vector<DOMString> classes;
    std::vector<std::pair<std::function<bool(dom::Element *)>, DOMString>> weirdFuncs;
    //Monitor spaces
    CSSSelectorRelation relation = CSSSelectorRelation::NONE;
    bool spaceFound = false;
    for (; i < j; i++) {
        switch (selector[i]) {
            default:
                if (spaceFound) {
                    if (relation == CSSSelectorRelation::NONE) {
                        //Clear up the entire token group; this is a valid space delimiter
                    }
                    spaceFound = false;
                }
                if (relation != CSSSelectorRelation::NONE) {
                    //We are done with the token; we should clean up the token alone
                }
                buffer << selector[i];
                continue;
            case '(': {
                DOMString prev = buffer.str();
                if (prev == ":dir") {
                } else if (prev == ":host-context") {
                    auto sub = getSubselector(&i, j, selector);
                    auto func = [sub](dom::Element *element) { return false;/*we don't support shadow roots yet*/ };
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                                 (func, ":host-context(" + sub.toString() + ")"));
                } else if (prev == ":is" || prev == ":matches" || prev == ":where") {
                    auto subList = getSubselectors(&i, j, selector);
                    auto func = [subList](dom::Element *element) {
                        for (auto sub : subList) {
                            if (sub.matches(element)) return true;
                        }
                        return false;
                    };
                    std::stringstream output;
                    output << ":is(" << subList[0].toString();
                    for (unsigned long i = 1; i < subList.size(); i++) output << ", " << subList[i].toString();
                    output << ')';
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(func, output.str()));
                } else if (prev == ":lang") {
                } else if (prev == ":not") {
                    auto sub = getSubselector(&i, j, selector);
                    auto func = [sub](dom::Element *element) { return !sub.matches(element); };
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>
                                                 (func, ":not(" + sub.toString() + ")"));
                } else if (prev == ":nth-child") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [&formula](dom::Element *element) { return isNthChild(formula, element); },
                            ":nth-child(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                } else if (prev == ":nth-last-child") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [&formula](dom::Element *element) { return isNthLastChild(formula, element); },
                            ":nth-last-child(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                } else if (prev == ":nth-last-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [&formula, tagName](dom::Element *element) {
                                return isNthLastOfType(formula, tagName, element);
                            },
                            ":nth-last-of-type(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) +
                            ")"));
                } else if (prev == ":nth-of-type") {
                    auto formula = parseFormula(&i, j, selector);
                    weirdFuncs.push_back(std::pair<std::function<bool(dom::Element *)>, DOMString>(
                            [&formula, tagName](dom::Element *element) {
                                return isNthOfType(formula, tagName, element);
                            },
                            ":nth-of-type(" + std::to_string(formula.A) + "n + " + std::to_string(formula.b) + ")"));
                }
                buffer.clear();
                break;
            }
            case ')':
                //There's been an error; the css has failed somehow
                //To try to save the file, we ignore this parentheses and proceed as if none was present.
                break;
            case '\n':
            case '\t':
            case '\r':
            case '\v':
            case '\f':
            case ' ':
                //There's some weird stuff that happens with whitespace; people might whitespace between combinators,
                //so we need to handle that case. That will require significant effort, and will also require much testing.
                spaceFound = true;
                break;
            case ':': {
                DOMString str = buffer.str();
                if (str != ":") {
                    interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                    buffer.clear();
                }
                buffer << ":";
                break;
            }
            case '.': {
                DOMString str = buffer.str();
                interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                buffer.clear();
                buffer << ".";
                break;
            }
            case '#': {
                DOMString str = buffer.str();
                interpretBuffer(&tagName, &id, classes, weirdFuncs, str);
                buffer.clear();
                buffer << "#";
                break;
            }
            case '*': {
                //The only valid place where this can appear is in front of a group of selectors
                //Therefore, since the default value of tagName is a "*", this does not need to be handled.
                break;
            }
            case '[': {
                //Skip ahead to the close bracket; that's where the attribute selector ends
                //We can parse the attribute selector based on that substring between open and close
                weirdFuncs.push_back(parseAttrSelector(&i, j, selector));
            }
            case ']':
                //This is an error; this shouldn't happen
                //We'll silently ignore it to best-effort parse the file
                break;
        }
    }
}

std::vector<css::CSSSelector> css::parseList(DOMString selectorList) {
    std::vector<css::CSSSelector> selectors;
    unsigned long lastI = 0;
    for (unsigned long i = 0; i < selectorList.length(); i++) {
        if (selectorList[i] == ',') {
            selectors.push_back(parse(selectorList.substr(lastI, i + lastI)));
            lastI = i + 1;
        }
    }
    selectors.push_back(parse(selectorList.substr(lastI, selectorList.length() - lastI)));
    return selectors;
}