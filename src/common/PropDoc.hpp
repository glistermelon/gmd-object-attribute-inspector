#pragma once

#include "PropKey.hpp"
#include "PropValue.hpp"

#include <matjson.hpp>

struct PropDoc {

	static inline std::map<PropKey, PropDoc> s_propDocs;

	std::string m_name;
	std::optional<std::string> m_desc;
	PropValueType m_type;

	static std::optional<PropDoc> getDoc(PropKey key);

	static void loadDocs(matjson::Value docsJson);

};

template <> struct matjson::Serialize<PropDoc> {
	static geode::Result<PropDoc> fromJson(const matjson::Value& val) {
		auto doc = PropDoc();
		GEODE_UNWRAP_INTO(doc.m_name, val["name"].asString());
		if (!val["desc"].isNull()) {
			GEODE_UNWRAP_INTO(doc.m_desc, val["desc"].asString());
		}
		if (!val["type"].isNull()) {
			GEODE_UNWRAP_INTO(std::string typeStr, val["type"].asString());
			doc.m_type = getPropValueTypeFromString(typeStr);
		}
		return geode::Ok(doc);
	}
};