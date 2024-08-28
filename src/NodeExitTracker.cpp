#include "NodeExitTracker.hpp"

// I don't care if reinterpret_cast is bad practice; it's the best practice for this case

void NodeExitTracker::addNode(CCNode* node, std::function<void()> fn) {
    auto ptr = reinterpret_cast<uintptr_t>(node);
    exitFunctions[ptr].push_back(fn);
}

void NodeExitTracker::removeNode(CCNode* node) {
    auto ptr = reinterpret_cast<uintptr_t>(node);
    auto it = exitFunctions.find(ptr);
    if (it != exitFunctions.end()) {
        for (auto fn : it->second) fn();
        exitFunctions.erase(it);
    }
}

decltype(NodeExitTracker::exitFunctions) NodeExitTracker::exitFunctions;

#include <Geode/modify/CCNode.hpp>
class $modify(CCNode) {

	void onExit() {
		NodeExitTracker::removeNode(this);
		CCNode::onExit();
	}

};