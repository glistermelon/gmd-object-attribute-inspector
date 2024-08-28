#pragma once

#include "include.hpp"
#include "attr.hpp"

struct ColorHSV {

	float h;
	float s;
	float v;
	bool s_on;
	bool v_on;
	
	inline std::string str() {
		std::stringstream ss;
		ss << h << ", ";
		ss << (s_on ? "+" : "x") << s << ", ";
		ss << (v_on ? "+" : "x") << v;
		return ss.str();
	}

	inline ColorHSV() = default;

	inline ColorHSV(float h, float s, float v, bool s_on, bool v_on) : h(h), s(s), v(v), s_on(s_on), v_on(v_on) {}

	inline ColorHSV(ccHSVValue cchsv) : h(cchsv.h), s(cchsv.s), v(cchsv.v), s_on(cchsv.absoluteSaturation), v_on(cchsv.absoluteBrightness) {}

	inline ccHSVValue cchsv() const {
		return { h, s, v, s_on, v_on };
	}

};

class ValueContainer {

	union {
		int m_intVal;
		float m_floatVal;
		bool m_boolVal;
		ColorHSV m_colorVal;
	};
	std::string m_strVal;

protected:

	AttributeType m_type;

public:

	inline ValueContainer() {
		m_type = ATTR_TYPE_UNKNOWN;
	}
	inline ValueContainer(AttributeType type) {
		m_type = type;
	}
	inline ValueContainer(int val) {
		m_intVal = val;
		m_type = ATTR_TYPE_INT;
	}
	inline ValueContainer(float val) {
		m_floatVal = val;
		m_type = ATTR_TYPE_FLOAT;
	}
	inline ValueContainer(bool val) {
		m_boolVal = val;
		m_type = ATTR_TYPE_BOOL;
	}
	inline ValueContainer(ColorHSV val) {
		m_colorVal = val;
		m_type = ATTR_TYPE_COLOR;
	}
	inline ValueContainer(std::string val) {
		m_strVal = val;
		m_type = ATTR_TYPE_STRING;
	}

	inline decltype(m_type) getType() { return m_type; }

	inline std::optional<int> getIntValue() { return m_type == ATTR_TYPE_INT ? m_intVal : std::optional<int>(); }
	inline std::optional<float> getFloatValue() { return m_type == ATTR_TYPE_FLOAT ? m_floatVal : std::optional<float>(); }
	inline std::optional<bool> getBoolValue() { return m_type == ATTR_TYPE_BOOL ? m_boolVal : std::optional<bool>(); }
	inline std::optional<ColorHSV> getColorValue() { return m_type == ATTR_TYPE_COLOR ? m_colorVal : std::optional<ColorHSV>(); }
	inline std::optional<std::string> getStringValue() { return m_type == ATTR_TYPE_STRING ? m_strVal : std::optional<std::string>(); }

	inline void setIntValue(int v) { m_intVal = v; m_type = ATTR_TYPE_INT; }
	inline void setFloatValue(float v) { m_floatVal = v; m_type = ATTR_TYPE_FLOAT; }
	inline void setBoolValue(bool v) { m_boolVal = v; m_type = ATTR_TYPE_BOOL; }
	inline void setColorValue(ColorHSV v) { m_colorVal = v; m_type = ATTR_TYPE_COLOR; }
	inline void setStringValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_STRING; }
	inline void setUnknownValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_UNKNOWN; }

	bool setValue(std::string unparsed, bool force = false, AttributeType type = ATTR_TYPE_UNKNOWN);

	std::string getValueLabel();

	gd::string getRaw() const;

};
