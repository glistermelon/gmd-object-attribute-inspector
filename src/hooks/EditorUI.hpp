#include <Geode/modify/EditorUI.hpp>

class $modify(EditorUIHook, EditorUI) {

    $override
    bool init(LevelEditorLayer* levelEditorLayer);

    $override
	void keyDown(cocos2d::enumKeyCodes key, double timestamp);

};