#include "ValueContainer.hpp"

#include <string>

bool ValueContainer::setValue(std::string unparsed, bool force, AttributeType type) {

    if (!force) type = m_type;

    auto stripBegin = unparsed.begin();
    auto stripEnd = unparsed.rbegin();
    while (stripBegin != unparsed.end() && std::isspace(*stripBegin)) ++stripBegin;
    while (stripEnd != unparsed.rend() && std::isspace(*stripEnd)) ++stripEnd;
    unparsed = std::string(stripBegin, stripEnd.base());
    if (unparsed.empty()) return false;
    for (auto it = unparsed.begin(); it != unparsed.end(); ++it) {
        if (std::isupper(*it)) *it = std::tolower(*it);
    }

    if (type == ATTR_TYPE_INT || type == ATTR_TYPE_FLOAT) {
        try {
            if (type == ATTR_TYPE_INT) m_intVal = std::stoi(unparsed);
            else m_floatVal = std::stof(unparsed);
            if (force) m_type = type;
            return true;
        }
        catch (std::invalid_argument) {
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }
    }
    else if (type == ATTR_TYPE_BOOL) {
        if (unparsed == "true" || unparsed == "1") m_boolVal = true;
        else if (unparsed == "0" || unparsed == "-1") m_boolVal = false;
        else return false;
        if (force) m_type = type;
        return true;
    }
    else if (type == ATTR_TYPE_COLOR) {
        try {
            std::stringstream ss(unparsed);
            std::string s;
            ColorHSV hsv;
            constexpr char splitChar = 'a';
            std::getline(ss, s, splitChar);
            hsv.h = std::stof(s);
            std::getline(ss, s, splitChar);
            hsv.s = std::stof(s);
            std::getline(ss, s, splitChar);
            hsv.v = std::stof(s);
            std::getline(ss, s, splitChar);
            hsv.s_on = (bool)std::stoi(s);
            std::getline(ss, s, splitChar);
            hsv.v_on = (bool)std::stoi(s);
            m_colorVal = hsv;
            if (force) m_type = type;
            return true;
        }
        catch (std::invalid_argument) {
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }
    }
    else {
        m_strVal = unparsed;
        if (force) m_type = type;
        return true;
    }

}

std::string ValueContainer::getValueLabel() {
    switch (m_type) {
        case ATTR_TYPE_INT:
            return std::to_string(m_intVal);
        case ATTR_TYPE_FLOAT:
            return std::to_string(m_floatVal);
        case ATTR_TYPE_BOOL:
            return m_boolVal ? "true" : "false";
        case ATTR_TYPE_COLOR:
            return m_colorVal.str();
        default:
            return m_strVal;
    }
}

gd::string ValueContainer::getRaw() const {
    switch (m_type) {
        case ATTR_TYPE_UNKNOWN:
        case ATTR_TYPE_ARRAY:
        case ATTR_TYPE_STRING:
            return m_strVal;
        case ATTR_TYPE_COLOR: {
            gd::string out;
            GameToolbox::stringFromHSV(out, m_colorVal.cchsv());
            return out;
        }
        case ATTR_TYPE_INT:
            return fmt::format("{0}", m_intVal);
        case ATTR_TYPE_BOOL:
            return fmt::format("{0}", (unsigned int)m_boolVal);
        case ATTR_TYPE_FLOAT:
            return fmt::format("{0}", (double)m_floatVal);
        default:
            return "";
    }
}