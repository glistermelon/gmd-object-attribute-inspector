#pragma once

#include "include.hpp"

#include "ObjectAttribute.hpp"

template <class Opt, class T = std::string, auto Stringify = do_nothing<std::string>>
class EnumSelectList : public SelectList<T, Stringify> {

    using Super = SelectList<T, Stringify>;

    std::vector<Opt> m_options;
    utils::MiniFunction<void(Opt)> m_onChange;

    bool init(float width, std::vector<Opt> const& options, std::vector<T> const& labels, utils::MiniFunction<void(Opt)> onChange) {

        if (options.size() != labels.size() || options.empty()) return false;
        if (!Super::init(width, labels, [this](const T&, size_t) { this->m_onChange(this->getSelectedOption()); })) return false;

        m_onChange = onChange;
        m_options = options;

        float buttonOffset = Super::m_label->getPositionX() - Super::m_prevBtn->getPositionX();
        Super::m_label->setPosition(this->getContentWidth() / 2, this->getContentHeight() / 2);
        Super::m_prevBtn->setPosition(ccp(Super::m_label->getPositionX() - buttonOffset, Super::m_label->getPositionY()));
        Super::m_nextBtn->setPosition(ccp(Super::m_label->getPositionX() + buttonOffset, Super::m_label->getPositionY()));

        return true;
    }

public:

    static EnumSelectList* create(float width, std::vector<Opt> options, std::vector<T> const& labels, utils::MiniFunction<void(Opt)> onChange = [](Opt) {}) {
        auto* list = new EnumSelectList();
        if (list->init(width, options, labels, onChange)) {
            list->autorelease();
            return list;
        }
        delete list;
        return nullptr;
    }

    Opt getSelectedOption() {
        return m_options[Super::m_index];
    }

};

class ColorableTextInput : public TextInput {

    bool init(float width, std::string const& placeholder, std::string const& font) {
        return TextInput::init(width, placeholder, font);
    }

public:

    static ColorableTextInput* create(float width, std::string const& placeholder, std::string const& font = "bigFont.fnt") {
        auto* input = new ColorableTextInput();
        if (input->init(width, placeholder, font)) {
            input->autorelease();
            return input;
        }
        delete input;
        return nullptr;
    }

    void setColor(ccColor3B color) {
        TextInput::m_label->setColor(color);
    }

    void resetColor() {
        TextInput::m_label->setColor(ccColor3B { 255, 255, 255 });
    }

};

class AttributeEditor : public geode::Popup<ObjectAttribute*> {

    ColorableTextInput* m_textInput;
    EnumSelectList<AttributeType, gd::string>* m_typeInput;

    bool setup(ObjectAttribute* objAttr) override;

public:

    static AttributeEditor* create(ObjectAttribute* objAttr);

};