#pragma once

#include "include.hpp"
#include "attr.hpp"
#include "GameObjectWrapper.hpp"
#include "DynamicListView.hpp"

extern const CCSize ATTR_LISTING_SIZE;

class AttributeListing : public DynamicListItem {

	GameObjectWrapper* m_object;
	int m_attrKey;
	
	std::vector<CCScale9Sprite*> textBoxes;

	inline AttributeListing() : DynamicListItem(ATTR_LISTING_SIZE) {}

public:

	void updateListItem(int index) override {
		for (auto sprite : textBoxes) sprite->setColor(index % 2 == 0 ? LIGHT_BROWN_3B : LIGHTER_BROWN_3B);
	}

	bool listBefore(DynamicListItem* other) override {
		auto listing = typeinfo_cast<AttributeListing*>(other);
		return listing ? m_attrKey < listing->m_attrKey : false;
	}

private:

	bool init(GameObjectWrapper* object, int attrKey);

public:

	static AttributeListing* create(GameObjectWrapper* object, int attrKey);

	void editCallback(CCObject*);
	void deleteCallback(CCObject*);

};
