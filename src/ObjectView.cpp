#include "ObjectView.hpp"

#include "InspectorPopup.hpp"

bool ObjectView::init(float size, ObjectSelection* selection, LevelEditorLayer* editorLayer) {

    m_selection = selection;
    m_editorLayer = editorLayer;

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentWidth(200.f);

    m_window = Border::create(nullptr, ccColor4B{ 0, 0, 0, 0 }, CCSize(size, size));
    if (!m_window) return false;
    m_window->setAnchorPoint(ccp(0.5f, 0.5f));
    m_window->ignoreAnchorPointForPosition(false);
    this->addChild(m_window);

    float buttonWidth = 15.f;
    float gapWidth = 5.f;
    float menuWidth = 2 * buttonWidth + gapWidth;

    auto buttons = CCMenu::create();
    buttons->setContentWidth(menuWidth);
    buttons->setContentHeight(m_window->getContentHeight());

    std::stringstream indexLabelStream;
    indexLabelStream << m_selection->getCurrentIndex() << "/" << m_selection->getCount();
    std::string indexLabelStr = indexLabelStream.str();
    m_indexLabel = CCLabelBMFont::create(indexLabelStr.c_str(), "chatFont.fnt");
    m_indexLabel->limitLabelWidth(menuWidth, 1.f, 0.1f);

    auto indexLabelContainer = CCNode::create();
    indexLabelContainer->setContentWidth(menuWidth);
    indexLabelContainer->setContentHeight(m_indexLabel->getContentHeight());
    m_indexLabel->setAnchorPoint(ccp(0.5f, 0.5f));
    m_indexLabel->setPosition(ccp(indexLabelContainer->getContentWidth() / 2.f, indexLabelContainer->getContentHeight() / 2.f));
    indexLabelContainer->addChild(m_indexLabel);

    buttons->addChild(indexLabelContainer);

    auto arrowPrevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowPrev = CCMenuItemSpriteExtra::create(arrowPrevSprite, this, menu_selector(ObjectView::selectPrevOrNext));
    buttons->addChild(arrowPrev);

    auto arrowNextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowNext = CCMenuItemSpriteExtra::create(arrowNextSprite, this, menu_selector(ObjectView::selectPrevOrNext));
    buttons->addChild(arrowNext);

    auto zoomOutSprite = CCSprite::createWithSpriteFrameName("GJ_zoomOutBtn_001.png");
    auto zoomOut = CCMenuItemSpriteExtra::create(zoomOutSprite, nullptr, nullptr);
    buttons->addChild(zoomOut);

    auto zoomInSprite = CCSprite::createWithSpriteFrameName("GJ_zoomInBtn_001.png");
    auto zoomIn = CCMenuItemSpriteExtra::create(zoomInSprite, nullptr, nullptr);
    buttons->addChild(zoomIn);

    auto focusSprite = CircleButtonSprite::createWithSprite("focus.png"_spr, 0.85f);
    auto focus = CCMenuItemSpriteExtra::create(focusSprite, nullptr, nullptr);
    buttons->addChild(focus);

    arrowPrevSprite->setScale(buttonWidth / arrowPrev->getContentWidth());
    arrowNextSprite->setScale(buttonWidth / arrowNext->getContentWidth());
    zoomOutSprite->setScale(buttonWidth / zoomOut->getContentWidth());
    zoomInSprite->setScale(buttonWidth / zoomIn->getContentWidth());
    focusSprite->setScale(buttonWidth / focus->getContentWidth());

    arrowPrev->setContentSize(arrowPrevSprite->getScaledContentSize());
    arrowNext->setContentSize(arrowNextSprite->getScaledContentSize());
    zoomOut->setContentSize(zoomOutSprite->getScaledContentSize());
    zoomIn->setContentSize(zoomInSprite->getScaledContentSize());
    focus->setContentSize(focusSprite->getScaledContentSize());

    arrowPrevSprite->setPosition(ccp(0.f, 0.f));
    arrowNextSprite->setPosition(ccp(0.f, 0.f));
    zoomOutSprite->setPosition(ccp(0.f, 0.f));
    zoomInSprite->setPosition(ccp(0.f, 0.f));
    focusSprite->setPosition(ccp(0.f, 0.f));

    arrowPrevSprite->setAnchorPoint(ccp(0.f, 0.f));
    arrowNextSprite->setAnchorPoint(ccp(1.f, 0.f));
    zoomOutSprite->setAnchorPoint(ccp(0.f, 0.f));
    zoomInSprite->setAnchorPoint(ccp(0.f, 0.f));
    focusSprite->setAnchorPoint(ccp(0.f, 0.f));
    
    arrowNextSprite->setScaleY(-arrowNextSprite->getScaleY());
    arrowNextSprite->setRotation(180.f);

    arrowPrev->setTag(0);
    arrowNext->setTag(1);

    this->addChild(buttons);

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

    layout = RowLayout::create();
    layout->setAutoScale(false);
    this->setLayout(layout);
    this->updateLayout();

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

void ObjectView::selectPrevOrNext(CCObject* obj) {
    obj->getTag() ? m_selection->selectNext() : m_selection->selectPrev();
    this->updateIndexLabel();
    this->focusObject();
    InspectorPopup::get()->setObject(m_selection->getSelectedObject());
}

ObjectView* ObjectView::create(float size, ObjectSelection* selection, LevelEditorLayer* editorLayer) {
    auto* obj = new ObjectView();
    if (obj->init(size, selection, editorLayer)) {
        obj->autorelease();
        return obj;
    }
    delete obj;
    return nullptr;
}

void ObjectView::focusObject() {

    GameObject* object = m_selection->getSelectedObject();
    CCLayer* objLayer = static_cast<GJBaseGameLayer*>(m_editorLayer)->m_objectLayer;
    CCNode* objLayerParent = objLayer->getParent();
    CCPoint objPos = objLayerParent->convertToNodeSpace(objLayer->convertToWorldSpace(object->getPosition()));
    CCSize editLayerSize = m_editorLayer->getContentSize();
    CCRect rect = m_window->boundingBox();
    CCPoint moveTo = objLayerParent->convertToNodeSpace(m_window->convertToWorldSpace(
        ccp(m_window->getContentWidth() / 2, m_window->getContentHeight() / 2)
    ));
    objLayer->setPosition(objLayer->getPosition() - objPos + moveTo);

}
