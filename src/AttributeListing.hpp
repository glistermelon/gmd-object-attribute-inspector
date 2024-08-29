#pragma once

#include "include.hpp"
#include "attributes.hpp"
#include "GameObjectWrapper.hpp"

extern const CCSize LISTING_SIZE;

class AttributeListing : public GenericListCell {

	GameObjectWrapper* m_object;
	int m_attrKey;
	
	std::vector<CCScale9Sprite*> textBoxes;

	AttributeListing();

	bool init(GameObjectWrapper* object, int attrKey);

public:

	static AttributeListing* create(GameObjectWrapper* object, int attrKey);

	void updateColor(int index) {
		for (auto sprite : textBoxes) sprite->setColor(index % 2 == 0 ? LIGHT_BROWN_3B : LIGHTER_BROWN_3B);
		GenericListCell::m_backgroundLayer->setColor(index % 2 == 0 ? LIGHTER_BROWN_3B : LIGHT_BROWN_3B);
	}

	bool listBefore(AttributeListing* other) {
		return m_attrKey < other->m_attrKey;
	}

	void editCallback(CCObject*);
	void deleteCallback(CCObject*);

};