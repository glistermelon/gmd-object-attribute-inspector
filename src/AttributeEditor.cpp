#include "AttributeEditor.hpp"

#include "attributes.hpp"
#include "AttributeDocs.hpp"
#include "InspectorPopup.hpp"

void HSVDisplayDelegate::hsvPopupClosed(HSVWidgetPopup* popup, ccHSVValue newHSV) {
    static_cast<HSVDisplay*>(popup->getUserData())->updateDisplay(newHSV);
    delete this;
}

bool HSVDisplay::init(ColorHSV hsv) {

    if (!CCMenu::init()) return false;

    m_hsv = hsv;

    auto colorSprite = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    colorSprite->setColor(DARK_BROWN_3B);
    colorSprite->setContentSize(CCSize(100.f, 50.f));
    auto colorButton = CCMenuItemSpriteExtra::create(colorSprite, this, menu_selector(HSVDisplay::onClick));
    colorButton->setPosition(colorSprite->getContentSize() / 2);
    this->addChild(colorButton);

    this->setContentSize(colorSprite->getContentSize());
    colorSprite->setAnchorPoint(ccp(0.f, 0.f));
    colorSprite->setPosition(ccp(0.f, 0.f));

    auto labelContainer = CCNode::create();
    this->addChild(labelContainer);

    CCLabelBMFont** labels[] = { &m_vLabel, &m_sLabel, &m_hLabel };
    for (CCLabelBMFont** labelPtr : labels) {
        *labelPtr = CCLabelBMFont::create("", "chatFont.fnt");
        (*labelPtr)->limitLabelWidth(colorSprite->getContentWidth(), 1.f, 0.1f);
        labelContainer->addChild(*labelPtr);
    }

    labelContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    labelContainer->setPosition(ccp(this->getContentWidth() / 2, this->getContentHeight() / 2));
    labelContainer->setScale(0.7f);
    labelContainer->setContentSize(this->getContentSize() / 0.7f);
    labelContainer->setZOrder(colorSprite->getZOrder() + 1);

    this->updateDisplay(hsv);

    auto labelsLayout = ColumnLayout::create();
    labelsLayout->setCrossAxisOverflow(false);
    labelsLayout->setAutoScale(false);
    labelContainer->setLayout(labelsLayout);
    labelContainer->updateLayout();

    return true;

}

HSVDisplay* HSVDisplay::create(ColorHSV hsv) {
    auto display = new HSVDisplay;
    if (display->init(hsv)) {
        display->autorelease();
        return display;
    }
    delete display;
    return nullptr;
}

void HSVDisplay::updateDisplay(ColorHSV hsv) {
    m_hsv = hsv;
    std::string s;
    s = (std::string)"H: " + std::to_string(hsv.h);
    m_hLabel->setString(s.c_str());
    s = (std::string)"S: " + (hsv.s_on ? "+" : "x") + std::to_string(hsv.s);
    m_sLabel->setString(s.c_str());
    s = (std::string)"V: " + (hsv.v_on ? "+" : "x") + std::to_string(hsv.v);
    m_vLabel->setString(s.c_str());
}

void HSVDisplay::onClick(CCObject*) {
    auto popup = HSVWidgetPopup::create(m_hsv.cchsv(), new HSVDisplayDelegate, "");
    popup->setUserData(static_cast<void*>(this));
    popup->show();
}

bool AttributeEditor::setup(GameObjectWrapper* object, int attrKey) {

    m_object = object;
    m_attrKey = attrKey;

    // Basically I dump all my nodes in this container and then apply a layout
    auto container = CCNode::create();
    this->addChild(container);

    // Set up primary (cancel and commit) buttons

    auto primaryButtons = CCMenu::create();
    auto cancel = CCMenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), this, menu_selector(AttributeEditor::onCancel));
    auto commit = CCMenuItemSpriteExtra::create(ButtonSprite::create("Commit"), this, menu_selector(AttributeEditor::onCommit));
    primaryButtons->addChild(cancel);
    primaryButtons->addChild(commit);
    primaryButtons->setLayout(RowLayout::create());
    primaryButtons->updateLayout();
    primaryButtons->setScale(0.75f);
    container->addChild(primaryButtons);

    // Begin setting up an input node which contains a text input, HSV input, and boolean input
    m_inputArea = CCMenu::create();
    container->addChild(m_inputArea);

    auto attr = m_object->getAttribute(attrKey);
    auto type = attr.has_value() ? attr.value().getType() : ATTR_TYPE_UNKNOWN;

    // Add the text input
    m_textInput = ColorableTextInput::create(350.f, "New Value", "chatFont.fnt");
    m_textInput->setCallback([this](const std::string& text) {
        if (ValueContainer(m_typeInput->getSelectedOption()).setValue(text)) m_textInput->resetColor();
        else m_textInput->setColor(ccColor3B { 255, 100, 100 });
    });
    m_inputArea->addChild(m_textInput);

    // Add the boolean input
    std::vector<bool> boolOptions = { true, false };
    std::vector<gd::string> boolLabels = { "true", "false" };
    if (type == ATTR_TYPE_BOOL && !attr.value().getBoolValue()) {
        std::iter_swap(boolOptions.begin(), boolOptions.begin() + 1);
        std::iter_swap(boolLabels.begin(), boolLabels.begin() + 1);
    }
    m_boolInput = EnumSelectList<bool, gd::string>::create(100.f, boolOptions, boolLabels);
    m_inputArea->addChild(m_boolInput);

    // Add the HSV input
    m_colorInput = HSVDisplay::create(ColorHSV(0, 0, 0, 0, 0));
    m_colorInput->setScale(0.8f);
    m_inputArea->addChild(m_colorInput);

    // Set up the type selector

    auto typeOptions = attrtype::getAllTypes();
    auto typeLabels = attrtype::getAllTypeStrings<gd::string>();
    typeLabels.pop_back();
    typeLabels.push_back("None");

    int typeIndex = 0;
    while (typeOptions[typeIndex] != type) ++typeIndex;
    std::rotate(typeOptions.begin(), typeOptions.begin() + typeIndex, typeOptions.end());
    std::rotate(typeLabels.begin(), typeLabels.begin() + typeIndex, typeLabels.end());

    if (type == ATTR_TYPE_COLOR) m_colorInput->updateDisplay(attr.value().getColorValue().value());
    else if (type != ATTR_TYPE_BOOL) m_textInput->setString(attr.value().getRaw());

    m_typeInput = EnumSelectList<AttributeType, gd::string>::create(
        100.f, typeOptions, typeLabels,
        [this](AttributeType) { this->updateInputArea(); }
    );
    container->addChild(m_typeInput);

    // Decide which inputs to show/hide depending on attribute current type and value
    this->updateInputArea();

    // Final container setup
    container->setPosition(ccp(m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() / 2));
    container->setAnchorPoint(ccp(0.5f, 0.5f));
    container->setContentSize(m_mainLayer->getContentSize());
    container->setLayout(ColumnLayout::create());
    container->updateLayout();

    return true;

}

void AttributeEditor::updateInputArea() {

    m_textInput->setVisible(false);
    m_boolInput->setVisible(false);
    m_colorInput->setVisible(false);
    CCNode* show = nullptr;
    auto type = m_typeInput->getSelectedOption();
    switch (type) {
        case ATTR_TYPE_BOOL:
            show = m_boolInput;
            break;
        case ATTR_TYPE_COLOR:
            show = m_colorInput;
            break;
        default:
            show = m_textInput;
    }
    show->setVisible(true);
    m_inputArea->setContentSize(show->getScaledContentSize());
    show->setAnchorPoint(ccp(0.5f, 0.5f));
    show->ignoreAnchorPointForPosition(false);
    show->setPosition(ccp(m_inputArea->getContentWidth() / 2, m_inputArea->getContentHeight() / 2 + 2.f));

    switch (m_typeInput->getSelectedOption()) {
        case ATTR_TYPE_INT:
            m_textInput->setCommonFilter(CommonFilter::Int);
            break;
        case ATTR_TYPE_FLOAT:
            m_textInput->setCommonFilter(CommonFilter::Float);
            break;
        case ATTR_TYPE_ARRAY:
            m_textInput->setCommonFilter(CommonFilter::Any);
            break;
        case ATTR_TYPE_COLOR:
            m_textInput->setCommonFilter(CommonFilter::Any);
            break;
    }

}

AttributeEditor* AttributeEditor::create(GameObjectWrapper* object, int attrKey) {
    auto* editor = new AttributeEditor();
    if (editor->init(400.f, 130.f, object, attrKey)) {
        editor->autorelease();
        return editor;
    }
    delete editor;
    return nullptr;
}

void AttributeEditor::onCancel(CCObject*) {
    this->getParent()->removeChild(this);
}

void AttributeEditor::onCommit(CCObject*) {

    auto type = m_typeInput->getSelectedOption();
    auto attr = m_object->getAttribute(m_attrKey).value();
    if (type == ATTR_TYPE_COLOR) {
        attr.setColorValue(m_colorInput->getColorValue());
    }
    else if (type == ATTR_TYPE_BOOL) {
        attr.setBoolValue(m_boolInput->getSelectedOption());
    }
    else {
        if (!attr.setValue(m_textInput->getString(), true, type))
            return commitFailed();
    }

    m_object->setAttribute(m_attrKey, attr);

    m_object->tryCommit(m_attrKey, [this](bool committed) {
        if (committed) this->getParent()->removeChild(this);
    });

}

void AttributeEditor::onExit() {
    InspectorPopup::get()->loadObject();
}