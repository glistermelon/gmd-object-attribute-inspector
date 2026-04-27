#include "PropKey.hpp"

#include <Geode/utils/general.hpp>

int findIntInString(std::string str) {
    auto begin = std::find_if(str.begin(), str.end(), [](char c) { return std::isdigit(c); });
    if (begin == str.end()) return 0;
    auto end = begin;
    while (end != str.end() && std::isdigit(*end)) {
        ++end;
    }
    return geode::utils::numFromString<int>(std::string_view(begin, end)).unwrapOr(0);
}

PropKey::PropKey(int intKey) : m_intKey(intKey), m_strKey(""), m_isInt(true) { }

PropKey::PropKey(std::string strKey) : m_intKey(0), m_strKey(strKey), m_isInt(false) { }

PropKey PropKey::createFromPropString(std::string str) {
    str = geode::utils::string::trim(str);
    if (std::ranges::all_of(str, [](char c) { return std::isdigit(c); })) {
        if (auto result = geode::utils::numFromString<int>(str); result.isOk()) {
            return PropKey(result.unwrap());
        }
    }
    return PropKey(str);
}

std::string PropKey::getString() {
    return m_isInt ? geode::utils::numToString(m_intKey) : m_strKey;
}

std::strong_ordering PropKey::operator<=>(const PropKey& other) const {
    if (m_isInt && other.m_isInt) return m_intKey <=> other.m_intKey;
    else if (m_isInt && !other.m_isInt) return std::strong_ordering::less;
    else if (!m_isInt && other.m_isInt) return std::strong_ordering::greater;
    else return findIntInString(m_strKey) <=> findIntInString(other.m_strKey);
}