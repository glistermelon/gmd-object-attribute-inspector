#pragma once

#include <common/GameObjectWrapper.hpp>

class ObjectViewPanel : public cocos2d::CCNode {

	geode::Ref<geode::Border> m_window;

	bool init(float size);

	void focusObject(GameObject* object);

public:

	static ObjectViewPanel* create(float size);

	cocos2d::CCNode* getWindow();

	void refresh(GameObject* object);

};
