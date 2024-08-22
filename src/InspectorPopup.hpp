#pragma once

#include "include.hpp"
#include "ObjectSelection.hpp"

enum {
	INFO_VIEW,
	JSON_VIEW,
	RAW_VIEW,
	NORMAL_VIEW // "Detailed"
};

class InspectorPopup : public geode::Popup<ObjectSelection*, LevelEditorLayer*> {

	std::vector<CCMenuItemToggler*> viewButtons;
	CCMenuItemToggler* typeButton;
	CCMenuItemToggler* docsButton;
	ListView* m_attrList = nullptr;
	std::shared_ptr<ObjectSelection> m_objSelection;
	LevelEditorLayer* m_editorLayer;

	static InspectorPopup* activePopup;

protected:

	bool setup(ObjectSelection* objSelection, LevelEditorLayer* editorLayer) override;

public:

	static InspectorPopup* create(ObjectSelection* objects, LevelEditorLayer* editorLayer);

	inline static InspectorPopup* get() { return activePopup; }

	void setObject(GameObject* object);

};