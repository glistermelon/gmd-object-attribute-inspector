#include "include.hpp"

CCPoint getPositionInNode(CCNode* spatialNode, CCNode* nodeToGetPositionOf) {
	auto localPos = nodeToGetPositionOf->getParent()->convertToWorldSpace(nodeToGetPositionOf->getPosition());
	return spatialNode->convertToNodeSpace(localPos);
}

CCRect getRectInNode(CCNode* spatialNode, CCNode* rectNode) {
	CCPoint bottomLeft = rectNode->convertToWorldSpace(ccp(0.f, 0.f));
	CCPoint topRight = rectNode->convertToWorldSpace(ccp(rectNode->getContentWidth(), rectNode->getContentHeight()));
	CCRect rect;
	rect.setRect(
		bottomLeft.x,
		bottomLeft.y,
		topRight.x - bottomLeft.x,
		topRight.y - bottomLeft.y
	);
	return rect;
}