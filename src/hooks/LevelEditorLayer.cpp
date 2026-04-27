#include "LevelEditorLayer.hpp"

#include <ui/InspectorPopup.hpp>

#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <Geode/utils/cocos.hpp>

bool LevelEditorLayerHook::init(GJGameLevel* p0, bool p1) {

    if (!LevelEditorLayer::init(p0, p1)) return false;

    this->addEventListener(
        geode::KeybindSettingPressedEventV3(geode::Mod::get(), "open-object-inspector-keybind"),
        [this](geode::Keybind const& keybind, bool down, bool repeat, double timestamp) {
            if (!down || repeat) return;
            this->openObjectInspector();
        }
    );

    return true;

}

void LevelEditorLayerHook::openObjectInspector() {

    if (m_fields->m_noObjectsAlertIsOpen || !m_editorUI || InspectorPopup::get()) return;

    std::vector<GameObject*> objects;
    if (m_editorUI->m_selectedObject) {
        objects = { m_editorUI -> m_selectedObject };
    }
    else {
        objects = geode::cocos::CCArrayExt<GameObject*>(m_editorUI->getSelectedObjects()).toVector();
    }

    if (objects.empty()) {
        m_fields->m_noObjectsAlertIsOpen = true;
        geode::createQuickPopup(
            "No Objects Selected!",
            "You need to select something to use the object inspector.",
            "OK", nullptr,
            [this](auto, auto) { m_fields->m_noObjectsAlertIsOpen = false; }
        );
    }

    auto popup = InspectorPopup::create(objects);
    if (popup) popup->show();

}

void LevelEditorLayerHook::onOpenObjectInspector(cocos2d::CCObject* sender) {
    this->openObjectInspector();
}