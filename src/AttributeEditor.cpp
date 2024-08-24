#include "AttributeEditor.hpp"

#include "attr.hpp"

bool AttributeEditor::setup(ObjectAttribute* objAttr) {

    auto container = CCNode::create();

    auto primaryButtons = CCMenu::create();
    auto cancel = CCMenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), nullptr, nullptr);
    auto commit = CCMenuItemSpriteExtra::create(ButtonSprite::create("Commit"), nullptr, nullptr);
    primaryButtons->addChild(cancel);
    primaryButtons->addChild(commit);
    primaryButtons->setLayout(RowLayout::create());
    primaryButtons->updateLayout();
    primaryButtons->setScale(0.75f);
    container->addChild(primaryButtons);

    m_textInput = ColorableTextInput::create(350.f, "New Value", "chatFont.fnt");
    m_textInput->setCallback([this](const std::string& text) {
        if (TypeContainer(m_typeInput->getSelectedOption()).setValue(text)) m_textInput->resetColor();
        else m_textInput->setColor(ccColor3B { 255, 100, 100 });
    });
    container->addChild(m_textInput);

    auto typeOptions = attrtype::getTypes();
    auto typeLabels = attrtype::getTypeStrings<gd::string>();
    typeLabels.pop_back();
    typeLabels.push_back("None");
    m_typeInput = EnumSelectList<AttributeType, gd::string>::create(100.f, typeOptions, typeLabels);
    container->addChild(m_typeInput);

    this->addChild(container);

    container->setPosition(ccp(m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() / 2));
    container->setAnchorPoint(ccp(0.5f, 0.5f));
    container->setContentSize(m_mainLayer->getContentSize());

    container->setLayout(ColumnLayout::create());
    container->updateLayout();

    return true;

}

AttributeEditor* AttributeEditor::create(ObjectAttribute* objAttr) {
    auto* editor = new AttributeEditor();
    if (editor->init(400.f, 130.f, objAttr)) {
        editor->autorelease();
        return editor;
    }
    delete editor;
    return nullptr;
}