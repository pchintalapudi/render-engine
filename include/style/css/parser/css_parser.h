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

namespace feather {
    namespace css {
        namespace parser {
            /**
             * CSSParser assumes that the given text is UTF-8 text
             */
            class CSSParser {
                void feed(char *);

                void feed(std::string);

                void clear();

                std::shared_ptr<CSSStyleSheet> read();
            };

            class CSSTokenizer {
            private:
                std::queue<std::tuple<std::shared_ptr<char[]>, size_t>> text;
                size_t index;
                size_t length;
            public:
                enum Token {
                    IDENT_TOKEN,
                    FUNCTION,
                    AT_KEYWORD,
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
                    QUOTATION_MARK,
                    NUMBER_SIGN,
                    DOLLAR_SIGN,
                    APOSTROPHE,
                    LEFT_PARENTHESIS,
                    RIGHT_PARENTHESIS,
                    ASTERISK,
                    PLUS_SIGN,
                    COMMON,
                    HYPHEN_MINUS,
                    FULL_STOP,
                    SOLIDUS,
                    COLON,
                    SEMICOLON,
                    LESS_THAN_SIGN,
                    COMMERICAL_AT,
                    LEFT_SQUARE_BRACKET,
                    REVERSE_SOLIDUS,
                    RIGHT_SQUARE_BRACKET,
                    CIRCUMFLEX_ACCENT,
                    LEFT_CURLY_BRACKET,
                    LAST_CHARACTER,
                    EOF_TOKEN
                };

                CSSTokenizer();

                inline void process(std::shared_ptr<char[]> elements, size_t n) {
                    if (n <= 0) return;
                    this->length += n;
                    this->text.emplace(std::make_tuple(elements, n));
                }

                Token next();

                inline size_t get_length() {
                    return std::get<1>(this->text.front());
                }

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
