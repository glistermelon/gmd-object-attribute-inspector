#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

#define c3BFromC4B(ccc4B) cocos2d::ccColor3B({ ccc4B.r, ccc4B.g, ccc4B.b })

constexpr cocos2d::ccColor4B LIGHTER_BROWN_4B({ 0xBF, 0x72, 0x3E, 0xff });
constexpr cocos2d::ccColor3B LIGHTER_BROWN_3B = c3BFromC4B(LIGHTER_BROWN_4B);
constexpr cocos2d::ccColor4B LIGHT_BROWN_4B({ 0xB0, 0x5C, 0x34, 0xff });
constexpr cocos2d::ccColor3B LIGHT_BROWN_3B = c3BFromC4B(LIGHT_BROWN_4B);
constexpr cocos2d::ccColor4B BROWN_4B({ 0xA0, 0x54, 0x34, 0xff });
constexpr cocos2d::ccColor3B BROWN_3B = c3BFromC4B(BROWN_4B);
constexpr cocos2d::ccColor4B DARK_BROWN_4B({ 0x82, 0x40, 0x21, 0xff });
constexpr cocos2d::ccColor3B DARK_BROWN_3B = c3BFromC4B(DARK_BROWN_4B);
constexpr cocos2d::ccColor4F DARK_BROWN_4F({ (float)DARK_BROWN_4B.r / 255.f, (float)DARK_BROWN_4B.g / 255.f, (float)DARK_BROWN_4B.b / 255.f, (float)DARK_BROWN_4B.a / 255.f });