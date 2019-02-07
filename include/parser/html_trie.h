//
// Created by prem on 2/5/2019.
//

#ifndef CURL_HTML_TRIE_H
#define CURL_HTML_TRIE_H

#include "typedefs.h"

namespace feather {
    namespace parser {

        struct Node {
            char c;
            bool end;
            Vector<Node> children;
        };

        const Map<DOMString, UInt> &getEscapeCodeMap();

        const Array<Node, 52> &getHeadNodes();

        const Array<int, 27> &getNumericEscapeCodes();

        class EscapeCodeTrieTraverser {
        public:
            EscapeCodeTrieTraverser(const EscapeCodeTrieTraverser &ref) = delete;

            explicit EscapeCodeTrieTraverser(const Node *ref) : ref(ref) {}

            EscapeCodeTrieTraverser(EscapeCodeTrieTraverser &&ref) = default;

            EscapeCodeTrieTraverser &operator=(const EscapeCodeTrieTraverser &ref) = default;

            EscapeCodeTrieTraverser &operator=(EscapeCodeTrieTraverser &&ref) = default;

            bool operator<<(char c) {
                for (const auto &n : ref->children) {
                    if (n.c == c) {
                        ref = &n;
                        return true;
                    }
                }
                return false;
            }

            explicit operator bool() {
                return ref->end;
            }

        private:
            const Node *ref;
        };
    }
}
#endif //CURL_HTML_TRIE_H
