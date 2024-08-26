#include "GameObjectWrapper.hpp"

#include <matjson.hpp>

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