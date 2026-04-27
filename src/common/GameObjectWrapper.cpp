#include "GameObjectWrapper.hpp"

std::map<std::string, std::string> GameObjectWrapper::getRawPropMap() {
    std::string propsStr = this->getObjectString();
    std::vector<std::string> parts = geode::utils::string::split(propsStr, ",");
    std::map<std::string, std::string> propMap;
    for (size_t i = 0, size = parts.size(); i + 1 < size; i += 2) {
        std::string keyStr = parts[i];
        std::string valStr = parts[i + 1];
        propMap[keyStr] = valStr;
    }
    return propMap;
}

GameObjectWrapper::GameObjectWrapper(GameObject* object) {
    m_object = object;
}

GameObject* GameObjectWrapper::getGameObject() {
    return m_object;
}

std::string GameObjectWrapper::getObjectString() {
    return m_object->getSaveString(GJBaseGameLayer::get());
}

std::optional<PropValue> GameObjectWrapper::getPropValue(PropKey key) {
    auto map = this->getRawPropMap();
    auto keyStr = key.getString();
    if (map.find(keyStr) == map.end()) {
        return std::nullopt;
    }
    else {
        return PropValue::createFromPropString(key, map[keyStr]);
    }
}

std::vector<PropKey> GameObjectWrapper::getPropKeys() {
    std::vector<PropKey> props;
    for (auto& [keyStr, valStr] : this->getRawPropMap()) {
        props.push_back(PropKey::createFromPropString(keyStr));
    }
    std::sort(props.begin(), props.end());
    return props;
}

matjson::Value GameObjectWrapper::getPropMapJson() {
    matjson::Value json;
    for (auto& [keyStr, valStr] : this->getRawPropMap()) {
        auto propKey = PropKey::createFromPropString(keyStr);
        auto propVal = PropValue::createFromPropString(propKey, valStr);
        json[keyStr] = propVal.getJson();
    }
    return json;
}

geode::Result<> GameObjectWrapper::setPropMap(std::map<std::string, std::string> propMap) {

    std::vector<std::string> strParts;
    for (auto [key, val] : propMap) {
        strParts.push_back(key);
        strParts.push_back(val);
    }
    std::string newObjectStr = fmt::format("{}", fmt::join(strParts, ","));

    geode::cocos::CCArrayExt<GameObject*> created =
        LevelEditorLayer::get()->createObjectsFromString(newObjectStr, true, true);
    if (created.empty()) return geode::Err("recreate failed");

    auto editorUI = EditorUI::get();

    geode::cocos::CCArrayExt<GameObject*> selectedObjects;
    if (!editorUI->m_selectedObject) {
        for (auto object : geode::cocos::CCArrayExt<GameObject*>(editorUI->getSelectedObjects())) {
            if (object != m_object) {
                selectedObjects.push_back(object);
            }
        }
    }

    editorUI->deselectAll();
    editorUI->deleteObject(m_object, true);

    m_object = created[0];
    selectedObjects.push_back(m_object);
    editorUI->selectObjects(selectedObjects.inner(), true);
    
    return geode::Ok();

}

geode::Result<> GameObjectWrapper::setProp(std::string setKey, std::string setVal) {
    auto propMap = this->getRawPropMap();
    propMap[setKey] = setVal;
    return this->setPropMap(propMap);
}

geode::Result<> GameObjectWrapper::deleteProp(std::string key) {
    auto propMap = this->getRawPropMap();
    propMap.erase(key);
    return this->setPropMap(propMap);
}