#include <Geode/Geode.hpp>

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

	gd::string getTypeLabel(AttributeType type);

	gd::string getTypeLabelShort(AttributeType type);
	
	ccColor3B getTypeLabelColor(AttributeType type);

	AttributeType fromJsonString(std::string s);

}