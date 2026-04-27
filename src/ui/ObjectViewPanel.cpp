#include "ObjectViewPanel.hpp"

#include "geometry.hpp"

bool ObjectViewPanel::init(float size) {

    this->setContentSize(ccp(size, size));

    m_window = geode::Border::create(nullptr, cocos2d::ccColor4B{ 0, 0, 0, 0 }, ccp(size, size));
    if (!m_window) return false;
    m_window->setAnchorPoint(cocos2d::CCPointZero);
    m_window->setPosition(cocos2d::CCPointZero);
    m_window->ignoreAnchorPointForPosition(false);
    this->addChild(m_window);

    return true;

}

void ObjectViewPanel::focusObject(GameObject* object) {
    auto objectLayer = LevelEditorLayer::get()->m_objectLayer;
    auto batchLayer = LevelEditorLayer::get()->getChildByIDRecursive("batch-layer");
    auto objectLayerParent = objectLayer->getParent();
    auto curPos = objectLayerParent->convertToNodeSpace(batchLayer->convertToWorldSpace(object->getPosition()));
    auto targetRect = getRectInNode(objectLayerParent, m_window);
    auto targetPos = targetRect.origin + targetRect.size / 2;
    objectLayer->setPosition(objectLayer->getPosition() - curPos + targetPos);
}

ObjectViewPanel* ObjectViewPanel::create(float size) {
    auto* panel = new ObjectViewPanel();
    if (panel && panel->init(size)) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

cocos2d::CCNode* ObjectViewPanel::getWindow() {
    return m_window;
}

void ObjectViewPanel::refresh(GameObject* object) {
    this->focusObject(object);
}