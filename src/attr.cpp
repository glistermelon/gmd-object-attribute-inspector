#include "attr.hpp"

#include <string>

using namespace attrtype;

gd::string getTypeLabel(AttributeType type) {
    switch (type) {
        case ATTR_TYPE_INT:
            return "integer";
        case ATTR_TYPE_FLOAT:
            return "float";
        case ATTR_TYPE_BOOL:
            return "boolean";
        case ATTR_TYPE_ARRAY:
            return "array";
        case ATTR_TYPE_COLOR:
            return "color";
        case ATTR_TYPE_STRING:
            return "string";
        default:
            return "unknown";
    }
}

gd::string getTypeLabelShort(AttributeType type) {
    switch (type) {
        case ATTR_TYPE_INT:
            return "int";
        case ATTR_TYPE_FLOAT:
            return "float";
        case ATTR_TYPE_BOOL:
            return "bool";
        case ATTR_TYPE_ARRAY:
            return "array";
        case ATTR_TYPE_COLOR:
            return "color";
        case ATTR_TYPE_STRING:
            return "string";
        default:
            return "unknown";
    }
}

ccColor3B getTypeLabelColor(AttributeType type) {
    switch (type) {
        case ATTR_TYPE_INT:
            return { 204, 51, 255 };  // purple
        case ATTR_TYPE_FLOAT:
            return { 255, 51, 0 };    // red
        case ATTR_TYPE_BOOL:
            return { 0, 0, 255 };     // blue
        case ATTR_TYPE_ARRAY:
            return { 255, 153, 51 };  // orange
        case ATTR_TYPE_COLOR:
            return { 102, 255, 255 }; // bright blue
        case ATTR_TYPE_STRING:
            return { 51, 204, 51 };   // green
        default:
            return { 200, 200, 200 }; // grey
    }
}

AttributeType fromJsonString(std::string s) {
    if (s == "int") return ATTR_TYPE_INT;
    if (s == "float") return ATTR_TYPE_FLOAT;
    if (s == "bool") return ATTR_TYPE_BOOL;
    if (s == "array") return ATTR_TYPE_ARRAY;
    if (s == "color") return ATTR_TYPE_COLOR;
    if (s == "string") return ATTR_TYPE_STRING;
    geode::log::warn("fromJsonString encountered unexpected type string: {0}", s);
    return ATTR_TYPE_UNKNOWN;
}