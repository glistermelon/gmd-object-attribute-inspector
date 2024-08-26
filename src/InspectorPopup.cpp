#include "InspectorPopup.hpp"

#include "AttributeListing.hpp"
#include "ObjectSelection.hpp"
#include "ObjectView.hpp"

InspectorPopup* InspectorPopup::activePopup;

CCLabelBMFont* getObjectInfoLabel() {
    return static_cast<CCLabelBMFont*>(LevelEditorLayer::get()->getChildByIDRecursive("object-info-label"));
}

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
    CCMenuItemToggler** const buttonMembers[] = { &m_infoViewBtn, &m_rawViewBtn, &m_jsonViewBtn, &m_normalViewBtn };

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

                this->updateAttrViewVisibility();

            }
        });

        *buttonMembers[i] = button;

        button->setTag(buttonTags[i]);
        button->setPosition(ccp(0.f, yPos));
        menu->addChild(button);
        viewButtons.push_back(button);
        if (i == 3) {
            button->setEnabled(false);
            button->toggle(true);
        }

        auto* label = CCLabelBMFont::create(buttonLabels[i].data(), "bigFont.fnt");
        label->setScale(0.5f);
        label->setAnchorPoint(ccp(0.f, 0.5f));
        label->setPosition(ccp(12.f, yPos));
        menu->addChild(label);

        float labelEnd = label->getPositionX() + label->getScaledContentWidth();

    }
    this->addChild(menu);

    typeButton = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [](CCMenuItemToggler*) {});
    typeButton->toggle(true);
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

    auto objectInfoLabel = getObjectInfoLabel();
    m_objectInfoLabelVisibility = objectInfoLabel->isVisible();
    objectInfoLabel->setVisible(false);

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

    AttributeViewMode viewMode;

    if (m_object && m_object->getGameObject() == object) return;

    bool firstUpdate = !m_attrView;

    m_attrView = CCNode::create();
    m_attrView->setContentSize(CCSize(ATTR_LISTING_SIZE.width, 235.f));
    m_attrView->setPosition(ccp(215.f, 40.f));
    m_attrView->setAnchorPoint(ccp(0.f, 0.f));
    m_attrView->setZOrder(1);
    this->addChild(m_attrView);

    if (firstUpdate) {

        auto listBorder = Border::create(nullptr, { 0, 0, 0, 0 }, m_attrView->getContentSize());
        listBorder->setPosition(m_attrView->getPosition());
        listBorder->setAnchorPoint(m_attrView->getAnchorPoint());
        listBorder->setZOrder(m_attrView->getZOrder() + 1);
        this->addChild(listBorder);

        m_attrViewBg = CCLayerColor::create();
        m_attrViewBg->setContentSize(m_attrView->getContentSize());
        m_attrViewBg->setPosition(m_attrView->getPosition());
        m_attrViewBg->setAnchorPoint(m_attrView->getAnchorPoint());
        m_attrViewBg->setZOrder(m_attrView->getZOrder() - 1);
        m_attrViewBg->setOpacity(255);
        this->addChild(m_attrViewBg);

    }

    m_object.reset(new GameObjectWrapper(object));

    /* normal view */

    if (m_attrList) this->removeChild(m_attrList);

    CCArray* listItems = CCArray::create();
    for (auto [key, value] : m_object->getAttributes())
        listItems->addObject(AttributeListing::create(m_object.get(), key));

    bool evenIndex = true;
    for (auto listItem : CCArrayExt<AttributeListing*>(listItems)) {
        listItem->updateColors(evenIndex);
        evenIndex = !evenIndex;
    }

    m_attrList = ListView::create(listItems, ATTR_LISTING_SIZE.height, m_attrView->getContentWidth(), m_attrView->getContentHeight());
    m_attrList->setPosition(ccp(0.f, 0.f));
    m_attrList->setAnchorPoint(ccp(0.f, 0.f));
    m_attrList->setZOrder(1);
    m_attrList->setPrimaryCellColor(LIGHTER_BROWN_3B);
    m_attrList->setSecondaryCellColor(LIGHT_BROWN_3B);
    m_attrView->addChild(m_attrList);

    /* json view */

    if (m_jsonView) this->removeChild(m_jsonView);

    m_jsonView = JsonTextArea::create(m_object->getJson(), m_attrView->getContentSize());
    m_jsonView->setBgLayer(m_attrViewBg);
    m_jsonView->setBgColor({ 50, 50, 50 });
    m_jsonView->setPosition(ccp(7.5f, 0.f));
    m_jsonView->setAnchorPoint(ccp(0.f, 0.f));
    m_attrView->addChild(m_jsonView);

    /* raw view */

    m_rawView = MDTextArea::create("", m_attrView->getContentSize());
    m_rawView->setPosition(ccp(7.5f, 0.f));
    m_rawView->setAnchorPoint(ccp(0.f, 0.f));
    m_attrView->addChild(m_rawView);

    /* finishing touches */

    this->updateAttrViewVisibility();

}

void InspectorPopup::updateAttrViewVisibility() {

    m_attrList->setVisible(m_normalViewBtn->isToggled());
    m_jsonView->setVisible(m_jsonViewBtn->isToggled());
    m_rawView->setVisible(m_rawViewBtn->isToggled() || m_infoViewBtn->isToggled());

    if (m_rawViewBtn->isToggled()) {
        std::string s = "```\n" + m_object->getRaw() + "\n```";
        m_rawView->setString(s.c_str());
    }
    else {
        std::stringstream ss;
        const char* s = getObjectInfoLabel()->getString();
        int len = std::strlen(s);
        for (int i = 0; i < len; ++i) {
            const char c = s[i];
            if (c == '\n' && i + 1 != len) ss << "\\\n";
            else ss << c;
        }
        std::string stdStr = ss.str();
        std::cout << "HEYYYY: '" << stdStr << "'" << std::endl;
        m_rawView->setString(stdStr.c_str());
    }
    m_attrViewBg->setVisible(!m_rawView->isVisible());
    if (m_attrList->isVisible()) m_attrViewBg->setColor(LIGHTER_BROWN_3B);
    else if (m_jsonView->isVisible()) m_jsonView->updateBgColor();

}

void InspectorPopup::onExit() {
    geode::Popup<ObjectSelection*, LevelEditorLayer*>::onExit();
    getObjectInfoLabel()->setVisible(m_objectInfoLabelVisibility);
}