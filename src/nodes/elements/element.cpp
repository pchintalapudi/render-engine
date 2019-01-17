//
// Created by prem on 12/13/2018.
//

#include "nodes/elements/element.h"
#include "nodes/documents/shadow_root.h"
#include "sstream"

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
        feather::DOMString tagName{};
    };

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
        feather::DOMString className{};
    };
}

class feather::dom::elists::FilteredByTagName
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, TagNameFilter> {
public:
    FilteredByTagName(StrongPointer<const Element> element, DOMString filter)
            : RiskyFilteredList(std::move(element), TagNameFilter(std::move(filter))) {
        element->bind(std::static_pointer_cast<Invalidatable>(shared_from_this()));
    }

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) const override {
        if (s.contains(observable::InvEvent::CHILDREN_CHANGE)) s += observable::InvEvent::INVALIDATE_THIS;
        else s -= observable::InvEvent::INVALIDATE_THIS;
    }
};

class feather::dom::elists::FilteredByTagNameNS
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, TagNameFilter> {
public:
    FilteredByTagNameNS(StrongPointer<const Element> element, const DOMString &ns, const DOMString &name)
            : RiskyFilteredList(std::move(element), TagNameFilter(ns + name)) {
        element->bind(std::static_pointer_cast<Invalidatable>(shared_from_this()));
    }

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) const override {
        if (s.contains(observable::InvEvent::CHILDREN_CHANGE)) s += observable::InvEvent::INVALIDATE_THIS;
        else s -= observable::InvEvent::INVALIDATE_THIS;
    }
};

class feather::dom::elists::FilteredByClassName
        : observable::RiskyFilteredList<StrongPointer<Element>, Element, ClassNameFilter> {
public:
    FilteredByClassName(StrongPointer<const Element> element, DOMString className)
            : RiskyFilteredList(std::move(element), ClassNameFilter(std::move(className))) {
        element->bind(std::static_pointer_cast<Invalidatable>(shared_from_this()));
    }

protected:
    void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *) const override {
        if (s.contains(observable::InvEvent::CHILDREN_CHANGE) || s.contains(observable::InvEvent::CLASS_CHANGE))
            s += observable::InvEvent::INVALIDATE_THIS;
        else s -= observable::InvEvent::INVALIDATE_THIS;
    }
};

namespace {
    const feather::Map<feather::DOMString, feather::UShort> knowns = {
            {"a",        0},
            {"area",     1},
            {"audio",    2},
            {"br",       3},
            {"base",     4},
            {"basefont", 5},
            {"body",     6},
            {"button",   7},
            {"canvas",   8},
            {"content",  9},
            {"custom",   10},
            {"dlist",    11},
            {"data",     12},
            {"datalist", 13},
            {"dialog",   14},
            {"div",      15},
            {"document", 16},
            {"embed",    17},
            {"fieldset", 18},
            {"form",     19},
            {"frameset", 20},
            {"hr",       21},
            {"head",     22},
            {"heading",  23},
            {"html",     24},
            {"iframe",   25},
            {"img",      26},
            {"input",    27},
            {"isindex",  28},
            {"keygen",   29},
            {"li",       30},
            {"label",    31},
            {"legend",   32},
            {"link",     33},
            {"map",      34},
            {"media",    35},
            {"meta",     36},
            {"meter",    37},
            {"mod",      38},
            {"ol",       39},
            {"object",   40},
            {"optgroup", 41},
            {"option",   42},
            {"output",   43},
            {"p",        44},
            {"param",    45},
            {"picture",  46},
            {"pre",      47},
            {"progress", 48},
            {"quote",    49},
            {"script",   50},
            {"select",   51},
            {"shadow",   52},
            {"source",   53},
            {"span",     54},
            {"style",    55},
            {"caption",  56},
            {"td",       57},
            {"col",      58},
            {"table",    59},
            {"th",       60},
            {"tr",       61},
            {"thead",    62},
            {"tbody",    63},
            {"tfoot",    64},
            {"template", 65},
            {"textarea", 66},
            {"time",     67},
            {"title",    68},
            {"track",    69},
            {"ul",       70},
            {"*",        71},
            {"video",    72}
    };
}

feather::DOMString feather::dom::encodeType(const feather::DOMString &input) {
    auto idx = knowns.find(input);
    return idx == knowns.end() ? input : std::to_string(idx->second);
}

KnownElements feather::dom::getType(const feather::DOMString &encoded) {
    std::stringstream stream;
    stream << encoded;
    UShort s = ~0u;
    stream >> s;
    return stream && ~s ? static_cast<KnownElements>(s) : KnownElements::HTMLCustomElement;
}

Element::Element(feather::DOMString baseURI, feather::DOMString tagName,
                 const feather::StrongPointer<feather::dom::Node> &parent, KnownElements type)
        : Node(std::move(baseURI), std::move(tagName), NodeType::ELEMENT_NODE, StrongPointer<DOMString>(), parent),
          Slotable(), type(type), children(getChildNodes()) {
    bind(getAttributes());
    bind(getClassList());
}

feather::StrongPointer<feather::DOMString> Element::getAttribute(const feather::DOMString &name) const {
    auto attr = attributes.getNamedItem(name);
    return attr ? std::make_shared<DOMString>(attr->getValue()) : StrongPointer<DOMString>();
}

feather::StrongPointer<Element> Element::getClosest(const feather::DOMString &selector) const {
    StrongPointer <Element> ref = getThisRef();
    auto sel = selector::CSSSelector::parse(selector, ref);
    while (!sel.matches(ref) && (ref = ref->getParentElement()));
    return ref;
}

feather::Vector<DOMRect> Element::getClientRects() const {
    //TODO: implement method
    return {};
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
    innerHTML.clear();
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
    innerHTML.reserve(reserve);
    for (UInt i = 0; i < getChildNodes()->size(); i++) {
        auto child = getChildNodes()->get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
                //It's been computed before, so this is actually a very cheap call
                innerHTML += std::static_pointer_cast<Element>(child)->getInnerHtml();
                break;
            case NodeType::TEXT_NODE:
                innerHTML += encode(*child->getNodeValue());
                break;
            case NodeType::COMMENT_NODE:
                innerHTML += "<!--" + *child->getNodeValue() + "-->";
                break;
            default:
                //Idc about the other random nodes; they dont really matter
                break;
        }
    }
    return innerHTML;
}

feather::DOMString Element::cacheOuterHTML() const {
    outerHTML.clear();
    UInt reserve = 0;
    DOMString attrs = attributes.toHTML();
    DOMString inner = getInnerHtml();
    reserve += 1 + getTagName().length() + attrs.length() + 1 + inner.length() + 2 + getTagName().length() + 1;
    outerHTML.reserve(reserve);
    outerHTML += "<";
    outerHTML += getTagName();
    outerHTML += attrs;
    outerHTML += ">";
    outerHTML += inner;
    outerHTML += "</";
    outerHTML += getTagName();
    outerHTML += ">";
    outerHTMLValid = true;
    return outerHTML;
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
        for (UInt i = 0, ri = children->size(); ri--; i++) children->get(i)->indeces.set(std::make_pair(i, ri));
    } else if (getParentNode()) {
        Vector <StrongPointer<Element>> v;
        auto children = getParentNode()->getChildNodes();
        for (UInt i = 0; i < children->size(); i++) {
            auto child = children->get(i);
            if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE)
                v.push_back(std::static_pointer_cast<Element>(child));
        }
        for (UInt i = 0, ri = children->size(); ri--; i++) v[i]->indeces.set(std::make_pair(i, ri));
    } else {
        indeces.set(std::make_pair(0, 0));
    }
}

void Element::updatedTypedIndeces() const {
    if (getParentElement()) {
        auto children = getParentElement()->getChildren();
        Multimap <DOMString, StrongPointer<Element>> m;
        for (UInt i = 0, ri = children->size(); ri--; i++) {
            auto child = children->get(i);
            child->indeces.set(std::make_pair(i, ri));
            m.emplace(child->getTagName(), child);
        }
        Vector <StrongPointer<Element>> v;
        DOMString s;
        for (const auto &pair : m) {
            if (s != pair.first) {
                for (UInt i = 0, ri = v.size(); ri--; i++) v[i]->typedIndeces.set(std::make_pair(i, ri));
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
                    p.second->typedIndeces.set(std::make_pair(i, ri));
                    p.second->indeces.set(std::make_pair(p.first, size - p.first));
                }
            }
        }
    } else {
        indeces.set(std::make_pair(0, 0));
        typedIndeces.set(std::make_pair(0, 0));
    }
}

feather::UInt Element::getElementIndex() const {
    if (!indeces.isValid()) updateElementIndeces();
    return indeces.get().first;
}

feather::UInt Element::getLastElementIndex() const {
    if (!indeces.isValid()) updateElementIndeces();
    return indeces.get().second;
}

feather::UInt Element::getTypedElementIndex() const {
    if (!typedIndeces.isValid()) updatedTypedIndeces();
    return typedIndeces.get().first;
}

feather::UInt Element::getLastTypedElementIndex() const {
    if (!typedIndeces.isValid()) updatedTypedIndeces();
    return typedIndeces.get().second;
}

feather::StrongPointer<feather::DOMString>
Element::getAttributeNS(feather::DOMString ns, feather::DOMString name) const {
    return getAttribute(ns + std::move(name));
}

feather::StrongPointer<elists::FilteredByClassName> Element::getElementsByClassName(DOMString className) const {
    return std::make_shared<elists::FilteredByClassName>(std::static_pointer_cast<const Element>(shared_from_this()),
                                                         std::move(className));
}

feather::StrongPointer<elists::FilteredByTagName> Element::getElementsByTagName(feather::DOMString tagName) const {
    return std::make_shared<elists::FilteredByTagName>(std::static_pointer_cast<const Element>(shared_from_this()),
                                                       std::move(tagName));
}

feather::StrongPointer<elists::FilteredByTagNameNS> Element::getElementsByTagNameNS(feather::DOMString ns,
                                                                                    feather::DOMString tagName) const {
    return std::make_shared<elists::FilteredByTagNameNS>(std::static_pointer_cast<const Element>(shared_from_this()),
                                                         std::move(ns), std::move(tagName));
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
            getAttributes()->setNamedItem(
                    std::make_shared<ClassAttr>(getThisRef(), getClassList(), std::move(value)));
            break;
        }
        default: {
            auto attr = std::make_shared<StandardAttr>(name, getThisRef());
        }
    }
}

bool Element::isEqualNode(const feather::StrongPointer<const feather::dom::Node> &other) const {
    if (other && other->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
        auto eOther = std::static_pointer_cast<const Element>(other);
        if (eOther->getElementType() == getElementType() &&
            (getElementType() != KnownElements::HTMLCustomElement
             || eOther->getTagName() == getTagName()) &&
            *eOther->getAttributes() == *getAttributes()) {
            return *getChildNodes() == *other->getChildNodes();
        }
    }
    return false;
}

feather::StrongPointer<Attr> ClassAttr::clone(const feather::StrongPointer<feather::dom::Element> &element) const {
    return std::make_shared<ClassAttr>(element, element->getClassList(), getValue());
}

feather::StrongPointer<Node> Element::cloneNode(bool deep) const {
    auto clone = virtualClone();
    auto map = getAttributes();
    auto cmap = clone->getAttributes();
    auto backing = *(map->getBacking()), cbacking = *(cmap->getBacking());
    auto order = *(map->getInsertionOrder()), corder = *(cmap->getInsertionOrder());
    for (const auto &pair : backing) cbacking[pair.first] = pair.second->clone(clone);
    corder.insert(corder.end(), order.begin(), order.end());
    if (deep) {
        auto children = getChildNodes();
        auto cchildren = clone->getChildNodes();
        cchildren->reserve(children->size());
        for (UInt i = 0; i < children->size(); cchildren->add(children->get(i++)->cloneNode(true)));
    }
    return clone;
}

bool feather::dom::compareType(const feather::StrongPointer<const feather::dom::Element> &e,
                               const feather::DOMString &type) {
    auto etype = getType(type);
    return etype == KnownElements::HTMLCustomElement
           ? type == e->getTagName() : etype == e->getElementType();
}