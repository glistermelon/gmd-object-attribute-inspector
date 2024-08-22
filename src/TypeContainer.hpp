struct ColorHSV {

	int h;
	int s;
	int v;
	bool s_on;
	bool v_on;
	
	std::string hexCode() {
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

	TypeContainer() {
		m_type = ATTR_TYPE_UNKNOWN;
	}
	TypeContainer(int val) {
		m_intVal = val;
		m_type = ATTR_TYPE_INT;
	}
	TypeContainer(float val) {
		m_floatVal = val;
		m_type = ATTR_TYPE_FLOAT;
	}
	TypeContainer(bool val) {
		m_boolVal = val;
		m_type = ATTR_TYPE_BOOL;
	}
	TypeContainer(ColorHSV val) {
		m_colorVal = val;
		m_type = ATTR_TYPE_COLOR;
	}
	TypeContainer(std::string val) {
		m_strVal = val;
		m_type = ATTR_TYPE_STRING;
	}

	decltype(m_type) getType() { return m_type; }

	std::optional<int> getIntValue() { return m_type == ATTR_TYPE_INT ? m_intVal : std::optional<int>(); }
	std::optional<float> getFloatValue() { return m_type == ATTR_TYPE_FLOAT ? m_floatVal : std::optional<float>(); }
	std::optional<bool> getBoolValue() { return m_type == ATTR_TYPE_BOOL ? m_boolVal : std::optional<bool>(); }
	std::optional<ColorHSV> getColorValue() { return m_type == ATTR_TYPE_COLOR ? m_colorVal : std::optional<ColorHSV>(); }
	std::optional<std::string> getStringValue() { return m_type == ATTR_TYPE_STRING ? m_strVal : std::optional<std::string>(); }

	bool setValue(std::string unparsed) {

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

	std::string getValueLabel() {
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

	void setIntValue(int v) { m_intVal = v; m_type = ATTR_TYPE_INT; }
	void setFloatValue(float v) { m_floatVal = v; m_type = ATTR_TYPE_FLOAT; }
	void setBoolValue(bool v) { m_boolVal = v; m_type = ATTR_TYPE_BOOL; }
	void setColorValue(ColorHSV v) { m_colorVal = v; m_type = ATTR_TYPE_COLOR; }
	void setStringValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_STRING; }
	void setUnknownValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_UNKNOWN; }

	bool setColor(ColorHSV newColor) {
		if (m_type != ATTR_TYPE_COLOR) return false;
		m_colorVal = newColor;
		return true;
	}

};
