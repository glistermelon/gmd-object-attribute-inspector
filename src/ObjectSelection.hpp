#pragma once

#include "include.hpp"

#include <vector>

class ObjectSelection {

	std::vector<GameObject*> m_objects;
	int m_index = 0;

public:

	inline GameObject* getSelectedObject() {
		return m_objects[m_index];
	}

	inline void sortObjects() {
		std::sort(
			m_objects.begin(), m_objects.end(),
			[](GameObject* first, GameObject* second) {
				if (first->getPositionX() < second->getPositionX()) return true;
				if (first->getPositionX() > second->getPositionX()) return false;
				return first->getPositionY() < second->getPositionY();
			}
		);
	}
	inline void addObject(GameObject* object) {
		m_objects.push_back(object);
		this->sortObjects();
	}
	
	template <class It> inline void addObjects(It begin, It end) {
		for (auto it = begin; it != end; ++it)
			m_objects.push_back(*it);
		this->sortObjects();
	}

	inline unsigned int getCount() {
		return m_objects.size();
	}
	inline unsigned int getCurrentIndex() {
		return m_index;
	}

	inline void selectPrev() {
		if (m_objects.empty()) return;
		--m_index;
		if (m_index < 0) m_index = m_objects.size() - 1;
	}
	inline void selectNext() {
		if (m_objects.empty()) return;
		++m_index;
		if (m_index >= m_objects.size()) m_index = 0;
	}

};