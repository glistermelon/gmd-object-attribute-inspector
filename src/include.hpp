#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define ccc3BFromccc4B(ccc4B) ccColor3B({ ccc4B.r, ccc4B.g, ccc4B.b })

constexpr ccColor4B LIGHTER_BROWN_4B({ 0xBF, 0x72, 0x3E, 0xff });
constexpr ccColor3B LIGHTER_BROWN_3B = ccc3BFromccc4B(LIGHTER_BROWN_4B);
constexpr ccColor4B LIGHT_BROWN_4B({ 0xB0, 0x5C, 0x34, 0xff });
constexpr ccColor3B LIGHT_BROWN_3B = ccc3BFromccc4B(LIGHT_BROWN_4B);
constexpr ccColor4B BROWN_4B({ 0xA0, 0x54, 0x34, 0xff });
constexpr ccColor3B BROWN_3B = ccc3BFromccc4B(BROWN_4B);
constexpr ccColor4B DARK_BROWN_4B({ 0x82, 0x40, 0x21, 0xff });
constexpr ccColor3B DARK_BROWN_3B = ccc3BFromccc4B(DARK_BROWN_4B);
constexpr ccColor4F DARK_BROWN_4F({ (float)DARK_BROWN_4B.r / 255.f, (float)DARK_BROWN_4B.g / 255.f, (float)DARK_BROWN_4B.b / 255.f, (float)DARK_BROWN_4B.a / 255.f });
constexpr float PADDING_HORIZONTAL = 50.f;
constexpr float PADDING_VERTICAL = 50.f;

CCPoint getPositionInNode(CCNode* spatialNode, CCNode* nodeToGetPositionOf);

CCRect getRectInNode(CCNode* spatialNode, CCNode* rectNode);