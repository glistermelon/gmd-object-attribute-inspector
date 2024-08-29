#pragma once

#include "include.hpp"

#include "GameObjectWrapper.hpp"

class ObjectSelection {

	std::vector<std::unique_ptr<GameObjectWrapper>> m_objects;
	int m_index = 0;

public:

	inline GameObjectWrapper* getSelectedObject() {
		return m_objects[m_index].get();
	}

	inline void sortObjects() {
		std::sort(
			m_objects.begin(), m_objects.end(),
			[](auto& firstWrapped, auto& secondWrapped) {
				auto first = firstWrapped->getGameObject();
				auto second = secondWrapped->getGameObject();
				if (first->getPositionX() < second->getPositionX()) return true;
				if (first->getPositionX() > second->getPositionX()) return false;
				return first->getPositionY() < second->getPositionY();
			}
		);
	}
	
	template <class It> inline void addObjects(It begin, It end) {
		for (auto it = begin; it != end; ++it)
			m_objects.push_back(std::make_unique<GameObjectWrapper>(*it));
		this->sortObjects();
	}

	inline unsigned int getCount() {
		return m_objects.size();
	}
	inline unsigned int getCurrentIndex() {
		return m_index;
	}

	inline void selectPrev() {
		if (m_objects.size() <= 1) return;
		--m_index;
		if (m_index < 0) m_index = m_objects.size() - 1;
	}
	inline void selectNext() {
		if (m_objects.size() <= 1) return;
		++m_index;
		if (m_index >= m_objects.size()) m_index = 0;
	}

};