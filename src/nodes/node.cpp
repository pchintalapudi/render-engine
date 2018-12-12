//
// Created by prem on 12/11/2018.
//

#include "nodes/node.h"
#include "nodes/elements/element.h"
#include "nodes/text/text.h"

using namespace feather::dom;

namespace {
    const feather::RegularEnumSet<feather::observable::InvEvent> listWatcherSet
            = feather::RegularEnumSet<feather::observable::InvEvent>(
                    1u << static_cast<int>(feather::observable::InvEvent::LIST_CHANGE));
}

Node::Node(feather::DOMString baseURI, feather::DOMString name, feather::dom::NodeType type,
           feather::StrongPointer<feather::DOMString> value, feather::StrongPointer<feather::dom::Node> parent)
        : baseURI(baseURI),
          nextSiblingPtr(
                  std::make_shared<feather::observable::WatchedObservableItem<feather::WeakPointer<feather::dom::Node>>>(
                          listWatcherSet)),
          prevSiblingPtr(
                  std::make_shared<feather::observable::WatchedObservableItem<feather::WeakPointer<feather::dom::Node>>>(
                          listWatcherSet)),
          name(name), type(type), value(value),
          ownerPtr(
                  std::make_shared<feather::observable::WatchedObservableItem<
                          feather::WeakPointer<feather::dom::Document>>>()) {
    parent->bind(ownerPtr);
    childNodes.bind(nextSiblingPtr);
    childNodes.bind(prevSiblingPtr);
}

feather::StrongPointer<Node> Node::getNextSibling() const {
    if (nextSiblingPtr->isValid()) {
        return nextSiblingPtr->get().lock();
    }
    if (getParentNode().get()) {
        auto me = std::static_pointer_cast<const Node>(shared_from_this());
        auto children = getParentNode()->getChildNodes();
        for (UInt i = 0; i < children.size() - 1; i++) {
            if (children.get(i) == me) {
                auto child = children.get(i + 1);
                nextSiblingPtr->set(child);
                return child;
            }
        }
    }
    nextSiblingPtr->set(WeakPointer<Node>());
    return nullptr;
}

feather::StrongPointer<Document> Node::getOwnerDocument() const {
    if (ownerPtr->isValid() && !ownerPtr->get().expired()) return ownerPtr->get().lock();
    if (getParentNode().get()) {
        auto ptr = getParentNode()->getOwnerDocument();
        ownerPtr->set(ptr);
        return ptr;
    }
    ownerPtr->set(WeakPointer<Document>());
    return nullptr;
}

void Node::setParentNode(feather::StrongPointer<feather::dom::Node> parentNode) {
    auto old = getParentNode();
    if (old.get()) {
        old->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
        old->unbindOwner(getOwnerItem());
    }
    parent.set(parentNode);
    if (parentNode.get()) parentNode->bindOwner(getOwnerItem());
}

feather::StrongPointer<Element> Node::getParentElement() const {
    auto ptr = getParentNode();
    return ptr->getNodeTypeInternal() == NodeType::ELEMENT_NODE ? std::static_pointer_cast<Element>(ptr) : nullptr;
}

feather::StrongPointer<Node> Node::getPrevSibling() const {
    if (prevSiblingPtr->isValid()) return prevSiblingPtr->get().lock();
    if (getParentNode().get()) {
        auto me = std::static_pointer_cast<const Node>(shared_from_this());
        auto children = getParentNode()->getChildNodes();
        for (UInt i = children.size(); i-- > 1;) {
            if (children.get(i) == me) {
                auto child = children.get(i - 1);
                prevSiblingPtr->set(child);
                return child;
            }
        }
    }
    prevSiblingPtr->set(WeakPointer<Node>());
    return nullptr;
}

feather::StrongPointer<feather::DOMString> Node::getTextContent() const {
    if (getNodeTypeInternal() != NodeType::ELEMENT_NODE) return getNodeValue();
    DOMString builder;
    auto children = getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::CDATA_SECTION_NODE:
            case NodeType::TEXT_NODE:
                builder += *child->getNodeValue();
                [[fallthrough]];
            default:
                break;
        }
    }
    return std::make_shared<DOMString>(builder);
}

void Node::setTextContent(feather::DOMString textContent) {
    switch (getNodeTypeInternal()) {
        case NodeType::ELEMENT_NODE:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
            getChildNodes().clear();
            getChildNodes().add(std::make_shared<Text>(getBaseURI(),
                                                       std::static_pointer_cast<Node>(shared_from_this()),
                                                       textContent));
            break;
        case NodeType::TEXT_NODE:
            setTextContent(textContent);
            break;
        default:
            break;
    }
}

feather::StrongPointer<Node> Node::appendChild(feather::StrongPointer<feather::dom::Node> child) {
    auto type = child->getNodeTypeInternal();
    if (type != NodeType::DOCUMENT_FRAGMENT_NODE && type != NodeType::SHADOW_ROOT) {
        child->setParentNode(std::static_pointer_cast<Node>(shared_from_this()));
        getChildNodes().add(child);
    } else {
        auto children = child->getChildNodes();

    }
    return child;
}

namespace {
    enum class DocumentPosition {
        DISCONNECTED, PRECEDING, FOLLOWING, CONTAINS, CONTAINED_BY, IMPLEMENTATION_SPECIFIC
    };
}

namespace {
    bool precedes(const Node &node1, const Node &node2) {
        feather::Deque<Node *> deque;
        auto parent = node1.getParentNode();
        while (parent.get()) deque.push_back(parent.get());
        parent = node2.getParentNode();
        auto p2 = parent;
        while (parent.get()) {
            auto it = std::find(deque.begin(), deque.end(), parent.get());
            if (it != deque.end()) {
                feather::Int idx = it - deque.begin() + 1;
                auto children = parent->getChildNodes();
                for (feather::Int i = 0; i < idx; i++) if (children.get(i) == p2) return false;
                return true;
            }
            p2 = parent;
            parent = parent->getParentNode();
        }
        return false;
    }
}

feather::UByte Node::compareDocumentPosition(feather::StrongPointer<const feather::dom::Node> node1) const {
    if (node1.get() == this) return 0u;
    const Node &node2 = *this;
    if (node1.get() == nullptr || (node1->getRootNode(true) != getRootNode(true)))
        return 1u << static_cast<UInt>(DocumentPosition::DISCONNECTED) |
               1u << static_cast<UInt>(DocumentPosition::PRECEDING) |
               1u << static_cast<UInt>(DocumentPosition::IMPLEMENTATION_SPECIFIC);
    if (node1->contains(std::static_pointer_cast<const Node>(shared_from_this())))
        return 1u << static_cast<UInt>(DocumentPosition::CONTAINS) |
               1u << static_cast<UInt>(DocumentPosition::PRECEDING);
    if (node2.contains(node1))
        return 1u << static_cast<UInt>(DocumentPosition::CONTAINED_BY) |
               1u << static_cast<UInt>(DocumentPosition::FOLLOWING);
    if (precedes(*node1.get(), node2)) return 1u << static_cast<UInt>(DocumentPosition::PRECEDING);
    return 1u << static_cast<UInt>(DocumentPosition::FOLLOWING);
}

bool Node::contains(feather::StrongPointer<const feather::dom::Node> other) const {
    StrongPointer<const Node> ptr = other;
    while (other.get() && other.get() != this) ptr = ptr->getParentNode();
    return ptr.get() != nullptr;
}

feather::StrongPointer<Node> Node::getRootNode(bool composed) const {
    switch (getNodeTypeInternal()) {
        case NodeType::SHADOW_ROOT:
            if (!composed) return nullptr;
            [[fallthrough]];
        default:
            if (getParentNode().get()) {
                auto type = getParentNode()->getNodeTypeInternal();
                switch (type) {
                    case NodeType::DOCUMENT_NODE:
                    case NodeType::DOCUMENT_FRAGMENT_NODE:
                        return getParentNode();
                    case NodeType::SHADOW_ROOT:
                        if (!composed) return getParentNode();
                        [[fallthrough]];
                    default:
                        return getParentNode()->getRootNode(composed);
                }
            }
            [[fallthrough]];
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::DOCUMENT_NODE:
            return nullptr;
    }
}

feather::StrongPointer<Node>
Node::insertBefore(feather::StrongPointer<Node> add, feather::StrongPointer<const Node> ref) {
    auto children = getChildNodes();
    auto type = add->getNodeTypeInternal();
    if (type == NodeType::DOCUMENT_FRAGMENT_NODE || type == NodeType::SHADOW_ROOT) {
        for (UInt i = 0; i < children.size(); i++) {
            if (children.get(i) == ref) {
                children.insert(i, add);
                return add;
            }
        }
        children.add(add);
        return add;
    } else {
        for (UInt i = 0; i < children.size(); i++) {
            if (children.get(i) == ref) {
                children.insertAll(i, add->getChildNodes());
                return add;
            }
        }
        return add;
    }
}

feather::StrongPointer<Node> Node::insertAfter(feather::StrongPointer<feather::dom::Node> add,
                                               feather::StrongPointer<const feather::dom::Node> ref) {
    auto type = add->getNodeTypeInternal();
    if (type == NodeType::DOCUMENT_FRAGMENT_NODE || type == NodeType::SHADOW_ROOT) {
        auto children = getChildNodes();
        for (UInt i = 0; i < children.size(); i++) {
            if (children.get(i) == ref) {
                children.insertAll(i + 1, add->getChildNodes());
                return add;
            }
        }
        children.insertAll(0, add->getChildNodes());
        return add;
    } else {
        auto children = getChildNodes();
        for (UInt i = 0; i < children.size(); i++) {
            if (children.get(i) == ref) {
                children.insert(i + 1, add);
                return add;
            }
        }
        children.insert(0, add);
        return add;
    }
}

namespace {
    bool isTrivial(feather::DOMString str) {
        for (auto c : str) if (!isspace(c))return false;
        return true;
    }
}

void Node::normalize() {
    auto children = getChildNodes();
    Vector <StrongPointer<Node>> normalized;
    DOMString str;
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::TEXT_NODE:
                str += std::static_pointer_cast<Text>(child)->getData();
                break;
            default:
                if (child->hasChildNodes()) child->normalize();
                if (!isTrivial(str))
                    normalized.push_back(std::make_shared<Text>(
                            getBaseURI(), std::static_pointer_cast<Node>(shared_from_this()), str));
                normalized.push_back(child);
                str.clear();
                break;
        }
    }
    children.swap(normalized);
}

feather::StrongPointer<Node> Node::removeChild(feather::StrongPointer<feather::dom::Node> child) {
    auto children = getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        if (children.get(i) == child) {
            children.get(i)->setParentNode(nullptr);
            children.remove(i);
        }
    }
    return child;
}

feather::StrongPointer<Node> Node::replaceChild(feather::StrongPointer<feather::dom::Node> newChild,
                                                feather::StrongPointer<feather::dom::Node> oldChild) {
    auto children = getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        if (children.get(i) == oldChild) {
            children.set(i, newChild);
            return oldChild;
        }
    }
    children.add(newChild);
    return oldChild;
}

void Node::insertBeforeChild(feather::Vector<std::shared_ptr<feather::dom::Node>> v,
                             feather::StrongPointer<const feather::dom::Node> ref) {
    auto children = getChildNodes();
    for (const auto &child : v) child->setParentNode(std::static_pointer_cast<Node>(shared_from_this()));
    for (UInt i = 0; i < children.size(); i++) {
        if (children.get(i) == ref) {
            children.insertAll(i, v);
            return;
        }
    }
    children.addAll(v);
}

void Node::insertAfterChild(feather::Vector<std::shared_ptr<feather::dom::Node>> v,
                            feather::StrongPointer<const feather::dom::Node> ref) {
    auto children = getChildNodes();
    for (const auto &child : v) child->setParentNode(std::static_pointer_cast<Node>(shared_from_this()));
    for (UInt i = 0; i < children.size(); i++) {
        if (children.get(i) == ref) {
            children.insertAll(i + 1, v);
            return;
        }
    }
    children.insertAll(0, v);
}

void Node::replaceChild(feather::Vector<std::shared_ptr<feather::dom::Node>> v,
                        feather::StrongPointer<const feather::dom::Node> replace) {
    auto children = getChildNodes();
    for (const auto &child : v) child->setParentNode(std::static_pointer_cast<Node>(shared_from_this()));
    for (UInt i = 0; i < children.size(); i++) {
        if (children.get(i) == replace) {
            children.get(i)->setParentNode(nullptr);
            children.remove(i);
            children.insertAll(i, v);
            return;
        }
    }
    children.addAll(getChildNodes());
}

feather::StrongPointer<Element>
Node::getElementBeforeChild(feather::StrongPointer<const feather::dom::Node> ref) const {
    auto children = getChildNodes();
    bool valid = false;
    for (UInt i = children.size() - 1; i-- > 0;) {
        auto child = children.get(i);
        if (valid && child->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
            return std::static_pointer_cast<Element>(child);
        }
        valid = valid || child == ref;
    }
    return nullptr;
}

feather::StrongPointer<Element> Node::getElementAfterChild(feather::StrongPointer<const feather::dom::Node> ref) const {
    auto children = getChildNodes();
    bool valid = false;
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (valid && child->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
            return std::static_pointer_cast<Element>(child);
        }
        valid = valid || child == ref;
    }
    return nullptr;
}

void Node::modify(feather::RegularEnumSet<feather::observable::InvEvent> &s,
                  const feather::observable::Invalidatable *ptr) const {
    if (s.contains(observable::InvEvent::LIST_CHANGE) && ptr == &childNodes) s.add(observable::InvEvent::RELAYOUT);
    s.add(observable::InvEvent::REPAINT);
    Invalidatable::modify(s, ptr);
}