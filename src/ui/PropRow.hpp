#pragma once

#include <common/GameObjectWrapper.hpp>

#include <Geode/cocos/label_nodes/CCLabelBMFont.h>

constexpr cocos2d::CCSize propRowSize = ccp(300.0f, 60.0f);

class PropRow : public geode::GenericListCell {

	PropKey m_propKey;
	GameObjectWrapper m_objectWrapper;

	PropRow(PropKey propKey, GameObjectWrapper objectWrapper);

	bool init(bool odd);

public:

	static PropRow* create(bool odd, PropKey propKey, GameObjectWrapper objectWrapper);

	void onEdit(cocos2d::CCObject* sender);
	void onDelete(cocos2d::CCObject* sender);

};