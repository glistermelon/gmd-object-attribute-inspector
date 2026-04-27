#include "PropValue.hpp"

#include "PropDoc.hpp"

geode::Result<bool> boolFromString(std::string str) {
    if (str == "0") return geode::Ok(false);
    else if (str == "1") return geode::Ok(true);
    else return geode::Err("invalid boolean string");
}

geode::Result<std::vector<int>> intArrayFromString(std::string str) {
    std::vector<std::string> intStrings = geode::utils::string::split(str, ".");
    std::vector<int> vals;
    for (auto& intStr : intStrings) {
        GEODE_UNWRAP_INTO(auto val, geode::utils::numFromString<int>(intStr));
        vals.push_back(val);
    }
    return geode::Ok(vals);
}

geode::Result<HSV> hsvFromString(std::string str) {
    std::vector<std::string> parts = geode::utils::string::split(str, "a");
    if (parts.size() != 5) return geode::Err("invalid hsv string");
    HSV hsv;
    GEODE_UNWRAP_INTO(hsv.h, geode::utils::numFromString<float>(parts[0]));
    GEODE_UNWRAP_INTO(hsv.s, geode::utils::numFromString<float>(parts[1]));
    GEODE_UNWRAP_INTO(hsv.v, geode::utils::numFromString<float>(parts[2]));
    GEODE_UNWRAP_INTO(hsv.absSat, boolFromString(parts[3]));
    GEODE_UNWRAP_INTO(hsv.absBri, boolFromString(parts[4]));
    return geode::Ok(hsv);
}

PropValueType getPropValueTypeFromString(std::string s) {
	s = geode::utils::string::toLower(s);
	s = geode::utils::string::trim(s);
	if (s == "int") return PropValueType::Int;
	else if (s == "double") return PropValueType::Double;
	else if (s == "float") return PropValueType::Float;
	else if (s == "bool") return PropValueType::Bool;
	else if (s == "int-array") return PropValueType::IntArray;
	else if (s == "hsv" || s == "color") return PropValueType::HSV;
	else if (s == "str" || s == "string") return PropValueType::String;
	else return PropValueType::Unknown;
}

std::string getPropValueTypeShortString(PropValueType type) {
	switch (type) {
        case PropValueType::Int:
            return "int";
		case PropValueType::Double:
			return "double";
        case PropValueType::Float:
            return "float";
        case PropValueType::Bool:
            return "bool";
        case PropValueType::IntArray:
            return "array";
        case PropValueType::HSV:
            return "hsv";
        case PropValueType::String:
            return "string";
        default:
            return "unknown";
    }
}

cocos2d::ccColor3B getPropValueTypeColor(PropValueType type) {
	switch (type) {
        case PropValueType::Int:
            return { 204, 51, 255 };  // purple
		case PropValueType::Double:
			return { 255, 51, 0 };    // red
        case PropValueType::Float:
            return { 255, 51, 0 };    // red
        case PropValueType::Bool:
            return { 0, 0, 255 };     // blue
        case PropValueType::IntArray:
            return { 255, 153, 51 };  // orange
        case PropValueType::HSV:
            return { 102, 255, 255 }; // bright blue
        case PropValueType::String:
            return { 51, 204, 51 };   // green
        default:
            return { 200, 200, 200 }; // grey
    }
}

PropValue PropValue::createFromInt(int val) {
    PropValue propVal;
    propVal.m_intVal = val;
    propVal.m_valType = PropValueType::Int;
    return propVal;
}

PropValue PropValue::createFromDouble(double val) {
    PropValue propVal;
    propVal.m_doubleVal = val;
    propVal.m_valType = PropValueType::Double;
    return propVal;
}

PropValue PropValue::createFromFloat(float val) {
    PropValue propVal;
    propVal.m_floatVal = val;
    propVal.m_valType = PropValueType::Float;
    return propVal;
}

PropValue PropValue::createFromBool(bool val) {
    PropValue propVal;
    propVal.m_boolVal = val;
    propVal.m_valType = PropValueType::Bool;
    return propVal;
}

PropValue PropValue::createFromIntArray(std::vector<int> val) {
    PropValue propVal;
    propVal.m_intArrayVal = val;
    propVal.m_valType = PropValueType::IntArray;
    return propVal;
}

PropValue PropValue::createFromHSV(HSV val) {
    PropValue propVal;
    propVal.m_hsvVal = val;
    propVal.m_valType = PropValueType::HSV;
    return propVal;
}

PropValue PropValue::createFromString(std::string val) {
    PropValue propVal;
    propVal.m_strVal = val;
    propVal.m_valType = PropValueType::String;
    return propVal;
}

PropValue PropValue::createFromUnknown(std::string val) {
    PropValue propVal;
    propVal.m_unkVal = val;
    propVal.m_valType = PropValueType::Unknown;
    return propVal;
}

geode::Result<PropValue> PropValue::createFromPropStringWithType(
    PropKey key, PropValueType type, std::string str
) {
    switch (type) {
        case PropValueType::Int: {
            GEODE_UNWRAP_INTO(auto val, geode::utils::numFromString<int>(str));
            return geode::Ok(PropValue::createFromInt(val));
        }
        case PropValueType::Double: {
            GEODE_UNWRAP_INTO(auto val, geode::utils::numFromString<double>(str));
            return geode::Ok(PropValue::createFromDouble(val));
        }
        case PropValueType::Float: {
            GEODE_UNWRAP_INTO(auto val, geode::utils::numFromString<float>(str));
            return geode::Ok(PropValue::createFromFloat(val));
        }
        case PropValueType::Bool: {
            GEODE_UNWRAP_INTO(auto val, boolFromString(str));
            return geode::Ok(PropValue::createFromBool(val));
        }
        case PropValueType::IntArray: {
            GEODE_UNWRAP_INTO(auto val, intArrayFromString(str));
            return geode::Ok(PropValue::createFromIntArray(val));
        }
        case PropValueType::HSV: {
            GEODE_UNWRAP_INTO(auto val, hsvFromString(str));
            return geode::Ok(PropValue::createFromHSV(val));
        }
        case PropValueType::String: {
            return geode::Ok(PropValue::createFromString(str));
        }
        default:
            return geode::Err("invalid prop value type");
    }
}

PropValue PropValue::createFromPropString(PropKey key, std::string str) {
    std::optional<PropDoc> maybeDoc = PropDoc::getDoc(key);
    if (maybeDoc.has_value()) {
        auto doc = maybeDoc.value();
        auto result = PropValue::createFromPropStringWithType(key, doc.m_type, str);
        if (result.isOk()) return result.unwrap();
    }
    return PropValue::createFromUnknown(str);
}
    
std::string PropValue::getString() {
    switch (m_valType) {
        case PropValueType::Int:
            return geode::utils::numToString(m_intVal);
        case PropValueType::Double:
            return geode::utils::numToString(m_doubleVal);
        case PropValueType::Float:
            return geode::utils::numToString(m_floatVal);
        case PropValueType::Bool:
            return m_boolVal ? "true" : "false";
        case PropValueType::IntArray:
            return fmt::format("{}", fmt::join(m_intArrayVal, ", "));
        case PropValueType::HSV:
            return fmt::format(
                "{}a{}a{}a{}a{}",
                m_hsvVal.h,
                m_hsvVal.s,
                m_hsvVal.v,
                m_hsvVal.absSat,
                m_hsvVal.absBri
            );
        case PropValueType::String:
            return m_strVal;
        case PropValueType::Unknown:
            return m_unkVal;
    }
}

std::string PropValue::getPrettyString() {
    if (m_valType == PropValueType::HSV) {
        return fmt::format(
            "{}, {}{}, {}{}",
            m_hsvVal.h,
            m_hsvVal.absSat ? "+" : "x", m_hsvVal.s,
            m_hsvVal.absBri ? "+" : "x", m_hsvVal.v
        );
    }
    else {
        return this->getString();
    }
}

matjson::Value PropValue::getJson() {
    switch (m_valType) {
        case PropValueType::Int:
            return m_intVal;
        case PropValueType::Double:
            return m_doubleVal;
        case PropValueType::Float:
            return m_floatVal;
        case PropValueType::Bool:
            return m_boolVal;
        case PropValueType::IntArray:
            return m_intArrayVal;
        case PropValueType::HSV: {
            matjson::Value json;
            json["h"] = m_hsvVal.h;
            json["s"] = m_hsvVal.s;
            json["v"] = m_hsvVal.v;
            json["abs-sat"] = m_hsvVal.absSat;
            json["abs-bri"] = m_hsvVal.absBri;
            return json;
        }
        case PropValueType::String:
            return m_strVal;
        case PropValueType::Unknown:
            return m_unkVal;
    }
}

geode::Result<HSV> PropValue::getHSV() {
    if (m_valType == PropValueType::HSV) {
        return geode::Ok(m_hsvVal);
    }
    return geode::Err("prop value is not hsv");
}