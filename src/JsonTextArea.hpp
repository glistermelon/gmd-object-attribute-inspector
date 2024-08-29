#pragma once

#include "include.hpp"

class JsonTextArea : public MDTextArea {

    matjson::Value m_json;
    ccColor3B m_bgColor;
    std::string m_bgColorHex;
    CCLayerColor* m_bgLayer = nullptr;

    std::string getIndent(int indentLevel);
    std::string dumpJsonMarkdown(matjson::Value json, int indentLevel = 0);

    void updateColorHex(ccColor3B);

    bool init(matjson::Value json, CCSize size);

    void updateText();

public:

    static JsonTextArea* create(matjson::Value json, CCSize size);

    void setJson(matjson::Value);

    void setBgColor(ccColor3B);

    void updateBgColor();

    void setBgLayer(CCLayerColor*);

};