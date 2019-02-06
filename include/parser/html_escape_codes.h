//
// Created by prem on 2/4/2019.
//

#ifndef CURL_HTML_ESCAPE_CODES_H
#define CURL_HTML_ESCAPE_CODES_H

#include "html_trie.h"

//#define RECONSTRUCT_ESCAPE_CODES

namespace feather {
    namespace parser {
        template<typename... P>
        constexpr feather::Array<feather::Pair<feather::DOMString, UInt>, sizeof...(P)> makeLiteralArray(P... args) {
            std::array<feather::Pair<feather::DOMString, UInt>, sizeof...(P)> arr = {args...};
            return arr;
        }


#ifdef RECONSTRUCT_ESCAPE_CODES
        template<typename arr>
        Pair <UInt, UInt> getBounds(arr &array, char c, UInt index, UInt start, UInt end) {
            UInt first, last;
            for (first = ~0u, last = start; last < end; last++) {
                const DOMString &ref = array[last].first;
                if (ref.length() > index && ref[index] == c) {
                    if (!~first) first = last;
                } else if (~first) {
                    return {first, last};
                }
            }
            return ~first ? UnaryPair<UInt>{first, last} : UnaryPair<UInt>{~0u, ~0u};
        }
#endif
    }
}
#endif //CURL_HTML_ESCAPE_CODES_H
