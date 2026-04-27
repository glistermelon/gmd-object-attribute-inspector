#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>

inline cocos2d::CCRect getRectInNode(cocos2d::CCNode* spatialNode, cocos2d::CCNode* rectNode) {
	cocos2d::CCPoint bottomLeft = rectNode->convertToWorldSpace(ccp(0.f, 0.f));
	cocos2d::CCPoint topRight = rectNode->convertToWorldSpace(ccp(rectNode->getContentWidth(), rectNode->getContentHeight()));
	bottomLeft = spatialNode->convertToNodeSpace(bottomLeft);
	topRight = spatialNode->convertToNodeSpace(topRight);
	cocos2d::CCRect rect;
	rect.setRect(
		bottomLeft.x,
		bottomLeft.y,
		topRight.x - bottomLeft.x,
		topRight.y - bottomLeft.y
	);
	return rect;
}