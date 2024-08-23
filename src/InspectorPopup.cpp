#include "InspectorPopup.hpp"

#include "AttributeListing.hpp"
#include "ObjectSelection.hpp"
#include "ObjectView.hpp"

InspectorPopup* InspectorPopup::activePopup;

bool InspectorPopup::setup(ObjectSelection* objSelection, LevelEditorLayer* editorLayer) {

    InspectorPopup::activePopup = this;

    m_editorLayer = editorLayer;
    m_objSelection.reset(objSelection);

    auto* newParent = CCClippingNode::create(CCNode::create());
    newParent->setContentSize(CCScene::get()->getContentSize());
    for (auto* child : CCArrayExt<CCNode*>(this->getChildren())) {
        newParent->addChild(child);
        this->removeChild(child);
    }
    this->addChild(newParent);

    //auto* list = ObjectView::create();
    //if (!list) return false;
    //this->addChild(list);
    //list->setPosition(CCPoint(PADDING_HORIZONTAL, PADDING_VERTICAL));

    //auto* dataViewOpt = CCMenuItemToggle::create();

    CCPoint padding(PADDING_HORIZONTAL, PADDING_VERTICAL);

    auto* menu = CCMenu::create();
    if (!menu) return false;
    menu->setAnchorPoint(ccp(0.f, 0.f));
    menu->setPosition(padding);

    constexpr std::string_view buttonLabels[] = { "Basic Info", "Raw", "JSON", "Detailed" };
    constexpr int buttonTags[] = { INFO_VIEW, RAW_VIEW, JSON_VIEW, NORMAL_VIEW };

    for (int i = 0; i < 4; ++i) {

        float yPos = 20.f * i;

        auto* button = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this](CCMenuItemToggler* from) {
            for (auto* other : this->viewButtons) {
                if (other != from && other->isOn()) {
                    other->toggle(false);
                    other->setEnabled(true);
                }
                from->setEnabled(false);
                from->toggle(true);
            }
        });
        if (!button) return false;
        button->setTag(buttonTags[i]);
        button->setPosition(ccp(0.f, yPos));
        menu->addChild(button);
        viewButtons.push_back(button);
        if (i == 0) {
            button->setEnabled(false);
            button->toggle(true);
        }

        auto* label = CCLabelBMFont::create(buttonLabels[i].data(), "bigFont.fnt");
        if (!label) return false;
        label->setScale(0.5f);
        label->setAnchorPoint(ccp(0.f, 0.5f));
        label->setPosition(ccp(12.f, yPos));
        menu->addChild(label);

        float labelEnd = label->getPositionX() + label->getScaledContentWidth();

    }
    this->addChild(menu);

    typeButton = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [](CCMenuItemToggler*) {});
    if (!typeButton) return false;
    typeButton->setPosition(ccp(0.f, viewButtons.back()->getPositionY() + viewButtons.back()->getScaledContentHeight() / 2.f + 30.f));
    menu->addChild(typeButton);

    auto* typeLabel = CCLabelBMFont::create("Type Handling", "bigFont.fnt");
    if (!typeLabel) return false;
    typeLabel->setAnchorPoint(ccp(0.f, 0.5f));
    typeLabel->setPosition(ccp(12.f, typeButton->getPositionY()));
    typeLabel->setScale(0.5f);
    menu->addChild(typeLabel);

    auto* segment = CCDrawNode::create();
    if (!segment) return false;
    segment->setAnchorPoint(ccp(0.f, 0.f));
    segment->setPosition(ccp(
        menu->getPositionX() - 10.f,
        padding.y + typeButton->getPositionY() - 20.f
    ));
    segment->setContentWidth(typeLabel->boundingBox().getMaxX() + 20.f);
    segment->drawSegment(ccp(0.f, 0.f), ccp(segment->getContentWidth(), 0.f), 1.f, DARK_BROWN_4F);
    segment->setZOrder(1);
    this->addChild(segment);

    float objViewSize = m_mainLayer->getContentHeight() - (typeButton->getPositionY() + typeButton->getScaledContentHeight()) - 50.f;
    auto* objView = ObjectView::create(objViewSize, m_objSelection.get(), m_editorLayer);
    if (!objView) return false;
    objView->setPosition(ccp(
        segment->getPositionX() + segment->getContentWidth() / 2.f,
        padding.y + typeLabel->boundingBox().getMaxY() + (m_mainLayer->getContentHeight() - typeLabel->boundingBox().getMaxY()) / 2 - 15.f
    ));
    this->addChild(objView);
    objView->focusObject();

    auto* mask = CCDrawNode::create();
    mask->ignoreAnchorPointForPosition(true);
    mask->setContentSize(newParent->getContentSize());
    auto rect = getRectInNode(this, objView->getNodeToClipBehind());
    mask->drawRect(
        ccp(rect.getMinX() + 1.f, rect.getMinY() + 1.f),
        ccp(rect.getMaxX() - 1.f, rect.getMaxY() - 1.f),
        ccColor4F{ 0.f, 0.f, 0.f, 1.f }, 0.f, ccColor4F{ 0.f, 0.f, 0.f, 0.f }
    );
    newParent->setStencil(mask);
    newParent->setInverted(true);
    
    this->setOpacity(0);
    
    auto* background = CCLayerColor::create(ccColor4B { 0, 0, 0, 105 });
    background->setZOrder(-1);
    newParent->addChild(background);

    this->setObject(objSelection->getSelectedObject());

    return true;
}

InspectorPopup* InspectorPopup::create(ObjectSelection* objects, LevelEditorLayer* editorLayer) {
    auto* popup = new InspectorPopup();
    CCSize size = CCDirector::sharedDirector()->getWinSize() - CCSize(PADDING_HORIZONTAL, PADDING_VERTICAL);
    if (popup->initAnchored(size.width, size.height, objects, editorLayer)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

void InspectorPopup::setObject(GameObject* object) {

    bool firstUpdate = !m_attrList;
    if (m_attrList) this->removeChild(m_attrList);

    CCArray* listItems = CCArray::create();
    gd::string saveStr = object->getSaveString(static_cast<GJBaseGameLayer*>(m_editorLayer));
    std::stringstream saveStream(saveStr);
    std::string k, v; // todo: breaking geode style guidelines?
    bool key = true;
    while (std::getline(saveStream, key ? k : v, ',')) {
        if (!key) {
            auto objAttr = new ObjectAttribute(object, std::stoi(k), v);
            listItems->addObject(AttributeListing::create(objAttr));
        }
        key = !key;
    }

    bool evenIndex = true;
    for (auto listItem : CCArrayExt<AttributeListing*>(listItems)) {
        listItem->updateColors(evenIndex);
        evenIndex = !evenIndex;
    }
    m_attrList = ListView::create(listItems, ATTR_LISTING_SIZE.height, ATTR_LISTING_SIZE.width, 235.f);
    m_attrList->setPosition(ccp(215.f, 40.f));
    m_attrList->setZOrder(1);
    m_attrList->setPrimaryCellColor(LIGHTER_BROWN_3B);
    m_attrList->setSecondaryCellColor(LIGHT_BROWN_3B);
    this->addChild(m_attrList);

    if (firstUpdate) {

        auto listBorder = Border::create(nullptr, { 0, 0, 0, 0 }, m_attrList->getContentSize());
        listBorder->setPosition(m_attrList->getPosition());
        listBorder->setAnchorPoint(m_attrList->getAnchorPoint());
        listBorder->setZOrder(m_attrList->getZOrder() + 1);
        this->addChild(listBorder);

        auto listBackground = CCLayerColor::create(LIGHTER_BROWN_4B);
        listBackground->setPosition(m_attrList->getPosition());
        listBackground->setAnchorPoint(m_attrList->getAnchorPoint());
        listBackground->setZOrder(m_attrList->getZOrder() - 1);
        listBackground->setContentSize(m_attrList->getContentSize());
        this->addChild(listBackground);

    }

}