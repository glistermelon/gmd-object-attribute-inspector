#pragma once

#include "include.hpp"
#include "ObjectSelection.hpp"

class ObjectView : public CCNode {

	ObjectSelection* m_selection;
	geode::Border* m_window;
	CCLabelBMFont* m_indexLabel;
	LevelEditorLayer* m_editorLayer;

	bool init(float size, ObjectSelection* selection, LevelEditorLayer* editorLayer);

	void updateIndexLabel();

	void selectPrevOrNext(CCObject* obj);

public:

	static ObjectView* create(float size, ObjectSelection* selection, LevelEditorLayer* editorLayer);

	void focusObject();

	inline CCNode* getNodeToClipBehind() {
		return m_window;
	}

};
