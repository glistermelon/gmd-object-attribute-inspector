#pragma once

#include "include.hpp"
#include "ValueContainer.hpp"
#include "AttributeDocs.hpp"

#include <matjson.hpp>

class GameObjectWrapper {

    GameObject* m_object;
    GameObject* m_updatedObject;
    float m_updatedObjectScale;
    
    std::map<int, ValueContainer> m_attributes;
    std::map<int, ValueContainer> m_attributesBackup;

    void setGameObject(GameObject* object);

public:

    GameObjectWrapper(GameObject* object);

    GameObject* getGameObject();

    std::optional<ValueContainer> getAttribute(int key);

    void setAttribute(int key, ValueContainer value);

    decltype(m_attributes) getAttributes();

    void removeAttribute(int key);

    void rollbackChanges();

    gd::string getObjectString();

    // returns true if attempted update just downright fails
	bool tryUpdate(std::vector<int>& modified, std::vector<int>& deleted, std::vector<int>& added);

    void finishUpdate();

    void cancelUpdate();

    // bool param to callback is whether or not the commit was carried out successfully
    void commitWithGUI(int modifiedAttr, std::function<void(bool)> callback = [](bool){});

    matjson::Object getJson();

    gd::string getRaw();

};