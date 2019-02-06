//
// Created by binhe on 12/20/2018.
//

#ifndef CURL_CSS_TOKENIZER_H
#define CURL_CSS_TOKENIZER_H

#include <tuple>
#include <string>
#include <memory>
#include <queue>
#include "style/css/css_style_sheet.h"
#include "enums/enum_set.h"
#include "typedefs.h"

namespace feather {
    namespace css {
        namespace parser {
            /**
             * CSSParser assumes that the given text is UTF-8 text and attempts to parse the element into a
             * CSSStyleSheet. This sheet is updated as the parser is fed characters. A new style sheet can be generated
             * and added to by calling clear().
             */
            class CSSParser {
                bool feed(char *);

                bool feed(std::string);

                void clear();

                std::vector<int> getErrors();

                std::shared_ptr<CSSStyleSheet> read();
            };

            class CSSTokenizer {
            public:
                enum class Token {
                    IDENT_TOKEN,
                    FUNCTION,
                    AT_KEYWORD,
                    ASTERISK,
                    HASH,
                    STRING,
                    BAD_STRING,
                    URL,
                    BAD_URL,
                    DELIM,
                    NUMBER,
                    PERCENTAGE,
                    DIMENSION,
                    UNICODE_RANGE,
                    INCLUDE_MATCH,
                    DASH_MATCH,
                    PREFIX_MATCH,
                    SUFFIX_MATCH,
                    SUBSTRING_MATCH,
                    COLUMN,
                    WHITESPACE,
                    CDO,
                    CDC,
                    COLON,
                    SEMICOLON,
                    COMMA,
                    LEFT_SQUARE,
                    RIGHT_SQUARE,
                    LEFT_PARENTHESIS,
                    RIGHT_PARENTHESIS,
                    LEFT_BRACKED,
                    RIGHT_BRACKET,
                    EOF_TOKEN,
                    WAIT_TOKEN,
                    __COUNT__
                };
                typedef feather::EnumSet<Token> TokenSet;

            private:
                std::queue<std::tuple<std::shared_ptr<char[]>, size_t>> text;
                size_t index;
                size_t length;
                size_t processed;
                bool finalized;

                /**
                 * Advances the character stream by one character and if necessary move to the next text sequence.
                 * If it is not possible, returns true for EOF. Being EOF can be invalidated if more text is later
                 * processed.
                 * @return a boolean determining whether the end of the stream has been reached。
                 */
                inline bool advance() {
                    if (this->text.empty()) {
                        return true;
                    }
                    size_t front_length;
                    if ((++this->index) < (front_length = this->get_front_length())) {
                        this->processed++;
                        return false;
                    }
                    this->text.pop();
                    if (!this->text.empty()) {
                        this->processed++;
                        this->index -= front_length;
                        return false;
                    } else {
                        return true;
                    }
                }

                inline char get_advance() {
                    char current = this->get();
                    if (this->advance()) {
                        if (this->finalized) {
                            return (char)0;
                        } else {
                            return (char)1;
                        }
                    }
                    return current;
                }


                /**
                 * Gets the length of the text sample in the front of the queue to be processed.
                 * @return the size of the text sample in the front of the queue.
                 */
                inline size_t get_front_length() {
                    return std::get<1>(this->text.front());
                }

                TokenSet processNumber();
                TokenSet processUrl();
                TokenSet processAt();
                TokenSet processQuotation();
                TokenSet processWhitespace();

            public:

                CSSTokenizer();

                inline void process(std::shared_ptr<char[]> elements, size_t n) {
                    if (n <= 0) return;
                    this->length += n;
                    this->text.emplace(std::make_tuple(elements, n));
                }

                Token next();

                /**
                 * finalize marks that the end of the text being tokenized has been reached. This implies that no more
                 * text will be fed into this tokenizer.
                 */
                inline void finalize() {
                    this->finalized = true;
                }

                /**
                 * Gets the character in the front of the text corpus.
                 * @return the character at the front of the text sequence
                 */
                inline char get() {
                    return std::get<0>(this->text.front())[this->index];
                }

                inline size_t size() {
                    return this->length;
                }
            };
        }
    }
}

#endif //CURL_CSS_TOKENIZER_H
