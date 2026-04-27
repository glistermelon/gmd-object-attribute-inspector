#pragma once

#include "PropKey.hpp"

#include <ccTypes.h>

enum class PropValueType {
	Int,
	Double,
	Float,
	Bool,
	IntArray,
	HSV,
	String,
	Unknown
};

PropValueType getPropValueTypeFromString(std::string s);
std::string getPropValueTypeShortString(PropValueType type);
cocos2d::ccColor3B getPropValueTypeColor(PropValueType type);

struct HSV {

	float h;
	float s;
	float v;
	bool absSat;
	bool absBri;

	inline cocos2d::ccHSVValue asCCHSV() {
		return { h, s, v, absSat, absBri };
	}

	inline static HSV fromCCHSV(cocos2d::ccHSVValue ccHSV) {
		return {
			ccHSV.h, ccHSV.s, ccHSV.v,
			ccHSV.absoluteSaturation, ccHSV.absoluteBrightness
		};
	}

};

class PropValue {

	PropValueType m_valType;

	int m_intVal;
	double m_doubleVal;
	float m_floatVal;
	bool m_boolVal;
	std::vector<int> m_intArrayVal;
	HSV m_hsvVal;
	std::string m_strVal;
	std::string m_unkVal;

	static PropValue createFromInt(int val);
	static PropValue createFromDouble(double val);
	static PropValue createFromFloat(float val);
	static PropValue createFromBool(bool val);
	static PropValue createFromIntArray(std::vector<int> val);
	static PropValue createFromHSV(HSV val);
	static PropValue createFromString(std::string val);
	static PropValue createFromUnknown(std::string val);

	static geode::Result<PropValue> createFromPropStringWithType(PropKey key, PropValueType type, std::string str);

public:

	static PropValue createFromPropString(PropKey key, std::string str);

	std::string getString();
	std::string getPrettyString();
	matjson::Value getJson();
	geode::Result<HSV> getHSV();

};