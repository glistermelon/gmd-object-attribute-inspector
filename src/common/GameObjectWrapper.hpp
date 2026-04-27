#pragma once

#include "PropKey.hpp"
#include "PropValue.hpp"

#include <Geode/binding/GameObject.hpp>

class GameObjectWrapper {

    geode::Ref<GameObject> m_object;

    std::map<std::string, std::string> getRawPropMap();

    geode::Result<> setPropMap(std::map<std::string, std::string> propMap);

public:

    GameObjectWrapper(GameObject* object);

    GameObject* getGameObject();

    std::string getObjectString();

    std::optional<PropValue> getPropValue(PropKey key);

    std::vector<PropKey> getPropKeys();

    matjson::Value getPropMapJson();

    geode::Result<> setProp(std::string key, std::string val);
    geode::Result<> deleteProp(std::string key);

};