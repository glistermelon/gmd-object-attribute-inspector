#pragma once

#include "include.hpp"
#include "ObjectAttribute.hpp"
#include "attr.hpp"

extern const CCSize ATTR_LISTING_SIZE;

class AttributeListing : public GenericListCell {

	ObjectAttribute* m_attr;
	
	std::vector<CCScale9Sprite*> textBoxes;

	inline AttributeListing() : GenericListCell("", ATTR_LISTING_SIZE) {}

public:

	inline void updateColors(bool evenIndex) {
		for (auto sprite : textBoxes) sprite->setColor(evenIndex ? LIGHT_BROWN_3B : LIGHTER_BROWN_3B);
	}

private:

	bool init(ObjectAttribute* objAttr);

public:

	static AttributeListing* create(ObjectAttribute* objAttr);

};
