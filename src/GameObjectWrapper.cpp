#include "GameObjectWrapper.hpp"

#include <matjson.hpp>

#pragma once

#include "include.hpp"
#include "ValueContainer.hpp"
#include "AttributeDocs.hpp"
#include "AttributeEditor.hpp"

#include <matjson.hpp>

void deleteObject(GameObject* object) {
    // either is all this part of an inlined deleteobject function or is robtop just really annoying
    LevelEditorLayer::get()->m_editorUI->deselectObject(object); // from decomp
    GJBaseGameLayer::get()->removeObjectFromSection(object);     // from decomp
    LevelEditorLayer::get()->removeSpecial(object);              // from decomp
    object->getParent()->removeChild(object);                    // added this part myself
    // TODO: is the gameobject leaked? idfk
}

void GameObjectWrapper::setGameObject(GameObject* object) {
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

GameObjectWrapper::GameObjectWrapper(GameObject* object) {
    this->setGameObject(object);
}

GameObject* GameObjectWrapper::getGameObject() { return m_object; }

std::optional<ValueContainer> GameObjectWrapper::getAttribute(int key) {
    auto it = m_attributes.find(key);
    return it == m_attributes.end() ? std::optional<ValueContainer>() : it->second;
}

void GameObjectWrapper::setAttribute(int key, ValueContainer value) {
    m_attributes[key] = value;
}

decltype(GameObjectWrapper::m_attributes) GameObjectWrapper::getAttributes() { return m_attributes; }

void GameObjectWrapper::removeAttribute(int key) {
    m_attributes.erase(key);
}

void GameObjectWrapper::rollbackChanges() { m_attributes = m_attributesBackup; }

gd::string GameObjectWrapper::getObjectString() {
    std::stringstream ss;
    for (auto const& [key, value] : m_attributes) {
        ss << key << ',' << value.getRaw() << ',';
    }
    std::string s = ss.str();
    s.pop_back();
    return s;
}

// returns true if attempted update just downright fails
bool GameObjectWrapper::tryUpdate(std::vector<int>& modified, std::vector<int>& deleted, std::vector<int>& added) {
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

void GameObjectWrapper::finishUpdate() {
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

void GameObjectWrapper::cancelUpdate() {
    if (!m_updatedObject) {
        log::error("GameObjectWrapper::cancelUpdate called without any updated object ready");
        return;
    }
    this->rollbackChanges();
    deleteObject(m_updatedObject);
    m_updatedObject = nullptr;
}

void GameObjectWrapper::commitWithGUI(int attrModified, std::function<void(bool)> callback) {

    std::vector<int> modified;
    std::vector<int> deleted;
    std::vector<int> added;
    if (this->tryUpdate(modified, deleted, added)) return AttributeEditor::commitFailed();

    auto it = std::find(modified.begin(), modified.end(), attrModified);
    if (it != modified.end()) modified.erase(it);

    std::vector<int>* changeVectors[] = { &modified, &deleted, &added };
    std::string_view changeWords[] = { "modified", "deleted", "created" };
    std::stringstream warnStream;

    bool noChanges = true;
    for (auto v : changeVectors) {
        if (!v->empty()) {
            noChanges = false;
            break;
        }
    }
    if (noChanges) return this->finishUpdate();

    for (int i = 0; i < 3; ++i) {
        std::vector<int>* changeVec = changeVectors[i];
        if (!changeVec->empty()) {
            warnStream << "The following other attributes were " << changeWords[i] << " due to your changes:\n";
            for (int attrKey : *changeVec) {
                warnStream << attrKey;
                auto docs = AttributeDocs::getDocs(attrKey);
                warnStream << " (" << (docs ? docs->getName() : "Unknown Attribute") << ")\n";
            }
        }
    }

    std::string warnStr = warnStream.str();
    warnStr.pop_back();
    createQuickPopup(
        "Commit Warning",
        warnStr,
        "Cancel", "Commit Anyway",
        [this, callback](auto, bool commitAnyway) {
            if (commitAnyway) this->finishUpdate();
            else this->cancelUpdate();
            callback(commitAnyway);
        }
    );

}

matjson::Object GameObjectWrapper::getJson() {
    matjson::Value json;
    for (auto [keyInt, value] : this->getAttributes()) {
        std::string key = std::to_string(keyInt);
        switch (value.getType()) {
            case ATTR_TYPE_INT:
                json[key] = value.getIntValue().value();
                break;
            case ATTR_TYPE_FLOAT:
                json[key] = value.getFloatValue().value();
                break;
            case ATTR_TYPE_BOOL:
                json[key] = value.getBoolValue().value();
                break;
            case ATTR_TYPE_STRING:
                json[key] = value.getStringValue().value();
                break;
            case ATTR_TYPE_COLOR: {
                matjson::Value colorJson;
                auto color = value.getColorValue().value();
                colorJson["hue"] = color.h;
                colorJson["saturation"] = color.s;
                colorJson["brightness"] = color.v;
                colorJson["absolute saturation"] = color.s_on;
                colorJson["absolute brightness"] = color.v_on;
                json[key] = colorJson;
                break;
            }
            case ATTR_TYPE_ARRAY: {
                std::stringstream ss(value.getRaw());
                std::string s;
                matjson::Array array;
                while (std::getline(ss, s, '.')) array.push_back(s);
                json[key] = array;
                break;
            }
            default:
                json[key] = value.getRaw();
        }
    }
    return json.as_object();
}

gd::string GameObjectWrapper::getRaw() {
    return m_object->getSaveString(GJBaseGameLayer::get());
}