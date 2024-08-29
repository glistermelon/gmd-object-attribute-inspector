#include "InspectorPopup.hpp"

#include "AttributeListing.hpp"
#include "ObjectSelection.hpp"
#include "ObjectView.hpp"
#include "AttributeEditor.hpp"

InspectorPopup* InspectorPopup::activePopup;

static bool noObjPopupOpen = false;
const CCPoint PADDING(50.f, 50.f);

bool InspectorPopup::setup() {

    // Gather selected objects

    if (noObjPopupOpen) return false;

    CCArrayExt<GameObject*> objects = LevelEditorLayer::get()->m_editorUI->getSelectedObjects();
    if (objects.size() == 0) {
        noObjPopupOpen = true;
        createQuickPopup(
            "No Objects Selected!",
            "You need to select something to use the attribute inspector.",
            "OK", nullptr,
            [](auto, bool) { noObjPopupOpen = false; }
        )->show();
        return false;
    }
    m_selection.addObjects(objects.begin(), objects.end());

    // Setup clipping node so we can see through the popup in the object viewer

    auto* clipNode = CCClippingNode::create(CCNode::create());
    clipNode->setContentSize(CCScene::get()->getContentSize());
    for (auto* child : CCArrayExt<CCNode*>(this->getChildren())) {
        clipNode->addChild(child);
        this->removeChild(child);
    }
    this->addChild(clipNode);

    // Put in a menu

    auto* menu = CCMenu::create();
    if (!menu) return false;
    menu->setAnchorPoint(ccp(0.f, 0.f));
    menu->setPosition(PADDING);
    this->addChild(menu);

    // Add view selection buttons (no I do not want to use a layout)

    constexpr int numViewOptions = 4;
    constexpr std::array<std::string_view, numViewOptions> buttonLabels = { "Basic Info", "Raw", "JSON", "Detailed" };
    std::array<CCMenuItemToggler** const, numViewOptions> buttonMembers = { &m_infoViewBtn, &m_rawViewBtn, &m_jsonViewBtn, &m_normalViewBtn };

    for (int i = 0; i < numViewOptions; ++i) {

        float yPos = 20.f * i;

        auto* button = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this](CCMenuItemToggler* from) {
            for (auto* other : { m_normalViewBtn, m_jsonViewBtn, m_rawViewBtn, m_infoViewBtn }) {
                if (other != from && other->isOn()) {
                    other->toggle(false);
                    other->setEnabled(true);
                }
                from->setEnabled(false);
                from->toggle(true);
            }
            this->updateAttrViewVisibility();
        });

        *buttonMembers[i] = button;

        button->setPosition(ccp(0.f, yPos));

        menu->addChild(button);
        if (i == numViewOptions - 1) {
            button->setEnabled(false);
            button->toggle(true);
        }

        auto* label = CCLabelBMFont::create(buttonLabels[i].data(), "bigFont.fnt");
        label->setScale(0.5f);
        label->setAnchorPoint(ccp(0.f, 0.5f));
        label->setPosition(ccp(12.f, yPos));
        menu->addChild(label);

    }

    // Add button for toggling type handling

    auto typeButton = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [](CCMenuItemToggler*) {});
    typeButton->toggle(true);
    typeButton->setPosition(ccp(0.f, m_normalViewBtn->getPositionY() + m_normalViewBtn->getScaledContentHeight() / 2.f + 30.f));
    menu->addChild(typeButton);

    auto* typeLabel = CCLabelBMFont::create("Type Handling", "bigFont.fnt");
    typeLabel->setAnchorPoint(ccp(0.f, 0.5f));
    typeLabel->setPosition(ccp(12.f, typeButton->getPositionY()));
    typeLabel->setScale(0.5f);
    menu->addChild(typeLabel);

    // Add separating segment between type handling button and view selection buttons

    auto* segment = CCDrawNode::create();
    if (!segment) return false;
    segment->setAnchorPoint(ccp(0.f, 0.f));
    segment->setPosition(ccp(
        menu->getPositionX() - 10.f,
        PADDING.y + typeButton->getPositionY() - 20.f
    ));
    segment->setContentWidth(typeLabel->boundingBox().getMaxX() + 20.f);
    segment->drawSegment(ccp(0.f, 0.f), ccp(segment->getContentWidth(), 0.f), 1.f, DARK_BROWN_4F);
    segment->setZOrder(1);
    this->addChild(segment);

    // Add object viewer

    float objViewSize = m_mainLayer->getContentHeight() - (typeButton->getPositionY() + typeButton->getScaledContentHeight()) - 50.f;
    m_objectView = ObjectView::create(&m_selection, objViewSize);
    if (!m_objectView) return false;
    m_objectView->setPosition(ccp(
        segment->getPositionX() + segment->getContentWidth() / 2.f,
        PADDING.y + typeLabel->boundingBox().getMaxY() + (m_mainLayer->getContentHeight() - typeLabel->boundingBox().getMaxY()) / 2 - 15.f
    ));
    this->addChild(m_objectView);
    m_objectView->focusObject();

    // Now that the object viewer is set up, we can set up the stencil for the clipping node

    auto* stencil = CCDrawNode::create();
    stencil->ignoreAnchorPointForPosition(true);
    stencil->setContentSize(clipNode->getContentSize());
    auto rect = getRectInNode(this, m_objectView->getNodeToClipBehind());
    stencil->drawRect(
        ccp(rect.getMinX() + 1.f, rect.getMinY() + 1.f),
        ccp(rect.getMaxX() - 1.f, rect.getMaxY() - 1.f),
        ccColor4F{ 0.f, 0.f, 0.f, 1.f }, 0.f, ccColor4F{ 0.f, 0.f, 0.f, 0.f }
    );
    clipNode->setStencil(stencil);
    clipNode->setInverted(true);

    // Add our own background dimming node for easy compatiblity with clipping
    
    this->setOpacity(0);

    auto* background = CCLayerColor::create(ccColor4B { 0, 0, 0, 105 });
    background->setZOrder(-1);
    clipNode->addChild(background);

    // Load in the selected object

    this->loadObject();

    // Hide the object info label because it blocks the object view

    auto objectInfoLabel = LevelEditorLayer::get()->m_editorUI->m_objectInfoLabel;
    m_objectInfoLabelVisibility = objectInfoLabel->isVisible();
    objectInfoLabel->setVisible(false);

    return true;
}

InspectorPopup* InspectorPopup::create() {
    if (InspectorPopup::get()) return nullptr;
    auto* popup = new InspectorPopup();
    CCSize size = CCDirector::sharedDirector()->getWinSize() - PADDING;
    if (popup->initAnchored(size.width, size.height)) {
        popup->autorelease();
        activePopup = popup;
        return popup;
    }
    delete popup;
    return nullptr;
}

void InspectorPopup::loadObject() {

    auto object = m_selection.getSelectedObject();

    if (!m_attrView) { // first update

        m_attrView = CCNode::create();
        m_attrView->setContentSize(CCSize(LISTING_SIZE.width, 235.f));
        m_attrView->setPosition(ccp(215.f, 40.f));
        m_attrView->setAnchorPoint(ccp(0.f, 0.f));
        m_attrView->setZOrder(1);
        this->addChild(m_attrView);

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

    /* normal view */

    // Set up and order the listings

    std::vector<AttributeListing*> attrListings;
    for (auto attr : m_selection.getSelectedObject()->getAttributes())
        attrListings.push_back(AttributeListing::create(object, attr.first));
    std::sort(attrListings.begin(), attrListings.end(), [](auto a, auto b) { return a->listBefore(b); });
    auto attrListingArray = CCArray::create();
    for (auto listing : attrListings) attrListingArray->addObject(listing);

    // UX stuff

    bool fixScroll = m_listView; // if the view already exists, keep scroll height locked for better UX
    float scroll;
    if (fixScroll) {
        scroll = m_listView->m_tableView->m_contentLayer->getPositionY();
        m_listView->getParent()->removeChild(m_listView); // get rid of the old one
    }

    // Create and set up the list view

    m_listView = ListView::create(
        attrListingArray, LISTING_SIZE.height,
        m_attrView->getContentWidth(), m_attrView->getContentHeight()
    );
    m_listView->setPosition(ccp(0.f, 0.f));
    m_listView->setAnchorPoint(ccp(0.f, 0.f));
    m_listView->setZOrder(1);
    m_listView->setPrimaryCellColor(LIGHTER_BROWN_3B);
    m_listView->setSecondaryCellColor(LIGHT_BROWN_3B);
    if (fixScroll) m_listView->m_tableView->m_contentLayer->setPositionY(scroll);
    for (int i = 0; i < attrListings.size(); ++i) attrListings[i]->updateColor(i);
    m_attrView->addChild(m_listView);

    /* json view */

    if (m_jsonView) m_attrView->removeChild(m_jsonView);

    m_jsonView = JsonTextArea::create(object->getJson(), m_attrView->getContentSize());
    m_jsonView->setBgLayer(m_attrViewBg);
    m_jsonView->setBgColor({ 50, 50, 50 });
    m_jsonView->setPosition(ccp(7.5f, 0.f));
    m_jsonView->setAnchorPoint(ccp(0.f, 0.f));
    m_attrView->addChild(m_jsonView);

    /* raw view */

    if (m_rawView) m_attrView->removeChild(m_rawView);
    m_rawView = MDTextArea::create("", m_attrView->getContentSize());
    m_rawView->setPosition(ccp(7.5f, 0.f));
    m_rawView->setAnchorPoint(ccp(0.f, 0.f));
    m_attrView->addChild(m_rawView);

    /* finishing touches */

    this->updateAttrViewVisibility();

}

void InspectorPopup::updateAttrViewVisibility() {

    m_listView->setVisible(m_normalViewBtn->isToggled());
    m_jsonView->setVisible(m_jsonViewBtn->isToggled());
    m_rawView->setVisible(m_rawViewBtn->isToggled() || m_infoViewBtn->isToggled());

    if (m_rawViewBtn->isToggled()) {
        // Use m_rawView to display raw object data string
        std::string s = "```\n" + m_selection.getSelectedObject()->getRaw() + "\n```";
        m_rawView->setString(s.c_str());
    }
    else {
        // Use m_rawView to display the basic object info (as in the object info label)
        std::stringstream ss;
        const char* s = LevelEditorLayer::get()->m_editorUI->m_objectInfoLabel->getString();
        int len = std::strlen(s);
        for (int i = 0; i < len; ++i) {
            const char c = s[i];
            if (c == '\n' && i + 1 != len) ss << "\\\n";
            else ss << c;
        }
        std::string stdStr = ss.str();
        m_rawView->setString(stdStr.c_str());
    }

    // Background color stuff
    m_attrViewBg->setVisible(!m_rawView->isVisible());
    if (m_listView->isVisible()) m_attrViewBg->setColor(LIGHTER_BROWN_3B);
    else if (m_jsonView->isVisible()) m_jsonView->updateBgColor();

}

void InspectorPopup::onAddAttribute(CCObject*) {
    int key;
    bool badKey = false;
    auto idStr = m_newAttrIDInput->getString();
    try {
        key = std::stoi(idStr);
    }
    catch (std::invalid_argument) {
        badKey = true;
    }
    catch (std::out_of_range) {
        badKey = true;
    }
    if (badKey) {
        FLAlertLayer::create(
            "Invalid Attribute Key",
            decltype(idStr)("'") + idStr + "' is not a valid key for a new attribute.",
            "OK"
        )->show();
    }
    AttributeEditor::create(m_selection.getSelectedObject(), key)->show();
}

void InspectorPopup::selectPrevOrNext(CCObject* obj) {
    obj->getTag() ? m_selection.selectNext() : m_selection.selectPrev();
    this->loadObject();
    m_objectView->updateIndexLabel();
    m_objectView->focusObject();
}

void InspectorPopup::onExit() {
    Popup<>::onExit();
    LevelEditorLayer::get()->m_editorUI->m_objectInfoLabel->setVisible(m_objectInfoLabelVisibility);
    InspectorPopup::activePopup = nullptr;
}