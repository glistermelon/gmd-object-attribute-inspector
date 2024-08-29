#pragma once

#include "attributes.hpp"

struct AttributeDocs {

	// todo:: make attribute docs private with initialization function (instead of main.cpp)

	static std::map<int, AttributeDocs> attributeDocs;

	std::string m_name;
	std::string m_description;
	AttributeType m_type;

	inline static AttributeDocs* getDocs(int attr) {
		auto it = attributeDocs.find(attr);
		return it == attributeDocs.end() ? nullptr : &it->second;
	}

};

template <> struct matjson::Serialize<AttributeDocs> {
	static AttributeDocs from_json(const matjson::Value& val) {
		auto doc = AttributeDocs();
		doc.m_name = val["name"].as_string();
		doc.m_description = val["desc"].is_null() ? "" : val["desc"].as_string();
		doc.m_type = val["type"].is_null() ? ATTR_TYPE_UNKNOWN : attrtype::typeFromShortLabel(val["type"].as_string());
		return doc;
	}
};