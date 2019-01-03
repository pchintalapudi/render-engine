//
// Created by prem on 12/30/2018.
//

#ifndef CURL_INPUT_ELEMENTS_H
#define CURL_INPUT_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {
            class HTMLButtonElement : public HTMLElement {
            public:
                inline bool isAutofocus() const { return hasAttribute("autofocus"); }

                inline void setAutofocus(bool autofocus) { toggleAttribute("autofocus", autofocus); }
                //TODO: Implement the rest
            };

            enum class InvalidStates {
                BAD_INPUT,
                CUSTOM_ERROR,
                PATTERN_MISMATCH,
                RANGE_OVERFLOW,
                RANGE_UNDERFLOW,
                STEP_MISMATCH,
                TOO_LONG,
                TOO_SHORT,
                TYPE_MISMATCH,
                VALUE_MISSING,
                __COUNT__
            };

            class ValidityState {
            public:
                ValidityState() = default;

                explicit ValidityState(UShort bitField) : bitField(bitField) {}

                inline ValidityState &add(InvalidStates i) {
                    bitField |= (1u << static_cast<unsigned int>(i));
                    return *this;
                }

                inline ValidityState &remove(InvalidStates i) {
                    bitField &= ~(1u << static_cast<unsigned int>(i));
                    return *this;
                }

                inline ValidityState &clear() {
                    bitField = 0;
                    return *this;
                }

                inline bool isBadInput() const { return get(InvalidStates::BAD_INPUT); }

                inline bool isCustomError() const { return get(InvalidStates::CUSTOM_ERROR); }

                inline bool isPatternMismtach() const { return get(InvalidStates::PATTERN_MISMATCH); }

                inline bool isRangeOverflow() const { return get(InvalidStates::RANGE_OVERFLOW); }

                inline bool isRangeUnderflow() const { return get(InvalidStates::RANGE_UNDERFLOW); }

                inline bool isStepMismatch() const { return get(InvalidStates::STEP_MISMATCH); }

                inline bool isTooLong() const { return get(InvalidStates::TOO_LONG); }

                inline bool isTooShort() const { return get(InvalidStates::TOO_SHORT); }

                inline bool isTypeMismatch() const { return get(InvalidStates::TYPE_MISMATCH); }

                inline bool isValid() const { return !bitField; }

                inline bool isValueMissing() const { return get(InvalidStates::VALUE_MISSING); }

            private:

                bool get(InvalidStates i) const { return (bitField & (1u << static_cast<int>(i))) != 0; }

                UShort bitField : static_cast<UInt>(InvalidStates::__COUNT__);
            };

            class HTMLFormElement;

            class HTMLFieldSetElement : public HTMLElement {
            public:
                inline bool isDisabled() const { return hasAttribute("disabled"); }

                inline void setDisabled(bool disabled) { toggleAttribute("disabled", disabled); }

                inline StrongPointer<HTMLFormControlsCollection> getElements() const { return elements; }

                inline DOMString getName() const { return getAttributeSafe("name"); }

                inline void setName(DOMString name) { setAttribute("name", std::move(name)); }

                inline DOMString getType() const { return "fieldset"; }

                inline void setType(DOMString type) { setAttribute("type", std::move(type)); }

                inline StrongPointer<ValidityState> getValidity() { return validityState; }

                inline DOMString getValidityMessage() { return validityMessage; }

                inline bool willValidate() { return false; }

                inline bool checkValidity() { return !validityMessage.empty(); }

                inline void setCustomValidity(DOMString validityMessage) {
                    if (!validityMessage.empty()) {
                        validityState->add(InvalidStates::CUSTOM_ERROR);
                        this->validityMessage = std::move(validityMessage);
                    } else {
                        validityState->clear();
                        this->validityMessage = "";
                    }
                }

            private:
                StrongPointer<ValidityState> validityState;
                StrongPointer<HTMLFormControlsCollection> elements;
                DOMString validityMessage;
            };

            class HTMLFormElement : public HTMLElement {

                inline StrongPointer<HTMLFormControlsCollection> getElements() { return elements; }

                inline UInt getLength() const { return elements->size(); }

                inline DOMString getName() const { return getAttributeSafe("name"); }

                inline void setName(DOMString name) { setAttribute("name", std::move(name)); }

                inline DOMString getMethod() const { return getAttributeSafe("method"); }

                inline void setMethod(DOMString method) { setAttribute("method", std::move(method)); }

                inline DOMString getTarget() const { return getAttributeSafe("target"); }

                inline void setTarget(DOMString target) { setAttribute("target", std::move(target)); }

                inline DOMString getAction() const { return getAttributeSafe("action"); }

                inline void setAction(DOMString action) { setAttribute("action", std::move(action)); }

                inline DOMString getEncType() { return getAttributeSafe("enctype"); }

                inline DOMString getEncoding() { return getEncType(); }

                inline void setEncType(DOMString encType) { setAttribute("enctype", std::move(encType)); }

                inline void setEncoding(DOMString encoding) { setEncType(std::move(encoding)); }

                inline DOMString getAcceptCharset() const { return getAttributeSafe("accept-charset"); }

                inline void setAcceptCharset(DOMString acceptCharset) {
                    setAttribute("accept-charset", std::move(acceptCharset));
                }

                inline DOMString getAutocomplete() const { return getAttributeSafe("autocomplete"); }

                inline void setAutocomplete(DOMString autocomplete) {
                    setAttribute("autocomplete", std::move(autocomplete));
                }

                inline bool getNoValidate() const { return hasAttribute("novalidate"); }

                inline void setNoValidate(bool noValidate) { toggleAttribute("novalidate", noValidate); }

                void submit();

                void reset();

                bool checkValidity();

                bool reportValidity();

            private:
                StrongPointer<HTMLFormControlsCollection> elements;
            };

            class HTMLInputElement : public HTMLElement {
                //TODO: Major major todo; complicated class
            };

            class HTMLLabelElement : public HTMLElement {
                //TODO: finish
            };

            class HTMLLegendElement : public HTMLElement {
            public:
                StrongPointer<HTMLFormElement> getForm();
                //TODO: finish up
            private:
                WeakPointer<observable::WatchedObservableItem<WeakPointer<HTMLFormElement>>> form;
            };

            class HTMLMeterElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLOptGroupElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLOptionElement : public HTMLElement {
                //TODO: write class
            };

            //Shut up i know this is a horrible coincidence
            class HTMLOutputElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLParamElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLSelectElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTextAreaElement : public HTMLElement {
                //TODO: write class
            };
        }
    }
}
#endif //CURL_INPUT_ELEMENTS_H
