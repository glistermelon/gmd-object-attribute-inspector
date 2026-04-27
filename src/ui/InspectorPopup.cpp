#include "InspectorPopup.hpp"

#include "PropRow.hpp"
#include "PropEditor.hpp"

#include "colors.hpp"
#include "geometry.hpp"

inline std::string getIndentString(int indentLevel) {
    std::string indent = "";
    for (int i = 0; i <= indentLevel; ++i) indent.append("   ");
    return indent;
}

std::string getFormattedJsonDump(matjson::Value json, int indentLevel = 0) {

    if (json.isString()) {
        return fmt::format("<cg>{}</c>", json.dump());
    }
    else if (json.isNumber()) {
        return fmt::format("<cr>{}</c>", json.dump());
    }
    else if (json.isBool()) {
        return fmt::format("<cj>{}</c>", json.dump());
    }
    else if (json.isNull()) {
        return fmt::format("<cb>{}</c>", "null");
    }
    else if (json.isArray()) {
        std::vector<matjson::Value> values = json.asArray().unwrap();
        std::vector<std::string> strings;
        for (auto value : values) {
            strings.push_back(getFormattedJsonDump(value, indentLevel + 1));
        }
        return fmt::format("[{}]", geode::utils::string::join(strings, ", "));
    }
    else if (!json.isObject()) {
        // mysterious mystery json I guess
        return json.dump();
    }

    std::vector<std::string> strings;
    std::string indent = getIndentString(indentLevel);
    for (auto [key, value] : json) {
        std::stringstream ss;
        ss << indent;
        ss << "\"" << key << "\": ";
        ss << getFormattedJsonDump(value, indentLevel + 1);
        strings.push_back(ss.str());
    }
    return fmt::format(
        "{{\n{}\n{}}}",
        geode::utils::string::join(strings, ",\n"),
        getIndentString(indentLevel - 1)
    );

}

constexpr cocos2d::CCSize outerPadding = cocos2d::CCSize(50.0f, 50.0f);
constexpr cocos2d::CCSize innerPadding = cocos2d::CCSize(10.0f, 10.0f);

CCMenuItemToggler* InspectorPopup::addViewTypeButton(int index, std::string name) {

    const float yPos = 20.f * index + 4.0f;

    auto button = geode::cocos::CCMenuItemExt::createTogglerWithStandardSprites(
        0.5f, std::bind(&InspectorPopup::onSelectViewTypeButton, this, std::placeholders::_1)
    );
    button->ignoreAnchorPointForPosition(false);
    button->setAnchorPoint(cocos2d::CCPointZero);
    button->setPosition(ccp(0.0f, yPos));
    m_selViewTypeMenu->addChild(button);

    auto label = cocos2d::CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint(cocos2d::CCPointZero);
    label->setPosition(ccp(button->getScaledContentWidth() + 5.0f, yPos));
    m_selViewTypeMenu->addChild(label);

    return button;

}

void InspectorPopup::onSelectViewTypeButton(CCMenuItemToggler* button) {
    for (auto other : { m_minViewBtn, m_jsonViewBtn, m_rawViewBtn, m_defaultViewBtn }) {
        if (other != button && other->isOn()) {
            other->toggle(false);
            other->setEnabled(true);
        }
        button->setEnabled(false);
        button->toggle(true);
    }
    this->refresh();
}

void InspectorPopup::replaceWithClipNode(cocos2d::CCNode* node) {

    auto clipNode = cocos2d::CCClippingNode::create(cocos2d::CCNode::create());
    clipNode->setContentSize(node->getContentSize());
    clipNode->setAnchorPoint(node->getAnchorPoint());
    clipNode->setPosition(node->getPosition());
    clipNode->setZOrder(node->getZOrder());
    node->getParent()->addChild(clipNode);
    node->removeFromParent();
    clipNode->addChild(node);
    node->setAnchorPoint(cocos2d::CCPointZero);
    node->setPosition(cocos2d::CCPointZero);

    auto stencil = cocos2d::CCDrawNode::create();
    stencil->ignoreAnchorPointForPosition(true);
    stencil->setContentSize(clipNode->getContentSize());
    auto rect = getRectInNode(clipNode, m_objectViewPanel);
    stencil->drawRect(
        ccp(rect.getMinX() + 1.f, rect.getMinY() + 1.f),
        ccp(rect.getMaxX() - 1.f, rect.getMaxY() - 1.f),
        cocos2d::ccColor4F{ 0.f, 0.f, 0.f, 1.f }, 0.f, cocos2d::ccColor4F{ 0.f, 0.f, 0.f, 0.f }
    );
    clipNode->setStencil(stencil);
    clipNode->setInverted(true);

}

void InspectorPopup::resetPropViewContainer() {
    m_propViewContainer->removeAllChildren();
    m_propViewContainer->addChild(m_propViewBorder);
    m_propViewContainer->addChild(m_propViewBackground);
    m_propViewContainer->addChild(m_propViewMenu);
}

bool InspectorPopup::init(std::vector<GameObject*> objects) {

    if (objects.empty()) return false;

    if (!geode::Popup::init(cocos2d::CCDirector::sharedDirector()->getWinSize() - outerPadding)) {
        return false;
    }

    // Add object wrappers

    for (auto object : objects) {
        m_objects.emplace_back(object);
    }
    m_selectedObjectIndex = 0;

    // Set up the property viewer area

    m_propViewContainer = cocos2d::CCNode::create();
    if (!m_propViewContainer) return false;
    m_propViewContainer->setAnchorPoint(ccp(1.0f, 0.0f));
    m_propViewContainer->ignoreAnchorPointForPosition(false);
    m_propViewContainer->setPosition(ccp(m_mainLayer->getContentWidth() - innerPadding.width, innerPadding.height));
    m_propViewContainer->setContentSize(ccp(propRowSize.width, m_mainLayer->getContentHeight() - 2 * innerPadding.height));
    m_propViewContainer->setZOrder(1);
    m_mainLayer->addChild(m_propViewContainer);

    m_propViewBorder = geode::Border::create(nullptr, { 0, 0, 0, 0 }, m_propViewContainer->getContentSize());
    if (!m_propViewBorder) return false;
    m_propViewBorder->setPosition(cocos2d::CCPointZero);
    m_propViewBorder->setAnchorPoint(cocos2d::CCPointZero);
    m_propViewBorder->setZOrder(2);

    m_propViewBackground = cocos2d::CCLayerColor::create();
    if (!m_propViewBackground) return false;
    m_propViewBackground->setPosition(cocos2d::CCPointZero);
    m_propViewBackground->setContentSize(m_propViewContainer->getContentSize());
    m_propViewBackground->setAnchorPoint(cocos2d::CCPointZero);
    m_propViewBackground->setZOrder(0);
    m_propViewBackground->setOpacity(255);

    // Add menu for overlay buttons in property viewer

    m_propViewMenu = cocos2d::CCMenu::create();
    if (!m_propViewMenu) return false;
    m_propViewMenu->setAnchorPoint(cocos2d::CCPointZero);
    m_propViewMenu->setPosition(cocos2d::CCPointZero);
    m_propViewMenu->setContentSize(m_propViewContainer->getContentSize());
    m_propViewMenu->setZOrder(5);
    m_propViewMenu->setTouchPriority(-510);
    m_propViewContainer->addChild(m_propViewMenu);

    // Add copy button

    auto copySprite = cocos2d::CCSprite::create("copy.png"_spr);
    if (!copySprite) return false;
    copySprite->setScale(0.6f);
    m_copyBtn = CCMenuItemSpriteExtra::create(copySprite, this, menu_selector(InspectorPopup::onCopy));
    if (!m_copyBtn) return false;
    m_copyBtn->setPosition(ccp(
            m_propViewContainer->getContentWidth() - 5.0f - m_copyBtn->getContentWidth() / 2,
            4.0f + m_copyBtn->getContentHeight() / 2
    ));
    m_propViewMenu->addChild(m_copyBtn);

    // Add new property button GJ_GameSheet03-uhd.png

    auto newPropSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    if (!newPropSprite) return false;
    newPropSprite->setScale(0.7f);
    m_newPropBtn = CCMenuItemSpriteExtra::create(newPropSprite, this, menu_selector(InspectorPopup::onAddProperty));
    if (!m_newPropBtn) return false;
    m_newPropBtn->setPosition(ccp(
            m_propViewContainer->getContentWidth() - 5.0f - m_newPropBtn->getContentWidth() / 2,
            4.0f + m_newPropBtn->getContentHeight() / 2
    ));
    m_propViewMenu->addChild(m_newPropBtn);

    // Add object viewer

    float objectViewPanelSize = std::min(
        m_mainLayer->getContentWidth()
            - m_propViewContainer->getContentWidth()
            - innerPadding.width * 3,
        m_mainLayer->getContentHeight() / 2
    );
    m_objectViewPanel = ObjectViewPanel::create(objectViewPanelSize);
    if (!m_objectViewPanel) return false;
    m_objectViewPanel->setAnchorPoint(ccp(0.5f, 1.0f));
    m_objectViewPanel->setPosition(ccp(
        (m_mainLayer->getContentWidth() - m_propViewContainer->getContentWidth()) / 2,
        m_mainLayer->getContentHeight() - innerPadding.height - 4.0f
    ));
    m_mainLayer->addChild(m_objectViewPanel);

    // Setup clipping and stencil nodes so we can see through the popup in the object view panel

    this->replaceWithClipNode(m_bgSprite);

    this->setOpacity(0);
    auto popupBackground = CCLayerColor::create(cocos2d::ccColor4B { 0, 0, 0, 105 });
    if (!popupBackground) return false;
    popupBackground->setAnchorPoint(cocos2d::CCPointZero);
    popupBackground->setPosition(cocos2d::CCPointZero);
    popupBackground->setContentSize(this->getContentSize());
    popupBackground->setZOrder(-1);
    this->addChild(popupBackground);
    this->replaceWithClipNode(popupBackground);

    // Add buttons for managing which object is selected

    const float objSelButtonScale = 0.7f;

    auto objSelMenu = cocos2d::CCMenu::create();
    if (!objSelMenu) return false;
    objSelMenu->setContentWidth(objectViewPanelSize);
    // menu->setContentHeight(m_window->getContentHeight());
    m_mainLayer->addChild(objSelMenu);

    auto arrowPrevSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    if (!arrowPrevSprite) return false;
    arrowPrevSprite->setScale(objSelButtonScale);
    auto arrowPrevBtn = CCMenuItemSpriteExtra::create(arrowPrevSprite, this, menu_selector(InspectorPopup::onPrevObject));
    if (!arrowPrevBtn) return false;
    objSelMenu->addChild(arrowPrevBtn);

    m_indexLabel = cocos2d::CCLabelBMFont::create("", "chatFont.fnt");
    if (!m_indexLabel) return false;

    auto indexLabelContainer = cocos2d::CCNode::create();
    if (!indexLabelContainer) return false;
    indexLabelContainer->setContentWidth(50.0f);
    indexLabelContainer->setContentHeight(m_indexLabel->getContentHeight());
    indexLabelContainer->addChild(m_indexLabel);
    objSelMenu->addChild(indexLabelContainer);

    auto arrowNextSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    if (!arrowNextSprite) return false;
    arrowNextSprite->setFlipX(true);
    arrowNextSprite->setScale(objSelButtonScale);
    auto arrowNextBtn = CCMenuItemSpriteExtra::create(arrowNextSprite, this, menu_selector(InspectorPopup::onNextObject));
    if (!arrowNextBtn) return false;
    objSelMenu->addChild(arrowNextBtn);

    auto zoomOutSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");
    if (!zoomOutSprite) return false;
    zoomOutSprite->setScale(objSelButtonScale);
    auto zoomOutBtn = CCMenuItemSpriteExtra::create(zoomOutSprite, this, menu_selector(InspectorPopup::onZoomOut));
    if (!zoomOutBtn) return false;
    objSelMenu->addChild(zoomOutBtn);

    auto zoomInSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
    if (!zoomInSprite) return false;
    zoomInSprite->setScale(objSelButtonScale);
    auto zoomInBtn = CCMenuItemSpriteExtra::create(zoomInSprite, this, menu_selector(InspectorPopup::onZoomIn));
    if (!zoomInBtn) return false;
    objSelMenu->addChild(zoomInBtn);

    m_indexLabel->limitLabelWidth(indexLabelContainer->getContentWidth(), 1.f, 0.1f);
    m_indexLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    m_indexLabel->setPosition(ccp(indexLabelContainer->getContentWidth() / 2.f, indexLabelContainer->getContentHeight() / 2.f));

    auto objSelMenuLayout = geode::RowLayout::create();
    objSelMenuLayout->setCrossAxisReverse(true);
    objSelMenuLayout->setAxisAlignment(geode::AxisAlignment::Center);
    objSelMenuLayout->setCrossAxisAlignment(geode::AxisAlignment::Center);
    objSelMenuLayout->setCrossAxisLineAlignment(geode::AxisAlignment::Center);
    objSelMenuLayout->setAutoScale(false);
    objSelMenuLayout->setGrowCrossAxis(false);
    objSelMenuLayout->setCrossAxisOverflow(false);
    objSelMenuLayout->setAutoGrowAxis(true);
    objSelMenuLayout->setGap(5.0f);
    objSelMenu->setLayout(objSelMenuLayout);
    objSelMenu->setContentHeight(40.0f);
    objSelMenu->setAnchorPoint(ccp(0.5f, 1.0f));
    objSelMenu->setPosition(ccp(
        m_objectViewPanel->getPositionX(),
        m_objectViewPanel->getPositionY()
            - m_objectViewPanel->getContentHeight()
    ));
    objSelMenu->updateLayout();
    objSelMenu->setScale(m_objectViewPanel->getContentWidth() / objSelMenu->getContentWidth());

    // Add view type selection buttons

    m_selViewTypeMenu = cocos2d::CCMenu::create();
    if (!m_selViewTypeMenu) return false;
    m_selViewTypeMenu->ignoreAnchorPointForPosition(false);
    m_selViewTypeMenu->setAnchorPoint(ccp(0.5f, 0.0f));
    m_selViewTypeMenu->setPosition(ccp(
        m_objectViewPanel->getPositionX(),
        innerPadding.height - 3.0f
    ));
    m_selViewTypeMenu->setContentSize(ccp(
        m_objectViewPanel->getContentWidth(),
        m_mainLayer->getContentHeight()
    ));
    m_mainLayer->addChild(m_selViewTypeMenu);

    m_minViewBtn = this->addViewTypeButton(0, "Minimal Info");
    m_rawViewBtn = this->addViewTypeButton(1, "Raw");
    m_jsonViewBtn = this->addViewTypeButton(2, "JSON");
    m_defaultViewBtn = this->addViewTypeButton(3, "Detailed");

    // Hide the object info label because it can get into the object view

    auto objectInfoLabel = EditorUI::get()->m_objectInfoLabel;
    if (objectInfoLabel) objectInfoLabel->setVisible(false);

    // Start with default view

    this->onSelectViewTypeButton(m_defaultViewBtn);

    // Success

    return true;

}

void InspectorPopup::onClose(cocos2d::CCObject* sender) {

    geode::Popup::onClose(sender);
    s_activePopup = nullptr;

    // reload the object info label as naturally as possible
    EditorUI::get()->toggleObjectInfoLabel();
    EditorUI::get()->toggleObjectInfoLabel();

}

InspectorPopup* InspectorPopup::create(std::vector<GameObject*> objects) {
    if (InspectorPopup::get()) return nullptr;
    auto popup = new InspectorPopup();
    if (popup->init(objects)) {
        popup->autorelease();
        s_activePopup = popup;
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

InspectorPopup* InspectorPopup::get() {
    return s_activePopup;
}

void InspectorPopup::refreshTextView(std::string text) {

    this->resetPropViewContainer();

    auto textArea = TextArea::create(
        text,
        "UbuntuMono.fnt"_spr,
        1.0f,
        m_propViewContainer->getContentWidth(),
        cocos2d::CCPointZero,
        20.0f,
        false
    );
    textArea->setScale(0.7f);
    textArea->setAnchorPoint(cocos2d::CCPointZero);
    textArea->setPosition(ccp(7.0f, 10.0f));

    m_textView = geode::ScrollLayer::create(m_propViewContainer->getContentSize(), true, true);
    m_textView->setAnchorPoint(cocos2d::CCPointZero);
    m_textView->setPosition(cocos2d::CCPointZero);

    m_textView->m_contentLayer->setContentSize(ccp(m_propViewContainer->getContentWidth(), textArea->getScaledContentHeight() + 25.0f));
    m_textView->m_contentLayer->addChild(textArea);
    m_textView->scrollToTop();

    m_propViewContainer->addChild(m_textView);

}

void InspectorPopup::refreshDefaultView() {

    this->resetPropViewContainer();

    auto rows = cocos2d::CCArray::create();
    auto& selectedObjectWrapper = m_objects[m_selectedObjectIndex];
    bool oddRow = false;
    for (auto key : selectedObjectWrapper.getPropKeys()) {
        auto row = PropRow::create(oddRow, key, selectedObjectWrapper);
        if (row) {
            rows->addObject(row);
            oddRow = !oddRow;
        }
        else {
            geode::log::warn("Failed to add PropRow (key {})", key.getString());
        }
    }

    m_propListView = geode::ListView::create(
        rows,
        propRowSize.height,
        m_propViewContainer->getContentWidth(),
        m_propViewContainer->getContentHeight()
    );
    if (!m_propListView) {
        geode::log::error("Failed to create property list view");
        return;
    }
    m_propListView->setPosition(cocos2d::CCPointZero);
    m_propListView->setAnchorPoint(cocos2d::CCPointZero);
    m_propListView->setZOrder(1);
    m_propListView->setPrimaryCellColor(LIGHTER_BROWN_3B);
    m_propListView->setSecondaryCellColor(LIGHT_BROWN_3B);

    m_propViewContainer->addChild(m_propListView);

    m_propViewBackground->setColor(LIGHT_BROWN_3B);

}

template <class T>
std::vector<T*> getChildrenByTypeRecursive(cocos2d::CCNode* node) {
    std::vector<T*> matches;
    for (auto child : node->getChildrenExt()) {
        auto casted = geode::cast::typeinfo_cast<T*>(child);
        if (casted) {
            matches.push_back(casted);
        }
        matches.append_range(getChildrenByTypeRecursive<T>(child));
    }
    return matches;
}

void InspectorPopup::refreshJsonView() {

    auto json = m_objects[m_selectedObjectIndex].getPropMapJson();

    this->refreshTextView(
        getFormattedJsonDump(json)
    );

    m_propViewBackground->setColor({ 50, 50, 50 });

    m_copyStr = json.dump();

}

void InspectorPopup::refreshRawView() {

    std::string objectStr = m_objects[m_selectedObjectIndex].getObjectString();

    this->refreshTextView(objectStr);

    m_propViewBackground->setColor(DARK_BROWN_3B);

    m_copyStr = objectStr;

}

void InspectorPopup::refreshMinView() {

    auto editorUI = EditorUI::get();
    editorUI->updateObjectInfoLabel();
    std::string infoStr;
    if (editorUI->m_objectInfoLabel) {  
        infoStr = editorUI->m_objectInfoLabel->getString();
    }
    else {
        infoStr = "Error";
        geode::log::error("Failed to get object info label string");
    }

    this->refreshTextView(infoStr);

    m_propViewBackground->setColor(DARK_BROWN_3B);

    m_copyStr = infoStr;

}

void InspectorPopup::refresh() {

    if (m_defaultViewBtn->isToggled()) {
        this->refreshDefaultView();
    }
    else if (m_jsonViewBtn->isToggled()) {
        this->refreshJsonView();
    }
    else if (m_rawViewBtn->isToggled()) {
        this->refreshRawView();
    }
    else if (m_minViewBtn->isToggled()) {
        this->refreshMinView();
    }

    m_copyBtn->setVisible(!m_defaultViewBtn->isToggled());
    m_newPropBtn->setVisible(m_defaultViewBtn->isToggled());

    m_objectViewPanel->refresh(m_objects[m_selectedObjectIndex].getGameObject());

    m_indexLabel->setString(
        fmt::format("{}/{}", m_selectedObjectIndex + 1, m_objects.size())
            .c_str()
    );

}

void InspectorPopup::onCopy(cocos2d::CCObject* sender) {
    geode::utils::clipboard::write(m_copyStr);
}

void InspectorPopup::onNextObject(cocos2d::CCObject* sender) {
    if (++m_selectedObjectIndex >= m_objects.size()) m_selectedObjectIndex = 0;
    this->refresh();
}

void InspectorPopup::onPrevObject(cocos2d::CCObject* sender) {
    m_selectedObjectIndex = m_selectedObjectIndex == 0
        ? m_objects.size() - 1
        : m_selectedObjectIndex - 1;
    this->refresh();
}

void InspectorPopup::onZoomIn(cocos2d::CCObject* sender) {
    EditorUI::get()->zoomIn(nullptr);
    m_objectViewPanel->refresh(m_objects[m_selectedObjectIndex].getGameObject());
}

void InspectorPopup::onZoomOut(cocos2d::CCObject* sender) {
    EditorUI::get()->zoomOut(nullptr);
    m_objectViewPanel->refresh(m_objects[m_selectedObjectIndex].getGameObject());
}

void InspectorPopup::onAddProperty(cocos2d::CCObject* sender) {
    auto editor = PropEditor::create(std::nullopt, m_objects[m_selectedObjectIndex]);
    if (editor) {
        editor->show();
    }
    else {
        geode::log::error("Failed to open editor to add new property");
    }
}

void InspectorPopup::setProperty(std::string setKey, std::string setVal) {
    this->handlePropUpdateResult(
        m_objects[m_selectedObjectIndex].setProp(setKey, setVal)
    );
}

void InspectorPopup::deleteProperty(std::string key) {
    this->handlePropUpdateResult(
        m_objects[m_selectedObjectIndex].deleteProp(key)
    );
}

void InspectorPopup::handlePropUpdateResult(geode::Result<> result) {
    if (result.isErr()) {
        geode::createQuickPopup(
            "Apply Failed",
            "Your changes could not be applied.",
            "OK", nullptr,
            [](auto, auto) { }
        );
    }
    else {
        this->refresh();
    }    
}