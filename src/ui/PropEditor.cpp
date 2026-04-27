#include "PropEditor.hpp"

#include "InspectorPopup.hpp"

PropEditor::PropEditor(std::optional<PropKey> propKey, GameObjectWrapper objectWrapper)
    : m_propKey(propKey), m_objectWrapper(objectWrapper) { }

bool PropEditor::init(cocos2d::CCSize size) {

    const float inputWidth = 250.0f;

    std::string title = m_propKey.has_value()
        ? fmt::format("Edit Property {}", m_propKey.value().getString())
        : "Add Property";
    auto titleLabel = cocos2d::CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
    titleLabel->limitLabelWidth(inputWidth, 1.0f, 0.1f);

    if (!m_propKey.has_value()) {
        m_keyInput = geode::TextInput::create(inputWidth, "New Key", "chatFont.fnt");
    }

    auto maybePropVal = m_propKey.has_value()
        ? m_objectWrapper.getPropValue(m_propKey.value())
        : std::nullopt;
    m_valInput = geode::TextInput::create(inputWidth, "New Value", "chatFont.fnt");
    if (maybePropVal.has_value()) m_valInput->setString(maybePropVal.value().getString());

    auto cancelBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel"), this, menu_selector(PropEditor::onCancel)
    );
    auto applyBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Apply"), this, menu_selector(PropEditor::onApply)
    );

    auto buttonMenu = cocos2d::CCMenu::create();
    buttonMenu->addChild(cancelBtn);
    buttonMenu->addChild(applyBtn);
    buttonMenu->setScale(0.75f);
    buttonMenu->setContentWidth(m_valInput->getContentWidth() / buttonMenu->getScale());
    buttonMenu->setLayout(geode::RowLayout::create());
    buttonMenu->updateLayout();

    auto columnContainer = cocos2d::CCNode::create();
    columnContainer->addChild(titleLabel);
    if (m_keyInput) columnContainer->addChild(m_keyInput);
    columnContainer->addChild(m_valInput);
    columnContainer->addChild(buttonMenu);
    columnContainer->setContentWidth(m_valInput->getScaledContentWidth() + 10.0f);
    columnContainer->setLayout(
        geode::ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(false)
            ->setAutoGrowAxis(true)
            ->setGrowCrossAxis(true)
            ->setGap(15.0f)
    );
    columnContainer->updateLayout();

    if (!geode::Popup::init(columnContainer->getContentSize() + ccp(10.0f, 25.0f))) return false;

    columnContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    columnContainer->setPosition(m_mainLayer->getContentSize() / 2);
    m_mainLayer->addChild(columnContainer);

    return true;

}

geode::Result<std::string> PropEditor::getKeyString() {
    if (m_propKey.has_value()) {
        return geode::Ok(m_propKey.value().getString());
    }
    if (m_keyInput) {
        return geode::Ok(
            geode::utils::string::trim(
                m_keyInput->getString()
            )
        );
    }
    return geode::Err("could not get property editor key string");
}

PropEditor* PropEditor::create(std::optional<PropKey> propKey, GameObjectWrapper objectWrapper) {
    auto editor = new PropEditor(propKey, objectWrapper);
    if (editor && editor->init(ccp(400.0f, 130.0f))) {
        editor->autorelease();
        return editor;
    }
    CC_SAFE_DELETE(editor);
    return nullptr;
}

void PropEditor::onCancel(cocos2d::CCObject* sender) {
    this->onClose(nullptr);
}

void PropEditor::onApply(cocos2d::CCObject* sender) {
    std::string valStr = m_valInput->getString();
    valStr = geode::utils::string::trim(valStr);
    GEODE_UNWRAP_OR_ELSE(keyStr, err, this->getKeyString()) {
        geode::log::error("{}", err);
        return this->onClose(nullptr);
    }
    if (keyStr.empty()) {
        geode::createQuickPopup(
            "Apply Failed",
            "Please enter a valid key.",
            "OK", nullptr,
            [](auto, auto) { }
        );
        return;
    }
    if (auto popup = InspectorPopup::get()) {
        popup->setProperty(keyStr, valStr);
    }
    this->onClose(nullptr);
}

void PropEditor::onClose(cocos2d::CCObject* sender) {
    geode::Popup::onClose(sender);
}