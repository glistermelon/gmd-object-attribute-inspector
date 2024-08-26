#pragma once

#include "include.hpp"

#include "GameObjectWrapper.hpp"

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
        TextInput::m_input->setLabelNormalColor(color);
    }

    void resetColor() {
        TextInput::m_input->setLabelNormalColor(ccColor3B { 255, 255, 255 });
    }

};

class HSVDisplayDelegate : public HSVWidgetDelegate {
public:
    void hsvPopupClosed(HSVWidgetPopup* popup, ccHSVValue newHSV) override;
};

class HSVDisplay : public CCMenu {

    CCLabelBMFont* m_hLabel;
    CCLabelBMFont* m_sLabel;
    CCLabelBMFont* m_vLabel;
    ColorHSV m_hsv;

    bool init(ColorHSV hsv);

public:

    static HSVDisplay* create(ColorHSV hsv);

    void updateHSV(ColorHSV hsv);

    void buttonCallback(CCObject*);

    inline ColorHSV getColorValue() { return m_hsv; }

};

class AttributeEditor : public geode::Popup<GameObjectWrapper*, int> {

    GameObjectWrapper* m_object;
    int m_attrKey;
    ColorableTextInput* m_textInput;
    EnumSelectList<AttributeType, gd::string>* m_typeInput;
    EnumSelectList<bool, gd::string>* m_boolInput;
    HSVDisplay* m_colorInput;
    CCMenu* m_inputArea;

    bool setup(GameObjectWrapper* object, int attrKey) override;
    void updateInputArea();

public:

    static AttributeEditor* create(GameObjectWrapper* object, int attrKey);

    void cancel(CCObject*);
    void commit(CCObject*);
    void finalizeCommit();

};