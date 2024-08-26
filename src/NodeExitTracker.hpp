#pragma once

#include "include.hpp"

class NodeExitTracker {

	static std::map<uintptr_t, std::function<void()>> exitFunctions;

public:

	// I don't care if reinterpret_cast is bad practice; it's the best practice for this case

	static void addNode(CCNode* node, std::function<void()> fn);

	static void removeNode(CCNode* node);

};