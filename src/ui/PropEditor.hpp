#pragma once

#include <common/GameObjectWrapper.hpp>

class PropEditor : public geode::Popup {

    std::optional<PropKey> m_propKey;
    GameObjectWrapper m_objectWrapper;

    geode::Ref<geode::TextInput> m_keyInput;
    geode::Ref<geode::TextInput> m_valInput;

    PropEditor(std::optional<PropKey> propKey, GameObjectWrapper objectWrapper);

    bool init(cocos2d::CCSize size);

    geode::Result<std::string> getKeyString();

public:

    static PropEditor* create(std::optional<PropKey> propKey, GameObjectWrapper objectWrapper);

    void onCancel(cocos2d::CCObject* sender);
    void onApply(cocos2d::CCObject* sender);

protected:

    virtual void onClose(cocos2d::CCObject* sender) override;

};