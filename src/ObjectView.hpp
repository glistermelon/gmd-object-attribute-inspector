#pragma once

#include "include.hpp"
#include "ObjectSelection.hpp"

class ObjectView : public CCNode {

	ObjectSelection* m_selection;
	geode::Border* m_window;
	CCLabelBMFont* m_indexLabel;

	bool init(ObjectSelection* selection, float size);

public:

	static ObjectView* create(ObjectSelection* selection, float size);

	// For the clipping node in InspectorPopup::setup
	inline CCNode* getNodeToClipBehind() { return m_window; }

	void updateIndexLabel();

	void focusObject();

	void onZoomIn(CCObject*);
	void onZoomOut(CCObject*);

};
