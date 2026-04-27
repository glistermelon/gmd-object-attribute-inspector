#include "EditorUI.hpp"

#include "EditButtonBar.hpp"
#include "LevelEditorLayer.hpp"

#include <ui/InspectorPopup.hpp>

$override
bool EditorUIHook::init(LevelEditorLayer* levelEditorLayer) {
    if (!EditorUI::init(levelEditorLayer)) return false;
    auto inspectSprite = cocos2d::CCSprite::create("logo.png"_spr);
    if (!inspectSprite) return false;
    inspectSprite->setScale(43.0f / inspectSprite->getScaledContentWidth());
    auto inspsectBtn = CCMenuItemSpriteExtra::create(
        inspectSprite,
        levelEditorLayer,
        menu_selector(LevelEditorLayerHook::onOpenObjectInspector)
    );
    if (!inspsectBtn) return false;
    static_cast<EditButtonBarExtension*>(m_editButtonBar)->addButton(inspsectBtn);
    return true;
}

// Block editor actions when inspector popup is open
$override
void EditorUIHook::keyDown(cocos2d::enumKeyCodes key, double timestamp) {
    if (!InspectorPopup::get()) {
        EditorUI::keyDown(key, timestamp);
    }
}
