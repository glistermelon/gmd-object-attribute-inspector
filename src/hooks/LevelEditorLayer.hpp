#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(LevelEditorLayerHook, LevelEditorLayer) {

    struct Fields {
        bool m_noObjectsAlertIsOpen;
    };

    $override
	bool init(GJGameLevel* p0, bool p1);

public:

    void openObjectInspector();

    void onOpenObjectInspector(cocos2d::CCObject* sender);

};