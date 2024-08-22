#include "TypeContainer.hpp"

#include <string>

bool TypeContainer::setValue(std::string unparsed) {

    auto stripBegin = unparsed.begin();
    auto stripEnd = unparsed.rbegin();
    while (stripBegin != unparsed.end() && std::isspace(*stripBegin)) ++stripBegin;
    while (stripEnd != unparsed.rend() && std::isspace(*stripEnd)) ++stripEnd;
    unparsed = std::string(stripBegin, stripEnd.base());
    if (unparsed.empty()) return false;
    for (auto it = unparsed.begin(); it != unparsed.end(); ++it) {
        if (std::isupper(*it)) *it = std::tolower(*it);
    }

    if (m_type == ATTR_TYPE_INT || m_type == ATTR_TYPE_FLOAT) {
        try {
            if (m_type == ATTR_TYPE_INT) m_intVal = std::stoi(unparsed);
            else m_floatVal = std::stof(unparsed);
            return true;
        }
        catch (std::invalid_argument) {
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }
    }
    else if (m_type == ATTR_TYPE_BOOL) {
        if (unparsed == "true" || unparsed == "1") m_boolVal = true;
        else if (unparsed == "0" || unparsed == "-1") m_boolVal = false;
        else return false;
        return true;
    }
    else if (m_type == ATTR_TYPE_COLOR) return false; // use setColor
    else {
        m_strVal = unparsed;
        return true;
    }

}

std::string TypeContainer::getValueLabel() {
    switch (m_type) {
        case ATTR_TYPE_INT:
            return std::to_string(m_intVal);
        case ATTR_TYPE_FLOAT:
            return std::to_string(m_floatVal);
        case ATTR_TYPE_BOOL:
            return m_boolVal ? "true" : "false";
        case ATTR_TYPE_COLOR:
            return m_colorVal.hexCode();
        default:
            return m_strVal;
    }
}