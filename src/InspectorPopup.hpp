#pragma once

#include "include.hpp"
#include "ObjectSelection.hpp"
#include "GameObjectWrapper.hpp"
#include "JsonTextArea.hpp"
#include "DynamicListView.hpp"

enum AttributeViewMode {
	INFO_VIEW,
	JSON_VIEW,
	RAW_VIEW,
	NORMAL_VIEW // "Detailed"
};

class InspectorPopup : public geode::Popup<ObjectSelection*, LevelEditorLayer*> {

	std::unique_ptr<GameObjectWrapper> m_object = nullptr;
	std::vector<CCMenuItemToggler*> viewButtons;
	CCMenuItemToggler* typeButton;
	CCMenuItemToggler* docsButton;
	std::shared_ptr<ObjectSelection> m_objSelection; // todo: unique-ify
	LevelEditorLayer* m_editorLayer; // todo: bye bye

	CCNode* m_attrView;
	CCLayerColor* m_attrViewBg;
	DynamicListView* m_attrList = nullptr;
	JsonTextArea* m_jsonView = nullptr;
	MDTextArea* m_rawView = nullptr;

	CCMenuItemToggler* m_normalViewBtn;
	CCMenuItemToggler* m_jsonViewBtn;
	CCMenuItemToggler* m_rawViewBtn;
	CCMenuItemToggler* m_infoViewBtn;

	bool m_objectInfoLabelVisibility;

	CCTextInputNode* m_newAttrIDInput;

	static InspectorPopup* activePopup;

protected:

	bool setup(ObjectSelection* objSelection, LevelEditorLayer* editorLayer) override;

public:

	static InspectorPopup* create(ObjectSelection* objects, LevelEditorLayer* editorLayer);

	inline static InspectorPopup* get() { return activePopup; }

	// set object to nullptr to reload same object
	void setObject(GameObject* object);

	void updateAttrViewVisibility();

	void onExit() override;

	void onAddAttribute(CCObject*);

};