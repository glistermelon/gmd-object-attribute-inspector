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

    void setGameObject(GameObject* object) {
        m_object = object;
        gd::string saveStr = object->getSaveString(GJBaseGameLayer::get());
        std::stringstream saveStream(saveStr);
        std::string keyStr, valStr; // todo: breaking geode style guidelines?
        bool isKey = true;
        while (std::getline(saveStream, isKey ? keyStr : valStr, ',')) {
            if (!isKey) {
                auto key = std::stoi(keyStr);
                auto docs = AttributeDocs::getDocs(key);
                auto val = ValueContainer();
                if (!val.setValue(valStr, true, docs ? docs->getType() : ATTR_TYPE_UNKNOWN))
                    val.setUnknownValue(valStr);
                m_attributes[key] = val;
            }
            isKey = !isKey;
        }
        m_attributesBackup = m_attributes;
    }

public:

    GameObjectWrapper(GameObject* object) {
        this->setGameObject(object);
    }

    inline GameObject* getGameObject() { return m_object; }

    std::optional<ValueContainer> getAttribute(int key) {
        auto it = m_attributes.find(key);
        return it == m_attributes.end() ? std::optional<ValueContainer>() : it->second;
    }

    void setAttribute(int key, ValueContainer value) {
        m_attributes[key] = value;
    }

    decltype(m_attributes) getAttributes() { return m_attributes; }

    void rollbackChanges() { m_attributes = m_attributesBackup; }

    gd::string getObjectString() {
        std::stringstream ss;
        for (auto const& [key, value] : m_attributes) {
            ss << key << ',' << value.getRaw() << ',';
        }
        std::string s = ss.str();
        s.pop_back();
        return s;
    }

    // returns true if attempted update just downright fails
	bool tryUpdate(std::vector<int>& modified, std::vector<int>& deleted, std::vector<int>& added) {
        m_updatedObject = static_cast<GameObject*>(LevelEditorLayer::get()->createObjectsFromString(this->getObjectString(), true, false)->objectAtIndex(0));
        if (!m_updatedObject) return true;
        m_updatedObjectScale = m_updatedObject->getScale();
        m_updatedObject->setScale(0.f); // stupid way to do setVisible(false)
        GameObjectWrapper wrapped(m_updatedObject);
        auto newAttributes = wrapped.getAttributes();
        for (auto const& [key, value] : newAttributes) {
            auto prevValue = this->getAttribute(key);
            if (!prevValue.has_value())
                added.push_back(key);
            else if (value.getRaw() != prevValue.value().getRaw())
                modified.push_back(key);
        }
        for (auto const& [key, value] : this->getAttributes()) {
            if (newAttributes.find(key) == newAttributes.end())
                deleted.push_back(key);
        }
        return false;
    }

    // TODO: this shouldnt be a member function. make standalone in a cpp file
    void deleteObject(GameObject* object) {
        // either is all this part of an inlined deleteobject function or is robtop just really annoying
        LevelEditorLayer::get()->m_editorUI->deselectObject(object); // from decomp
        GJBaseGameLayer::get()->removeObjectFromSection(object);     // from decomp
        LevelEditorLayer::get()->removeSpecial(object);              // from decomp
        object->getParent()->removeChild(object);                    // added this part myself
        // TODO: is the gameobject leaked? idfk
    }

    void finishUpdate() {
        if (!m_updatedObject) {
            log::error("GameObjectWrapper::finishUpdate called without any updated object ready");
            return;
        }

        deleteObject(m_object);

        LevelEditorLayer::get()->m_editorUI->selectObject(m_updatedObject, false);
        this->setGameObject(m_updatedObject);
        m_updatedObject->setScale(m_updatedObjectScale);
        m_updatedObject = nullptr;

    }

    void cancelUpdate() {
        if (!m_updatedObject) {
            log::error("GameObjectWrapper::cancelUpdate called without any updated object ready");
            return;
        }
        this->rollbackChanges();
        deleteObject(m_updatedObject);
        m_updatedObject = nullptr;
    }

    matjson::Object getJson();

    gd::string getRaw();

};