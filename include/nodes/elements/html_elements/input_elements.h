//
// Created by prem on 12/30/2018.
//

#ifndef CURL_INPUT_ELEMENTS_H
#define CURL_INPUT_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {

            class HTMLFormElement;

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

                bool get(InvalidStates i) const { return (bitField & (1u << static_cast<unsigned int>(i))) != 0; }

                UShort bitField : static_cast<UInt>(InvalidStates::__COUNT__);
            };

            template<typename Derived>
            class HTMLFormTypeElement : public HTMLElement {
            public:

                B_ATTRIBUTE(autofocus, Autofocus)

                B_ATTRIBUTE(disabled, Disabled)

                const DOMString &getForm() const { return id; }

                StrongPointer <HTMLFormElement> getFormElement() const {
                    auto owner = getOwnerDocument();
                    if (owner) {
                    }
                }

                ATTRIBUTE(name, Name)

                ATTRIBUTE(type, Type)

                ATTRIBUTE(value, Value)

                ATTRIBUTE(formaction, FormAction)

                ATTRIBUTE(formenctype, FormEncType)

                ATTRIBUTE(formmethod, FormMethod)

                B_ATTRIBUTE(formnovalidate, FormNoValidate)

                ATTRIBUTE(formtarget, FormTarget)

                //TODO: tabindex

                inline StrongPointer<const ValidityState> getValidity() {
                    return StrongPointer<const ValidityState>(shared_from_this(), &validityState);
                }

                inline const DOMString &getValidityMessage() { return validityMessage; }

                inline bool willValidate() const { return static_cast<const Derived *>(this)->willValidateImpl(); }

                inline bool checkValidity() const { return static_cast<const Derived *>(this)->checkValidityImpl(); }

                inline void setCustomValidity(const DOMString &validityMessage) {
                    if (!validityMessage.empty()) {
                        validityState.add(InvalidStates::CUSTOM_ERROR);
                        this->validityMessage = validityMessage;
                    } else {
                        validityState.clear();
                        this->validityMessage = "";
                    }
                }

            private:
                DOMString id{};
                ValidityState validityState{};
                DOMString validityMessage{};
            };

            class HTMLMenuElement : public HTMLElement {

            };

            class HTMLLabelElement;

            class HTMLButtonElement : public HTMLFormTypeElement<HTMLButtonElement> {
            public:
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }

                inline const Vector <WeakPointer<HTMLLabelElement>> &getLabels() const { return labels; }

                inline void addLabel(const StrongPointer <HTMLLabelElement> &label) {
                    for (const auto &wptr : labels) if (wptr.lock() == label) return;
                    labels.push_back(label);
                }

                inline void removeLabel(const StrongPointer <HTMLLabelElement> &label) {
                    labels.erase(
                            std::remove_if(labels.begin(), labels.end(),
                                           [&label](WeakPointer<HTMLLabelElement> wptr) {
                                               return wptr.expired() || wptr.lock() == label;
                                           }), labels.end());
                }

                inline StrongPointer <HTMLMenuElement> getMenu() const { return menu; }

            private:
                Vector <WeakPointer<HTMLLabelElement>> labels;
                StrongPointer <HTMLMenuElement> menu;
            };

            class HTMLFieldSetElement : public HTMLFormTypeElement<HTMLFieldSetElement> {
            public:

                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }

                inline StrongPointer <HTMLFormControlsCollection> getElements() const { return elements; }

            private:
                StrongPointer <HTMLFormControlsCollection> elements;
            };

            class HTMLFormElement : public HTMLElement {

                inline StrongPointer <HTMLFormControlsCollection> getElements() { return elements; }

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

                bool checkValidityImpl();

                bool reportValidity();

            private:
                StrongPointer <HTMLFormControlsCollection> elements{};
            };

            class HTMLInputElement : public HTMLFormTypeElement<HTMLInputElement> {
            public:
                inline bool willValidateImpl() const { return false; }//TODO: add validation

                inline bool checkValidityImpl() const { return true; }
            };

            class HTMLLabelElement : public HTMLFormTypeElement<HTMLLabelElement> {
            public:
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }

            private:
                observable::ObservableItem<WeakPointer<HTMLElement>, false> control;
            };

            class HTMLLegendElement : public HTMLFormTypeElement<HTMLLegendElement> {
            public:

                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
                //TODO: finish up
            };

            class HTMLMeterElement : public HTMLFormTypeElement<HTMLMeterElement> {
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
                //TODO: write class
            };

            class HTMLOptGroupElement : public HTMLFormTypeElement<HTMLOptGroupElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };

            class HTMLOptionElement : public HTMLFormTypeElement<HTMLOptionElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };

            //Shut up i know this is a horrible coincidence
            class HTMLOutputElement : public HTMLFormTypeElement<HTMLOutputElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };

            class HTMLParamElement : public HTMLFormTypeElement<HTMLParamElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };

            class HTMLSelectElement : public HTMLFormTypeElement<HTMLSelectElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };

            class HTMLTextAreaElement : public HTMLFormTypeElement<HTMLTextAreaElement> {
                //TODO: write class
                inline bool willValidateImpl() const { return false; }

                inline bool checkValidityImpl() const { return true; }
            };
        }
    }
}
#endif //CURL_INPUT_ELEMENTS_H
