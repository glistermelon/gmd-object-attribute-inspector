#include "JsonTextArea.hpp"

constexpr std::string_view mdNewline = "\\\n";

std::string JsonTextArea::getIndent(int indentLevel) {
    if (indentLevel < 0) return "";
    std::string indent = "<c-" + m_bgColorHex + ">";
    for (int i = 0; i <= indentLevel; ++i) indent.append("XXX");
    indent.append("</c>");
    return indent;
}

std::string JsonTextArea::dumpJsonMarkdown(matjson::Value json, int indentLevel) {

    if (!(json.is_array() || json.is_object())) {
        std::string s;
        if (json.is_string()) s = "<cg>";
        else if (json.is_number()) s = "<cr>";
        else if (json.is_bool()) s = "<cj>";
        else if (json.is_null()) s = "<cb>";
        return s + json.dump() + "</c>";
    }

    const char* brackets = json.is_object() ? "{}" : "[]";

    std::string indent = getIndent(indentLevel);

    std::stringstream jsonStream;

    jsonStream << brackets[0] << mdNewline;

    std::vector<std::string> keys;
    std::vector<matjson::Value> values;
    
    if (json.is_object()) {
        for (auto [key, value] : json.as_object()) {
            keys.push_back(key);
            values.push_back(value);
        }
    }
    else values = json.as_array();

    size_t iterCount = values.size();
    for (size_t i = 0; i < iterCount; ++i) {
        jsonStream << indent;
        if (json.is_object()) jsonStream << "\"" << keys[i] << "\": ";
        jsonStream << dumpJsonMarkdown(values[i], indentLevel + 1);
        if (i + 1 != iterCount) jsonStream << ",";
        jsonStream << mdNewline;
    }

    jsonStream << getIndent(indentLevel - 1) << brackets[1];

    return jsonStream.str();

}

void JsonTextArea::updateColorHex(ccColor3B color) {
    std::stringstream bgColorHex;
    unsigned int rgb[] = { color.r, color.g, color.b };
    for (int i = 0; i < 3; ++i) {
        std::stringstream hex;
        hex << std::hex << rgb[i];
        std::string s = hex.str();
        if (s.size() == 1) bgColorHex << '0';
        bgColorHex << s;
    }
    m_bgColorHex = bgColorHex.str();
}

bool JsonTextArea::init(matjson::Value json, CCSize size) {

    if (!MDTextArea::init("", size)) return false;

    MDTextArea::m_bgSprite->setOpacity(0);

    const char* fontLocal = "bigFont.fnt";
    const char* font = new char[std::strlen(fontLocal)];
    std::strcpy((char*)font, fontLocal);
    MDTextArea::m_renderer->pushBMFont(font);

    m_json = json;
    this->updateColorHex(MDTextArea::m_bgSprite->getColor());
    this->updateText();

    return true;

}

JsonTextArea* JsonTextArea::create(matjson::Value json, CCSize size) {
    auto obj = new JsonTextArea();
    if (obj->init(json, size)) {
        obj->autorelease();
        return obj;
    }
    delete obj;
    return nullptr;
}

inline void JsonTextArea::setJson(matjson::Value json) {
    m_json = json;
    this->updateText();
}

void JsonTextArea::setBgColor(ccColor3B color) {
    m_bgColor = color;
    if (!m_bgLayer) return;
    m_bgLayer->setColor(color);
    this->updateColorHex(color);
    this->updateText();
}

void JsonTextArea::updateBgColor() {
    m_bgLayer->setColor(m_bgColor);
}

void JsonTextArea::updateText() {
    std::string s = dumpJsonMarkdown(m_json);
    this->setString(s.c_str());
}