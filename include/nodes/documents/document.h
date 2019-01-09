//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "nodes/node.h"
#include "style/css/pseudoclass_manager.h"

namespace feather {
    namespace dom {

        namespace dlists {
            class ObjectMappedList;

            class FormMappedList;

            class ImageMappedList;

            class LinkMappedList;

            class EmbedMappedList;

            class ScriptMappedList;
        }
        namespace html {
            class HTMLElement;

            class HTMLHtmlElement;

            class HTMLHeadElement;

            class HTMLBodyElement;
        }

        class DocumentOrShadowRoot {
        public:
            css::PseudoclassManager &getPseudoclassManager() { return pseudoclassManager; }

        private:
            css::PseudoclassManager pseudoclassManager;
        };

        class DocumentType : public Node {
        public:
            inline DOMString getName() const { return name; }

            inline DOMString getPublicId() const { return ""; }

            inline DOMString getSystemId() const { return ""; }

            inline void remove() { getParentNode()->removeChild(getSharedFromThis()); }

        private:
            DOMString name;
        };

        class Document : public Node, public DocumentOrShadowRoot {
        public:

            StrongPointer<html::HTMLBodyElement> getBody() const;

            void setBody(StrongPointer<html::HTMLElement> body);

            DOMString getCharacterSet() const { return characterSet; }

            inline TriValue getMode() const { return quirks; }

            inline DOMString getContentType() const { return "application/xml"; }

            StrongPointer<DocumentType> getDocType() const;

            StrongPointer<html::HTMLHtmlElement> getDocumentElement() const;

            inline DOMString getDocumentURI() const { return documentURI; }

            StrongPointer<dlists::ObjectMappedList> getEmbeds() const;

            //TODO: FontFaceSet, document.fonts()

            StrongPointer<dlists::FormMappedList> getForms() const;

            StrongPointer<html::HTMLHeadElement> getHead();

            inline bool isHidden() { return hidden; }

            StrongPointer<dlists::ImageMappedList> getImages() const;

            //TODO: DOMImplementation

            inline DOMString getLastStyleSheet() const { return lastStyleSheet; }

            StrongPointer<dlists::LinkMappedList> getLinks() const;

            StrongPointer<dlists::EmbedMappedList> getPlugins() const;

            inline DOMString getPreferredStyleSheet() const { return preferredStyleSheet; }

            StrongPointer<dlists::ScriptMappedList> getScripts() const;

            StrongPointer<html::HTMLElement> getScrollingElement() const;

            inline DOMString getSelectedStyleSheet() const { return preferredStyleSheet; }

        private:
            DOMString characterSet;
            TriValue quirks;
            DOMString documentURI;
            bool hidden;
            DOMString lastStyleSheet;
            DOMString preferredStyleSheet;

            mutable StrongPointer<dlists::ObjectMappedList> objects;
            mutable StrongPointer<dlists::FormMappedList> forms;
            mutable StrongPointer<dlists::ImageMappedList> images;
            mutable StrongPointer<dlists::LinkMappedList> links;
            mutable StrongPointer<dlists::EmbedMappedList> embeds;
            mutable StrongPointer<dlists::ScriptMappedList> scripts;
        };
    }
}

#endif //FEATHER_DOCUMENT_H
