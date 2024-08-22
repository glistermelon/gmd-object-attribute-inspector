#pragma once

#include "attr.hpp"

#include <string>
#include <map>

class AttributeDocs {

	friend struct matjson::Serialize<AttributeDocs>;

	std::string m_name = "Object ID"; // todo: get rid of this
	std::string m_description = "this is a description"; // todo: get rid of this
	AttributeType m_type = ATTR_TYPE_INT; // todo: change to ATTR_TYPE_UNKNOWN

public:

	static std::map<int, AttributeDocs> attributeDocs;

	inline static AttributeDocs* getDocs(int attr) {
		auto it = attributeDocs.find(attr);
		return it == attributeDocs.end() ? nullptr : &it->second;
	}

	std::string getName() { return m_name; }
	std::string getDescription() { return m_description; }
	AttributeType getType() { return m_type; }

};

template <> struct matjson::Serialize<AttributeDocs> {
	static AttributeDocs from_json(const matjson::Value& val) {
		auto doc = AttributeDocs();
		doc.m_name = val["name"].as_string();
		doc.m_description = val["desc"].is_null() ? "" : val["desc"].as_string();
		doc.m_type = val["type"].is_null() ? ATTR_TYPE_UNKNOWN : attrtype::fromJsonString(val["type"].as_string());
		return doc;
	}
};