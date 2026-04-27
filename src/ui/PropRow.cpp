#include "PropRow.hpp"

#include "InspectorPopup.hpp"
#include "PropEditor.hpp"

#include "colors.hpp"

#include <common/PropDoc.hpp>

constexpr const char* chatFont = "chatFont.fnt";

PropRow::PropRow(PropKey propKey, GameObjectWrapper objectWrapper)
    : geode::GenericListCell("", propRowSize),
    m_objectWrapper(objectWrapper),
    m_propKey(propKey) { }

bool PropRow::init(bool odd) {

    if (!geode::GenericListCell::init()) return false;

    m_mainLayer->setContentSize(propRowSize);

    auto keyLabel = cocos2d::CCLabelBMFont::create("", chatFont);
    if (!keyLabel) return false;
    keyLabel->setString(m_propKey.getString().c_str());

    auto valLabel = cocos2d::CCLabelBMFont::create("", chatFont);
    if (!valLabel) return false;
    std::optional<PropValue> maybeVal = m_objectWrapper.getPropValue(m_propKey);
    valLabel->setString(maybeVal.has_value () ? maybeVal.value().getPrettyString().c_str() : "Error");
    if (!maybeVal.has_value()) {
        geode::log::warn("Failed to get property value (key: {})", m_propKey.getString());
    }

    auto keySideLabel = cocos2d::CCLabelBMFont::create("Key:", chatFont);
    if (!keySideLabel) return false;
    keySideLabel->setScale(0.7f);

    auto valueSideLabel = cocos2d::CCLabelBMFont::create("Value:", chatFont);
    if (!valueSideLabel) return false;
    valueSideLabel->setScale(0.7f);

    const auto containerColor = odd ? LIGHTER_BROWN_3B : LIGHT_BROWN_3B;

    auto keyContainer = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    if (!keyContainer) return false;
    keyContainer->setContentSize(cocos2d::CCSize(keyLabel->getContentWidth() / 0.6f, 35.f));
    keyContainer->setScale(0.6f);
    keyContainer->setColor(containerColor);
    keyContainer->setContentWidth(57.0f);
    keyContainer->addChild(keyLabel);

    auto valContainer = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    if (!valContainer) return false;
    valContainer->setContentSize(cocos2d::CCSize(valLabel->getContentWidth() / 0.6f, 35.f));
    valContainer->setScale(0.6f);
    valContainer->setColor(containerColor);
    valContainer->setContentWidth(325.0f);
    valContainer->addChild(valLabel);

    keyLabel->limitLabelWidth(keyContainer->getContentWidth(), 1.f, 0.01f);
    keyLabel->setScale(1 / 0.6f);
    keyLabel->setPosition(keyContainer->getContentSize() / 2);

    valLabel->limitLabelWidth(valContainer->getContentWidth(), 1.f, 0.01f);
    valLabel->setScale(1 / 0.6f);
    valLabel->setPosition(valContainer->getContentSize() / 2);

    keySideLabel->setPosition(ccp(15.f, 16.f));
    keyContainer->setPosition(ccp(46.f, 16.f));

    valueSideLabel->setPosition(ccp(84.f, 16.f));
    valContainer->setPosition(ccp(198.f, 16.f));

    m_mainLayer->addChild(keySideLabel);
    m_mainLayer->addChild(keyContainer);
    m_mainLayer->addChild(valueSideLabel);
    m_mainLayer->addChild(valContainer);

    auto rightMenu = cocos2d::CCMenu::create();
    rightMenu->setContentWidth(100.f);
    rightMenu->setPosition(ccp(m_mainLayer->getContentWidth() - 6.0f, 31.0f));
    rightMenu->setAnchorPoint(ccp(1.0f, 0.0f));
    if (!rightMenu) return false;
    m_mainLayer->addChild(rightMenu);

    auto trashSprite = geode::EditorButtonSprite::createWithSprite("trash.png"_spr, 0.8f, geode::EditorBaseColor::DarkGray);
    trashSprite->setScale(0.7f);
    auto trashButton = CCMenuItemSpriteExtra::create(trashSprite, this, menu_selector(PropRow::onDelete));
    rightMenu->addChild(trashButton);

    auto editSprite = geode::EditorButtonSprite::createWithSprite("pencil.png"_spr, 0.8f, geode::EditorBaseColor::BrightGreen);
    editSprite->setScale(0.7f);
    auto editButton = CCMenuItemSpriteExtra::create(editSprite, this, menu_selector(PropRow::onEdit));
    rightMenu->addChild(editButton);

    auto rightMenuLayout = geode::RowLayout::create();
    rightMenuLayout->setAxisReverse(true);
    rightMenuLayout->setAxisAlignment(geode::AxisAlignment::End);
    rightMenuLayout->setAutoScale(false);
    rightMenu->setLayout(rightMenuLayout);
    rightMenu->updateLayout();

    auto maybePropDoc = PropDoc::getDoc(m_propKey);

    std::string propName = maybePropDoc.has_value () ? maybePropDoc.value().m_name : "Unknown Property";
    auto nameLabel = cocos2d::CCLabelBMFont::create(propName.c_str(), "bigFont.fnt");
    nameLabel->setAnchorPoint(ccp(0.f, 0.f));
    nameLabel->setPosition(ccp(5.f, 31.f));
    nameLabel->limitLabelWidth(175.f, 0.7f, 0.1f);
    m_mainLayer->addChild(nameLabel);


    if (maybePropDoc.has_value()) {

        auto propDoc = maybePropDoc.value();

        std::string typeShortName = getPropValueTypeShortString(propDoc.m_type);
        auto typeLabel = cocos2d::CCLabelBMFont::create(typeShortName.c_str(), "bigFont.fnt");
        typeLabel->setColor(getPropValueTypeColor(propDoc.m_type));
        typeLabel->setAnchorPoint(ccp(0.f, 0.f));
        typeLabel->setPosition(ccp(nameLabel->getPositionX() + nameLabel->getScaledContentWidth() + 2.f, nameLabel->getPositionY()));
        typeLabel->setScale(0.325f);
        m_mainLayer->addChild(typeLabel);

        if (propDoc.m_desc.has_value()) {

            auto infoMenu = cocos2d::CCMenu::create();
            infoMenu->setAnchorPoint(ccp(0.f, 0.f));
            infoMenu->setPosition(ccp(
                nameLabel->getPositionX() + nameLabel->getScaledContentWidth() + 2.f,
                nameLabel->getPositionY() + 9.f)
            );
            infoMenu->setScale(0.5f);
            m_mainLayer->addChild(infoMenu);

            auto infoBtn = InfoAlertButton::create(
                "Property Documentation", propDoc.m_desc.value().c_str(), 1.f
            );
            infoBtn->setPosition(infoBtn->getContentSize() / 2);
            infoMenu->addChild(infoBtn);
            infoMenu->setContentSize(infoBtn->getScaledContentSize());        

        }

    }

    return true;

}

PropRow* PropRow::create(bool odd, PropKey propKey, GameObjectWrapper objectWrapper) {
    auto propRow = new PropRow(propKey, objectWrapper);
    if (propRow && propRow->init(odd)) {
        propRow->autorelease();
        return propRow;
    }
    CC_SAFE_DELETE(propRow);
    return nullptr;
}

void PropRow::onEdit(cocos2d::CCObject* sender) {
    auto editor = PropEditor::create(m_propKey, m_objectWrapper);
    if (editor) {
        editor->show();
    }
    else {
        geode::log::error("Failed to open property editor (key {})", m_propKey.getString());
    }
}

void PropRow::onDelete(cocos2d::CCObject* sender) {
    if (auto popup = InspectorPopup::get()) {
        popup->deleteProperty(m_propKey.getString());
    }
}