//
// Created by prem on 12/11/2018.
//

#include "nodes/node.h"
#include "nodes/text/text.h"
#include "nodes/documents/document.h"

using namespace feather::dom;

feather::StrongPointer<feather::observable::Invalidatable> feather::dom::toInvalidatable(
        const feather::WeakPointer<feather::dom::Node> &ref) {
    return std::static_pointer_cast<observable::Invalidatable>(ref.lock());
}

feather::StrongPointer<feather::observable::Invalidatable> feather::dom::toInvalidatable(
        const feather::WeakPointer<feather::dom::Document> &ref) {
    return std::static_pointer_cast<observable::Invalidatable>(ref.lock());
}

bool NodeList::deepEquals(const feather::dom::NodeList &other) const {
    if (other.size() == size()) {
        for (UInt i = 0; i < size(); i++) {
            if (*other[i] != *(*this)[i]) return false;
        }
        return true;
    }
    return false;
}

void NodeList::invalidate() const {
    Invalidatable::invalidate(EnumSet<observable::InvEvent>(observable::InvEvent::LOCAL_CHILDREN_CHANGE),
                              this);
    Invalidatable::invalidate(EnumSet<observable::InvEvent>(observable::InvEvent::LOCAL_NODE_INDEX_CHANGE,
                                                            observable::InvEvent::PROPAGATE_DOWNWARD), this);
}

feather::EnumSet<feather::observable::InvEvent> NodeList::modify
        (feather::EnumSet<feather::observable::InvEvent> s, const feather::observable::Invalidatable *) const {
    return s - observable::InvEvent::INVALIDATE_THIS;
}

feather::UInt Node::getIndex() const {
    if (nodeIndex.isValid()) return *nodeIndex;
    if (getParentNode()) {
        getParentNode()->updateChildIndeces();
        return *nodeIndex;
    } else {
        nodeIndex = 0;
        return 0;
    }
}

void Node::updateChildIndeces() const {
    for (UInt i = 0; i < childNodes.size(); i++) childNodes[i]->nodeIndex = i;
}

feather::StrongPointer<Node> Node::replaceChild(feather::StrongPointer<feather::dom::Node> newChild,
                                                feather::StrongPointer<feather::dom::Node> oldChild) {
    if (oldChild->parent->lock().get() == this) {
        if (newChild->getParentNode()) newChild->getParentNode()->removeChild(newChild);
        newChild->setParentNode(getSharedFromThis());
        auto idx = oldChild->getIndex();
        oldChild->clearParentNode();
        childNodes.set(idx, newChild);
    }
    return oldChild;
}

feather::StrongPointer<Node> Node::removeChild(feather::StrongPointer<feather::dom::Node> child) {
    if (child->parent->lock().get() == this) {
        child->clearParentNode();
        getChildNodes()->remove(child->getIndex());
        return child;
    }
    return StrongPointer<Node>();
}

void Node::normalize() {
    UInt lastIndex = ~0u;
    DOMString temp;
    for (UInt i = childNodes.size(); i-- > 0;) {
        const auto &child = childNodes[i];
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
            case NodeType::SHADOW_ROOT:
            case NodeType::DOCUMENT_NODE:
            case NodeType::DOCUMENT_FRAGMENT_NODE: {
                if (~lastIndex && lastIndex > i + 1) {
                    auto reserve = temp.length();
                    auto text = Text::create(getBaseURI(), getSharedFromThis(), std::move(temp));
                    childNodes[lastIndex]->clearParentNode();
                    childNodes.removeAll(i + 1, lastIndex);
                    childNodes.insert(i + 1, std::move(text));
                    temp = "";
                    temp.reserve(reserve);
                    lastIndex = ~0u;
                }
                break;
            }
            case NodeType::TEXT_NODE: {
                auto val = *child->getNodeValue();
                temp.insert(temp.begin(), val.begin(), val.end());
                if (~lastIndex) {
                    child->clearParentNode();
                } else lastIndex = i;
                break;
            }
            default:
                break;
        }
    }
    if (~lastIndex && lastIndex > 0) {
        auto text = Text::create(getBaseURI(), getSharedFromThis(), std::move(temp));
        childNodes[lastIndex]->clearParentNode();
        childNodes.removeAll(0, lastIndex);
        childNodes.insert(0, std::move(text));
    }
}

feather::StrongPointer<Node> Node::insertAfter(feather::StrongPointer<feather::dom::Node> add,
                                               feather::StrongPointer<const feather::dom::Node> ref) {
    if (ref->getParentNode().get() == this) {
        if (add->getParentNode()) add->getParentNode()->removeChild(add);
        add->setParentNode(getSharedFromThis());
        childNodes.insert(ref->getIndex() + 1, add);
        return add;
    }
    return StrongPointer<Node>();
}

feather::StrongPointer<Node> Node::insertBefore(feather::StrongPointer<feather::dom::Node> add,
                                                feather::StrongPointer<const feather::dom::Node> ref) {
    if (ref->getParentNode().get() == this) {
        if (ref->getNodeTypeInternal() == NodeType::DOCUMENT_FRAGMENT_NODE) {
            auto children = add->getChildNodes();
            getChildNodes()->insertAll(ref->getIndex(), std::move(*children));
            children->clear();
            return add;
        } else {
            if (add->getParentNode()) add->getParentNode()->removeChild(add);
            add->setParentNode(getSharedFromThis());
            getChildNodes()->insert(ref->getIndex(), add);
            return add;
        }
    }
    return StrongPointer<Node>();
}

feather::StrongPointer<Node> Node::getRootNode(bool composed) const {
    StrongPointer <Node> parent = getParentNode();
    //TODO: Make this better
    if (!parent) return std::const_pointer_cast<Node>(getSharedFromThis());
    while (parent->getParentNode() && (composed || parent->getNodeTypeInternal() != NodeType::SHADOW_ROOT))
        parent = parent->getParentNode();
    return parent;
}

bool Node::contains(const feather::StrongPointer<const feather::dom::Node> &other) const {
    if (other.get() == this) return true;
    auto parent = other->getParentNode();
    while (parent && parent.get() != this) parent = parent->getParentNode();
    return parent != nullptr;
}

namespace {
    enum DocumentPosition {
        DISCONNECTED = 1u,
        PRECEDING = 1u << 1u,
        FOLLOWING = 1u << 2u,
        CONTAINS = 1u << 3u,
        CONTAINED_BY = 1u << 4u,
        IMPLEMENTATION_SPECIFIC = 1u << 5u
    };

    const feather::dom::Node *
    getPrecedence(const feather::dom::Node *const &root, const feather::dom::Node *const &node1,
                  const feather::dom::Node *const &node2) {
        for (const auto &child : *root->getChildNodes()) {
            if (child.get() == node1) return node1;
            if (child.get() == node2) return node2;
            if (child->hasChildNodes()) {
                auto precedence = getPrecedence(child.get(), node1, node2);
                if (precedence) return precedence;
            }
        }
        return nullptr;
    }

    bool precedes(const feather::StrongPointer<const feather::dom::Node> &node1,
                  const feather::StrongPointer<const feather::dom::Node> &node2) {
        return getPrecedence(node1->getRootNode().get(), node1.get(), node2.get()) == node1.get();
    }
}

unsigned char Node::compareDocumentPosition(const feather::StrongPointer<const feather::dom::Node> &other) const {
    if (other.get() == this) return 0u;
    if (!other || other->getRootNode(true) != getRootNode(true))
        return DocumentPosition::DISCONNECTED | DocumentPosition::PRECEDING | DocumentPosition::IMPLEMENTATION_SPECIFIC;
    if (other->contains(getSharedFromThis())) return DocumentPosition::CONTAINS | DocumentPosition::PRECEDING;
    if (contains(other)) return DocumentPosition::CONTAINED_BY | DocumentPosition::FOLLOWING;
    if (precedes(other, getSharedFromThis())) return DocumentPosition::PRECEDING;
    return DocumentPosition::FOLLOWING;
}

feather::StrongPointer<Node> Node::appendChild(feather::StrongPointer<feather::dom::Node> child) {
    if (child->getParentNode()) child->getParentNode()->removeChild(child);
    child->setParentNode(getSharedFromThis());
    getChildNodes()->add(child);
    return child;
}

void Node::setTextContent(feather::DOMString textContent) {
    switch (getNodeTypeInternal()) {
        case NodeType::SHADOW_ROOT:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::ELEMENT_NODE:
            getChildNodes()->clear();
            getChildNodes()->add(Text::create(getBaseURI(), getSharedFromThis(), std::move(textContent)));
            break;
        case NodeType::TEXT_NODE:
        case NodeType::COMMENT_NODE:
        case NodeType::PROCESSING_INSTRUCTION_NODE:
            *this->value = std::move(textContent);
            break;
        default:
            break;
    }
}

feather::StrongPointer<const feather::DOMString> Node::getTextContent() const {
    switch (getNodeTypeInternal()) {
        case NodeType::TEXT_NODE:
            return getNodeValue();
        case NodeType::ELEMENT_NODE:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::DOCUMENT_NODE:
            return std::make_shared<DOMString>(getTextContentInternal());
        default:
            return StrongPointer<DOMString>();
    }
}

feather::DOMString Node::getTextContentInternal() const {
    switch (getNodeTypeInternal()) {
        case NodeType::TEXT_NODE:
            return *value;
        case NodeType::ELEMENT_NODE:
        case NodeType::DOCUMENT_NODE:
        case NodeType::DOCUMENT_FRAGMENT_NODE:
        case NodeType::SHADOW_ROOT: {
            DOMString temp;
            for (const auto &child : childNodes) temp += child->getTextContentInternal();
            return temp;
        }
        default:
            return "";
    }
}

feather::StrongPointer<feather::dom::Node> Node::getPrevSibling() const {
    auto idx = getIndex();
    return idx ? childNodes[idx - 1] : StrongPointer<Node>();
}

feather::StrongPointer<Element> Node::getParentElement() const {
    auto parent = getParentNode();
    return parent->getNodeTypeInternal() == NodeType::ELEMENT_NODE
           ? std::static_pointer_cast<Element>(parent)
           : StrongPointer<Element>();
}

void Node::setParentNode(const feather::StrongPointer<feather::dom::Node> &parentNode) {
    parent = parentNode;
}

feather::StrongPointer<feather::dom::Document> Node::getOwnerDocument() const {
    if (owner.isValid()) return owner->lock();
    auto owner = getParentNode() ? getParentNode()->getOwnerDocument() : StrongPointer<Document>();
    this->owner = owner;
    return owner;
}

feather::StrongPointer<feather::dom::Node> Node::getNextSibling() const {
    auto idx = getIndex();
    return idx < childNodes.size() ? childNodes[idx] : StrongPointer<Node>();
}

void Node::insertBeforeChildNDTCN(const feather::StrongPointer<const feather::dom::Node> &ref,
                                  const feather::Vector<std::shared_ptr<feather::dom::Node>> &add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex.isValid()) idx = *ref->nodeIndex;
        else {
            idx = ~0u;
            while (++idx < childNodes.size() && childNodes[idx] != ref) childNodes[idx]->nodeIndex = idx;
        }
        for (const auto &child : add) {
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
        }
        childNodes.insertAll(idx, add);
    }
}

void Node::insertAfterChildNDTCN(const feather::StrongPointer<const feather::dom::Node> &ref,
                                 const feather::Vector<std::shared_ptr<feather::dom::Node>> &add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex.isValid()) idx = *ref->nodeIndex + 1;
        else {
            idx = 0;
            while (idx < childNodes.size() && childNodes[idx++] != ref);
        }
        for (const auto &child : add) {
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
        }
        childNodes.insertAll(idx, add);
    }
}

void Node::replaceChildNDTCN(const feather::StrongPointer<const feather::dom::Node> &ref,
                             const feather::Vector<std::shared_ptr<feather::dom::Node>> &add) {
    if (ref->getParentNode().get() == this) {
        UInt idx;
        if (ref->nodeIndex.isValid()) idx = *ref->nodeIndex;
        else {
            idx = 0;
            while (idx < childNodes.size() && childNodes[idx++] != ref);
        }
        for (const auto &child : add) {
            if (child->getParentNode()) child->getParentNode()->removeChild(child);
            child->setParentNode(getSharedFromThis());
        }
        childNodes.insertAll(idx, add);
        childNodes[idx - 1]->clearParentNode();
        childNodes.remove(idx - 1);
    }
}

Node::Node(feather::DOMString baseURI, feather::DOMString name, feather::dom::NodeType type,
           feather::StrongPointer<feather::DOMString> value,
           const feather::StrongPointer<feather::dom::Node> &parent)
        : baseURI(std::move(baseURI)), name(std::move(name)), type(type), value(std::move(value)) {
    bindTo(getChildNodes());
    auto parentItem = StrongPointer<observable::ObservableItem<feather::WeakPointer<feather::dom::Node>, true,
            toInvalidatable>>(getSharedFromThis(), &this->parent);
    owner.bindTo(parentItem);
    nodeIndex.bindTo(parentItem);
    setParentNode(parent);
}

namespace {
    bool isLocal(const feather::EnumSet<feather::observable::InvEvent> &types) {
        return feather::EnumSet<feather::observable::InvEvent>::size_type(types)
                       << static_cast<unsigned int>(feather::observable::InvEvent::__MANGLE__LOCAL__) != 0u;
    }

    const feather::ULong global_mask = ~(~0u
            >> static_cast<unsigned int>(feather::observable::InvEvent::__MANGLE_NONLOCAL__));

    void delocalize(feather::EnumSet<feather::observable::InvEvent> &types) {
        types = ((feather::EnumSet<feather::observable::InvEvent>::size_type(types) & global_mask) |
                 (feather::EnumSet<feather::observable::InvEvent>::size_type(types)
                         << static_cast<unsigned int>(feather::observable::InvEvent::__MANGLE__LOCAL__)
                         >> static_cast<unsigned int>(feather::observable::InvEvent::__MANGLE_NONLOCAL__)));
    }
}

feather::EnumSet<feather::observable::InvEvent> Node::modify(
        feather::EnumSet<feather::observable::InvEvent> types,
        const feather::observable::Invalidatable *p) const {
    types += observable::InvEvent::INVALIDATE_THIS;
    if (isLocal(types)) {
        if (p == &childNodes) delocalize(types);
        //TODO: Implement change-specific handling
    }
    if (types[observable::InvEvent::PROPAGATE_DOWNWARD]) {
        types += observable::InvEvent::STOP_PROPAGATION;
        for (const auto &child : childNodes) child->invalidate(types, p);
    }
    return types;
}