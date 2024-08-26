#pragma once

#include "include.hpp"
#include "attr.hpp"
#include "GameObjectWrapper.hpp"

extern const CCSize ATTR_LISTING_SIZE;

class AttributeListing : public GenericListCell {

	GameObjectWrapper* m_object;
	int m_attrKey;
	
	std::vector<CCScale9Sprite*> textBoxes;

	inline AttributeListing() : GenericListCell("", ATTR_LISTING_SIZE) {}

public:

	inline void updateColors(bool evenIndex) {
		for (auto sprite : textBoxes) sprite->setColor(evenIndex ? LIGHT_BROWN_3B : LIGHTER_BROWN_3B);
	}

private:

	bool init(GameObjectWrapper* object, int attrKey);

public:

	static AttributeListing* create(GameObjectWrapper* object, int attrKey);

	void editCallback(CCObject*);

};
