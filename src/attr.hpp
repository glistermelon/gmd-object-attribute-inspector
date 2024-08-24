#pragma once

#include "include.hpp"

#include <optional>

enum AttributeType {
	ATTR_TYPE_INT,
	ATTR_TYPE_FLOAT,
	ATTR_TYPE_BOOL,
	ATTR_TYPE_ARRAY,
	ATTR_TYPE_COLOR,
	ATTR_TYPE_STRING,
	ATTR_TYPE_UNKNOWN
};

namespace attrtype {
	
	inline std::vector<AttributeType> getTypes() {
		return {
			ATTR_TYPE_INT,
			ATTR_TYPE_FLOAT,
			ATTR_TYPE_BOOL,
			ATTR_TYPE_ARRAY,
			ATTR_TYPE_COLOR,
			ATTR_TYPE_STRING,
			ATTR_TYPE_UNKNOWN
		};
	}

	template <class String>
	inline std::vector<String> getTypeStrings() {
		return {
			"Integer",
			"Float",
			"Boolean",
			"Array",
			"Color",
			"String",
			"Unknown"
		};
	}

	gd::string getTypeLabel(AttributeType type);

	gd::string getTypeLabelShort(AttributeType type);
	
	ccColor3B getTypeLabelColor(AttributeType type);

	AttributeType fromJsonString(std::string s);

}