#pragma once

#include <string>
#include <compare>

struct PropKey {

	int m_intKey;
	std::string m_strKey;

	bool m_isInt;

	PropKey(int intKey);
	PropKey(std::string strKey);

	static PropKey createFromPropString(std::string str);

	std::string getString();

	std::strong_ordering operator<=>(const PropKey& other) const;

};