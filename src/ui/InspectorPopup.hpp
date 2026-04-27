#pragma once

#include "ObjectViewPanel.hpp"

#include <common/GameObjectWrapper.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/TextArea.hpp>

class InspectorPopup : public geode::Popup {

	static inline InspectorPopup* s_activePopup;

	std::vector<GameObjectWrapper> m_objects;
	size_t m_selectedObjectIndex;

	geode::Ref<ObjectViewPanel> m_objectViewPanel;
	geode::Ref<cocos2d::CCLabelBMFont> m_indexLabel;

	geode::Ref<cocos2d::CCMenu> m_selViewTypeMenu;

	geode::Ref<cocos2d::CCNode> m_propViewContainer;
	geode::Ref<geode::Border> m_propViewBorder;
	geode::Ref<cocos2d::CCLayerColor> m_propViewBackground;

	geode::Ref<geode::ListView> m_propListView = nullptr;
	geode::Ref<geode::ScrollLayer> m_textView = nullptr;

	geode::Ref<CCMenuItemToggler> m_propTypeBtn;
	geode::Ref<CCMenuItemToggler> m_propDocBtn;

	geode::Ref<CCMenuItemToggler> m_defaultViewBtn;
	geode::Ref<CCMenuItemToggler> m_jsonViewBtn;
	geode::Ref<CCMenuItemToggler> m_rawViewBtn;
	geode::Ref<CCMenuItemToggler> m_minViewBtn;

	geode::Ref<CCTextInputNode> m_newPropInput;

	geode::Ref<cocos2d::CCMenu> m_propViewMenu;
	geode::Ref<CCMenuItemSpriteExtra> m_copyBtn;
	geode::Ref<CCMenuItemSpriteExtra> m_newPropBtn;

	std::string m_copyStr;

	CCMenuItemToggler* addViewTypeButton(int index, std::string name);
	void onSelectViewTypeButton(CCMenuItemToggler* button);

	void replaceWithClipNode(cocos2d::CCNode* node);

	void resetPropViewContainer();

protected:

	bool init(std::vector<GameObject*> objects);

	virtual void onClose(cocos2d::CCObject* sender) override;

public:

	static InspectorPopup* create(std::vector<GameObject*> objects);

	static InspectorPopup* get();

	void refreshTextView(std::string text);

	void refreshDefaultView();
	void refreshJsonView();
	void refreshRawView();
	void refreshMinView();

	void refresh();

	void onCopy(cocos2d::CCObject* sender);

	void onNextObject(cocos2d::CCObject* sender);
	void onPrevObject(cocos2d::CCObject* sender);

	void onZoomIn(cocos2d::CCObject* sender);
	void onZoomOut(cocos2d::CCObject* sender);	

	void onAddProperty(cocos2d::CCObject* sender);

	void setProperty(std::string setKey, std::string setVal);
	void deleteProperty(std::string key);
	void handlePropUpdateResult(geode::Result<> result);

	// void selectPrevOrNext(CCObject* obj);

};