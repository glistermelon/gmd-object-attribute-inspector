#include "ObjectView.hpp"

#include "InspectorPopup.hpp"

bool ObjectView::init(ObjectSelection* selection, float size) {

    m_selection = selection;

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentWidth(200.f); // random large number because it uses a layout about the center anyway

    // Add object viewing window

    m_window = Border::create(nullptr, ccColor4B{ 0, 0, 0, 0 }, CCSize(size, size));
    if (!m_window) return false;
    m_window->setAnchorPoint(ccp(0.5f, 0.5f));
    m_window->ignoreAnchorPointForPosition(false);
    this->addChild(m_window);

    // Add control buttons to the side of the window

    constexpr float buttonWidth = 15.f;
    constexpr float gapWidth = 5.f;
    constexpr float menuWidth = 2 * buttonWidth + gapWidth;

    auto buttons = CCMenu::create();
    buttons->setContentWidth(menuWidth);
    buttons->setContentHeight(m_window->getContentHeight());
    this->addChild(buttons);

    m_indexLabel = CCLabelBMFont::create("X", "chatFont.fnt"); // IIRC, the string being empty caused problems so this is a placeholder value
    m_indexLabel->limitLabelWidth(menuWidth, 1.f, 0.1f);

    auto indexLabelContainer = CCNode::create();
    indexLabelContainer->setContentWidth(menuWidth);
    indexLabelContainer->setContentHeight(m_indexLabel->getContentHeight());
    m_indexLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    m_indexLabel->setPosition(ccp(indexLabelContainer->getContentWidth() / 2.f, indexLabelContainer->getContentHeight() / 2.f));
    indexLabelContainer->addChild(m_indexLabel);

    buttons->addChild(indexLabelContainer);

    auto arrowPrevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowPrev = CCMenuItemSpriteExtra::create(arrowPrevSprite, this, menu_selector(InspectorPopup::selectPrevOrNext));
    buttons->addChild(arrowPrev);

    auto arrowNextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowNext = CCMenuItemSpriteExtra::create(arrowNextSprite, this, menu_selector(InspectorPopup::selectPrevOrNext));
    buttons->addChild(arrowNext);

    auto zoomOutSprite = CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");
    auto zoomOut = CCMenuItemSpriteExtra::create(zoomOutSprite, this, menu_selector(ObjectView::onZoomOut));
    buttons->addChild(zoomOut);

    auto zoomInSprite = CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
    auto zoomIn = CCMenuItemSpriteExtra::create(zoomInSprite, this, menu_selector(ObjectView::onZoomIn));
    buttons->addChild(zoomIn);

    constexpr int numButtons = 4;
    std::array<CCSprite*, numButtons> sprites = { arrowPrevSprite, arrowNextSprite, zoomOutSprite, zoomInSprite };
    std::array<CCMenuItemSpriteExtra*, numButtons> buttonItems = { arrowPrev, arrowNext, zoomOut, zoomIn };
    for (int i = 0; i < numButtons; ++i) {
        sprites[i]->setScale(buttonWidth / buttonItems[i]->getContentWidth());
        buttonItems[i]->setContentSize(sprites[i]->getScaledContentSize());
        sprites[i]->setPosition(CCPointZero);
        sprites[i]->setAnchorPoint(CCPointZero);
    }

    // Flip the "next" arrow around
    arrowNextSprite->setAnchorPoint(ccp(1.f, 0.f));
    arrowNextSprite->setScaleY(-arrowNextSprite->getScaleY());
    arrowNextSprite->setRotation(180.f);

    // Tags for selection callback
    arrowPrev->setTag(0);
    arrowNext->setTag(1);

    // Set up the layout for the buttons

    auto layout = RowLayout::create();
    layout->setCrossAxisReverse(true);
    layout->setAxisAlignment(AxisAlignment::Start);
    layout->setCrossAxisAlignment(AxisAlignment::Start);
    layout->setCrossAxisLineAlignment(AxisAlignment::Center);
    layout->setGrowCrossAxis(true);
    layout->setCrossAxisOverflow(true);
    layout->setAutoScale(false);
    layout->setGap(gapWidth);
    buttons->setLayout(layout);
    buttons->updateLayout();

    // Set up the top-level layout

    layout = RowLayout::create();
    layout->setAutoScale(false);
    this->setLayout(layout);
    this->updateLayout();

    // Initialize the index label
    this->updateIndexLabel();

    return true;

}

void ObjectView::updateIndexLabel() {
    std::stringstream ss;
    ss << (m_selection->getCurrentIndex() + 1);
    ss << "/";
    ss << m_selection->getCount();
    std::string s = ss.str();
    m_indexLabel->setString(s.c_str());
}

ObjectView* ObjectView::create(ObjectSelection* selection, float size) {
    auto* obj = new ObjectView();
    if (obj->init(selection, size)) {
        obj->autorelease();
        return obj;
    }
    delete obj;
    return nullptr;
}

void ObjectView::focusObject() {

    // TODO: use those util functions from include.hpp
    GameObject* object = m_selection->getSelectedObject()->getGameObject();
    CCLayer* objLayer = LevelEditorLayer::get()->m_objectLayer;
    CCNode* objLayerParent = objLayer->getParent();
    CCPoint objPos = objLayerParent->convertToNodeSpace(objLayer->convertToWorldSpace(object->getPosition()));
    CCSize editLayerSize = LevelEditorLayer::get()->getContentSize();
    CCRect rect = m_window->boundingBox();
    CCPoint moveTo = objLayerParent->convertToNodeSpace(m_window->convertToWorldSpace(
        ccp(m_window->getContentWidth() / 2, m_window->getContentHeight() / 2)
    ));
    objLayer->setPosition(objLayer->getPosition() - objPos + moveTo);

}

void ObjectView::onZoomIn(CCObject* sender) {
    LevelEditorLayer::get()->m_editorUI->zoomIn(sender);
    this->focusObject();
}

void ObjectView::onZoomOut(CCObject* sender) {
    LevelEditorLayer::get()->m_editorUI->zoomOut(sender);
    this->focusObject();
}