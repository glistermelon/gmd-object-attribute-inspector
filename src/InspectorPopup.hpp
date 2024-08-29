#pragma once

#include "include.hpp"
#include "ObjectSelection.hpp"
#include "ObjectView.hpp"
#include "GameObjectWrapper.hpp"
#include "JsonTextArea.hpp"

enum AttributeViewMode {
	INFO_VIEW,
	JSON_VIEW,
	RAW_VIEW,
	NORMAL_VIEW // "Detailed"
};

class InspectorPopup : public geode::Popup<> {

	ObjectSelection m_selection;

	ObjectView* m_objectView;

	CCNode* m_attrView;
	CCLayerColor* m_attrViewBg;
	ListView* m_listView = nullptr;
	JsonTextArea* m_jsonView = nullptr;
	MDTextArea* m_rawView = nullptr;

	CCMenuItemToggler* m_typeBtn;
	CCMenuItemToggler* m_docsBtn;

	CCMenuItemToggler* m_normalViewBtn;
	CCMenuItemToggler* m_jsonViewBtn;
	CCMenuItemToggler* m_rawViewBtn;
	CCMenuItemToggler* m_infoViewBtn;

	bool m_objectInfoLabelVisibility;

	CCTextInputNode* m_newAttrIDInput;

	static InspectorPopup* activePopup;

protected:

	bool setup() override;

public:

	static InspectorPopup* create();

	inline static InspectorPopup* get() { return activePopup; }

	void loadObject();

	void updateAttrViewVisibility();

	void onAddAttribute(CCObject*);

	void selectPrevOrNext(CCObject* obj);

	void onExit() override;

};