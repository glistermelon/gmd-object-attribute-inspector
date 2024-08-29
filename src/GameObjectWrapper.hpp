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

    // returns true if attempted update just downright fails
	bool tryUpdate(std::vector<int>& modified, std::vector<int>& deleted, std::vector<int>& added);
    void finishUpdate(); // confirms changes from tryUpdate
    void cancelUpdate(); // cancels changes from tryUpdate

public:

    GameObjectWrapper(GameObject* object);

    GameObject* getGameObject();

    std::optional<ValueContainer> getAttribute(int key);

    decltype(m_attributes) getAttributes();

    void setAttribute(int key, ValueContainer value);

    void removeAttribute(int key);

    void rollbackChanges();

    gd::string getObjectString();

    matjson::Object getJson();

    gd::string getRaw();

    // Semi-interactive (popups) function that orchestrates tryUpdate, finishUpdate, and cancelUpdate
    // Bool param to callback is whether or not the commit was carried out successfully.
    void tryCommit(int modifiedAttr, std::function<void(bool)> callback = [](bool) {});

};