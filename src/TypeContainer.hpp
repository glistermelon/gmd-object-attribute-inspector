#pragma once

#include "attr.hpp"

struct ColorHSV {

	int h;
	int s;
	int v;
	bool s_on;
	bool v_on;
	
	inline std::string hexCode() {
		std::stringstream ss;
		ss << "todo"; // https://docs.geode-sdk.org/classes/geode/ColorPickPopup maybe
		return ss.str();
	}

};

class TypeContainer {

	union {
		int m_intVal;
		float m_floatVal;
		bool m_boolVal;
		ColorHSV m_colorVal;
	};
	std::string m_strVal; // also used for unknown type

protected:

	AttributeType m_type;

public:

	inline TypeContainer() {
		m_type = ATTR_TYPE_UNKNOWN;
	}
	inline TypeContainer(int val) {
		m_intVal = val;
		m_type = ATTR_TYPE_INT;
	}
	inline TypeContainer(float val) {
		m_floatVal = val;
		m_type = ATTR_TYPE_FLOAT;
	}
	inline TypeContainer(bool val) {
		m_boolVal = val;
		m_type = ATTR_TYPE_BOOL;
	}
	inline TypeContainer(ColorHSV val) {
		m_colorVal = val;
		m_type = ATTR_TYPE_COLOR;
	}
	inline TypeContainer(std::string val) {
		m_strVal = val;
		m_type = ATTR_TYPE_STRING;
	}

	inline decltype(m_type) getType() { return m_type; }

	inline std::optional<int> getIntValue() { return m_type == ATTR_TYPE_INT ? m_intVal : std::optional<int>(); }
	inline std::optional<float> getFloatValue() { return m_type == ATTR_TYPE_FLOAT ? m_floatVal : std::optional<float>(); }
	inline std::optional<bool> getBoolValue() { return m_type == ATTR_TYPE_BOOL ? m_boolVal : std::optional<bool>(); }
	inline std::optional<ColorHSV> getColorValue() { return m_type == ATTR_TYPE_COLOR ? m_colorVal : std::optional<ColorHSV>(); }
	inline std::optional<std::string> getStringValue() { return m_type == ATTR_TYPE_STRING ? m_strVal : std::optional<std::string>(); }

	bool setValue(std::string unparsed);

	std::string getValueLabel();

	inline void setIntValue(int v) { m_intVal = v; m_type = ATTR_TYPE_INT; }
	inline void setFloatValue(float v) { m_floatVal = v; m_type = ATTR_TYPE_FLOAT; }
	inline void setBoolValue(bool v) { m_boolVal = v; m_type = ATTR_TYPE_BOOL; }
	inline void setColorValue(ColorHSV v) { m_colorVal = v; m_type = ATTR_TYPE_COLOR; }
	inline void setStringValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_STRING; }
	inline void setUnknownValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_UNKNOWN; }

	inline bool setColor(ColorHSV newColor) {
		if (m_type != ATTR_TYPE_COLOR) return false;
		m_colorVal = newColor;
		return true;
	}

};
