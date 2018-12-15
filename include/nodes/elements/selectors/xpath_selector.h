//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_XPATH_SELECTOR_H
#define FEATHER_XPATH_SELECTOR_H
namespace feather {
    namespace dom {
        namespace selector {
            enum class Operator {
                CHILD,
                DESCENDANT,
                CURRENT,
                PARENT,
                WILDCARD,
                ATTRIBUTE,
                ATTRIBUTE_WILDCARD,
                NAMESPACE,
                PRECEDENCE,
                FILTER,
                SUBSCRIPT,
                ADDITION,
                SUBTRACTION,
                DIVISION,
                MULTIPLICATION,
                MODULAR,
                AND,
                NOT,
                EQUALS,
                NOT_EQUALS,
                LESS_THAN,
                LESS_THAN_OR_EQUAL,
                GREATER_THAN,
                GREATER_THAN_OR_EQUAL,
                UNION
            };

            class XPathSelector {

            };
        }
    }
}
#endif //FEATHER_XPATH_SELECTOR_H
