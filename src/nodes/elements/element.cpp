//
// Created by prem on 12/13/2018.
//

#include "nodes/elements/element.h"

using namespace feather::dom;
namespace {

    class TagNameFilter {
    public:

        explicit TagNameFilter(feather::DOMString tagName) : tagName(std::move(tagName)) {}

        inline void operator()(const feather::StrongPointer<const Element> &p,
                               feather::Vector<feather::StrongPointer<Element>> &addTo) {
            if (p->getTagName() == tagName) addTo.push_back(p->getThisRef());
            auto children = p->getChildren();
            for (feather::UInt i = 0; i < children->size(); (*this)(children->get(i++), addTo));
        }

    private:
        feather::DOMString tagName;
    };
}

class feather::dom::FilteredByTagName
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, TagNameFilter> {
public:
    FilteredByTagName(StrongPointer<const Element> element, TagNameFilter filter)
            : RiskyFilteredList(std::move(element), std::move(filter)) {}

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override {}
};

class feather::dom::FilteredByTagNameNS
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, TagNameFilter> {
public:
    FilteredByTagNameNS(StrongPointer<const Element> element, TagNameFilter filter)
            : RiskyFilteredList(std::move(element), std::move(filter)) {}

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override {}
};

namespace {
    class ClassNameFilter {
    public:
        explicit ClassNameFilter(feather::DOMString className) : className(std::move(className)) {}

        inline void operator()(const feather::StrongPointer<const Element> &p,
                               feather::Vector<feather::StrongPointer<Element>> &addTo) {
            if (p->getClassList()->contains(className)) addTo.push_back(p->getThisRef());
            auto children = p->getChildren();
            for (feather::UInt i = 0; i < children->size(); (*this)(children->get(i++), addTo));
        }

    private:
        feather::DOMString className;
    };
}

class feather::dom::FilteredByClassName
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, ClassNameFilter> {
public:
    FilteredByClassName(StrongPointer<const Element> element, ClassNameFilter filter)
            : RiskyFilteredList(std::move(element), std::move(filter)) {}

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override {

    }
};

feather::StrongPointer<feather::DOMString> Element::getAttribute(feather::DOMString name) const {
    auto attr = attributes->getNamedItem(std::move(name));
    return attr ? std::make_shared<DOMString>(attr->getValue()) : StrongPointer<DOMString>();
}

feather::StrongPointer<Element> Element::getClosest(feather::DOMString selector) const {
    StrongPointer <Element> ref = getThisRef();
    auto sel = selector::CSSSelector::parse(std::move(selector), ref);
    while (!sel.matches(ref) && (ref = ref->getParentElement()));
    return ref;
}

DOMRect Element::getBoundingClientRect() const {
    auto vec = getClientRects();
    DOMRect dr;
    for (auto cr : vec) {
        if (cr.getTop() < dr.getY()) dr.setY(cr.getTop());
        if (cr.getLeft() < dr.getX()) dr.setX(cr.getLeft());
        if (cr.getRight() > dr.getWidth() + dr.getX()) dr.setWidth(cr.getRight() - dr.getX());
        if (cr.getBottom() > dr.getHeight() + dr.getY()) dr.setHeight(cr.getBottom() - dr.getY());
    }
    return vec.empty() || dr.getX() + dr.getY() + dr.getWidth() + dr.getBottom() != 0 ? dr : vec.front();
}

namespace {
    //Taken from stackoverflow: https://stackoverflow.com/a/2112111
    constexpr unsigned int hasher(const char *input) {
        return *input ? static_cast<unsigned int>(*input) + 33 * hasher(input + 1) : 5381;
    }
}

void Element::insertAdjacentElement(feather::DOMString position, feather::StrongPointer<feather::dom::Element> e) {
    switch (hasher(position.c_str())) {
        case hasher("beforebegin"):
            this->insertBefore(e, getSharedFromThis());
            break;
        case hasher("afterbegin"):
            getChildNodes()->insert(0, e);
            break;
        case hasher("beforeend"):
            getChildNodes()->add(e);
            break;
        case hasher("afterend"):
            this->insertAfter(e, getSharedFromThis());
            break;
        default:
            break;
    }
}

namespace {
    feather::DOMString encode(feather::DOMString str) {
        feather::UInt reserve = str.length();
        for (auto c : str) {
            switch (c) {
                case '&':
                    reserve += 4;
                    break;
                case '<':
                case '>':
                    reserve += 3;
                    break;
                default:
                    break;
            }
        }
        feather::DOMString modded;
        modded.reserve(reserve);
        for (auto c : str) {
            switch (c) {
                default:
                    modded += c;
                    break;
                case '&':
                    modded += "&amp;";
                    break;
                case '<':
                    modded += "&lt;";
                    break;
                case '>':
                    modded += "&gt;";
                    break;
            }
        }
        return modded;
    }
}

feather::DOMString Element::cacheInnerHTML() const {
    DOMString html;
    UInt reserve = 0;
    for (UInt i = 0; i < getChildNodes()->size(); i++) {
        auto child = getChildNodes()->get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
                //It's computed here, but the repeated call shouldn't matter since the value is cached.
                reserve += std::static_pointer_cast<Element>(child)->getOuterHtml().length();
                break;
            case NodeType::TEXT_NODE:
                reserve += encode(*child->getNodeValue()).length();
                break;
            case NodeType::COMMENT_NODE:
                reserve += 4 + child->getNodeValue()->length() + 3;
                break;
            default:
                //See below
                break;
        }
    }
    html.reserve(reserve);
    for (UInt i = 0; i < getChildNodes()->size(); i++) {
        auto child = getChildNodes()->get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
                //It's been computed before, so this is actually a very cheap call
                html += std::static_pointer_cast<Element>(child)->getInnerHtml();
                break;
            case NodeType::TEXT_NODE:
                html += encode(*child->getNodeValue());
                break;
            case NodeType::COMMENT_NODE:
                html += "<!--" + *child->getNodeValue() + "-->";
                break;
            default:
                //Idc about the other random nodes; they dont really matter
                break;
        }
    }
    return html;
}

feather::DOMString Element::cacheOuterHTML() const {
    DOMString html;
    UInt reserve = 0;
    DOMString attrs = attributes->toHTML();
    DOMString inner = getInnerHtml();
    reserve += 1 + getTagName().length() + attrs.length() + 1 + inner.length() + 2 + getTagName().length() + 1;
    html.reserve(reserve);
    html += "<";
    html += getTagName();
    html += attrs;
    html += ">";
    html += inner;
    html += "</";
    html += getTagName();
    html += ">";
    innerHTMLValid = true;
    return innerHTML = html;
}

feather::StrongPointer<Element> Element::getNextElementSibling() const {
    auto idx = getElementIndex();
    if (getParentElement()) {
        auto children = getParentElement()->getChildren();
        return idx < children->size() - 1 ? children->get(idx + 1) : StrongPointer<Element>();
    } else if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        while (++idx < children->size() && children->get(idx)->getNodeTypeInternal() != NodeType::ELEMENT_NODE);
        return idx < children->size() ? std::static_pointer_cast<Element>(children->get(idx))
                                      : StrongPointer<Element>();
    } else return StrongPointer<Element>();
}

feather::StrongPointer<Element> Element::getPreviousElementSibling() const {
    auto idx = getElementIndex();
    if (getParentElement()) {
        auto children = getParentElement()->getChildren();
        return idx > 0 ? children->get(idx - 1) : StrongPointer<Element>();
    } else if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        while (idx-- > 0 && children->get(idx)->getNodeTypeInternal() != NodeType::ELEMENT_NODE);
        return idx ? std::static_pointer_cast<Element>(children->get(idx)) : StrongPointer<Element>();
    } else return StrongPointer<Element>();
}

void Element::updateElementIndeces() const {
    if (getParentElement()) {
        auto children = getParentElement()->getChildren();
        for (UInt i = 0, ri = children->size(); ri--; i++) children->get(i)->indeces->set(std::make_pair(i, ri));
    } else if (getParentNode()) {
        Vector <StrongPointer<Element>> v;
        auto children = getParentNode()->getChildNodes();
        for (UInt i = 0; i < children->size(); i++) {
            auto child = children->get(i);
            if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE)
                v.push_back(std::static_pointer_cast<Element>(child));
        }
        for (UInt i = 0, ri = children->size(); ri--; i++) v[i]->indeces->set(std::make_pair(i, ri));
    } else {
        indeces->set(std::make_pair(0, 0));
    }
}

void Element::updatedTypedIndeces() const {
    if (getParentElement()) {
        auto children = getParentElement()->getChildren();
        Multimap <DOMString, StrongPointer<Element>> m;
        for (UInt i = 0, ri = children->size(); ri--; i++) {
            auto child = children->get(i);
            child->indeces->set(std::make_pair(i, ri));
            m.emplace(child->getTagName(), child);
        }
        Vector <StrongPointer<Element>> v;
        DOMString s;
        for (const auto &pair : m) {
            if (s != pair.first) {
                for (UInt i = 0, ri = v.size(); ri--; i++) v[i]->typedIndeces->set(std::make_pair(i, ri));
                v.clear();
                s = pair.first;
            }
            v.push_back(pair.second);
        }
    } else if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        Multimap < DOMString, Pair < UInt, StrongPointer < Element >> > m;
        auto size = 0;
        for (UInt i = 0; i < children->size(); i++) {
            auto child = children->get(i);
            if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
                auto eChild = std::static_pointer_cast<Element>(child);
                m.emplace(eChild->getTagName(), std::make_pair(size++, eChild));
            }
        }
        Vector < Pair < UInt, StrongPointer < Element >> > v;
        DOMString s;
        for (const auto &pair : m) {
            if (s != pair.first) {
                for (UInt i = 0, ri = v.size(); ri--; i++) {
                    Pair <UInt, StrongPointer<Element>> p = v[i];
                    p.second->typedIndeces->set(std::make_pair(i, ri));
                    p.second->indeces->set(std::make_pair(p.first, size - p.first));
                }
            }
        }
    } else {
        indeces->set(std::make_pair(0, 0));
        typedIndeces->set(std::make_pair(0, 0));
    }
}

feather::UInt Element::getElementIndex() const {
    if (!indeces->isValid()) updateElementIndeces();
    return indeces->get().first;
}

feather::UInt Element::getLastElementIndex() const {
    if (!indeces->isValid()) updateElementIndeces();
    return indeces->get().second;
}

feather::UInt Element::getTypedElementIndex() const {
    if (!typedIndeces->isValid()) updatedTypedIndeces();
    return typedIndeces->get().first;
}

feather::UInt Element::getLastTypedElementIndex() const {
    if (!typedIndeces->isValid()) updatedTypedIndeces();
    return typedIndeces->get().second;
}

feather::StrongPointer<feather::DOMString>
Element::getAttributeNS(feather::DOMString ns, feather::DOMString name) const {
    return getAttribute(ns + std::move(name));
}

feather::StrongPointer<FilteredByClassName> Element::getElementsByClassName(DOMString className) const {
    return std::make_shared<FilteredByClassName>(std::static_pointer_cast<const Element>(shared_from_this()),
                                                 ClassNameFilter(std::move(className)));
}

feather::StrongPointer<FilteredByTagName> Element::getElementsByTagName(feather::DOMString tagName) const {
    return std::make_shared<FilteredByTagName>(std::static_pointer_cast<const Element>(shared_from_this()),
                                               TagNameFilter(std::move(tagName)));
}

feather::StrongPointer<FilteredByTagNameNS> Element::getElementsByTagNameNS(feather::DOMString ns,
                                                                            feather::DOMString tagName) const {
    return std::make_shared<FilteredByTagNameNS>(std::static_pointer_cast<const Element>(shared_from_this()),
                                                 TagNameFilter(ns + std::move(tagName)));
}

bool Element::toggleAttribute(feather::DOMString attr) {
    if (getAttributes()->contains(attr)) {
        removeAttribute(std::move(attr));
        return false;
    } else {
        setAttribute(std::move(attr), "");
        return true;
    }
}

bool Element::toggleAttribute(feather::DOMString attr, bool force) {
    if (force) { if (getAttributes()->contains(attr)) { removeAttribute(std::move(attr)); }}
    else if (!getAttributes()->contains(attr)) { setAttribute(std::move(attr), ""); }
    return force;
}

void Element::setAttribute(feather::DOMString name, feather::DOMString value) {
    switch (hasher(name.c_str())) {
        case hasher("class"): {
            getAttributes()->setNamedItem(std::make_shared<ClassAttr>(getThisRef(), classList, std::move(value)));
            break;
        }
        default: {
            auto attr = std::make_shared<StandardAttr>(name, getThisRef());
        }
    }
}