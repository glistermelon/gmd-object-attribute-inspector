#include "include.hpp"


#include <vector>

class ObjectSelection {

	std::vector<GameObject*> m_objects;
	int m_index;

public:

	void sortObjects() {
		std::sort(
			m_objects.begin(), m_objects.end(),
			[](GameObject* first, GameObject* second) {
				if (first->getPositionX() < second->getPositionX()) return true;
				if (first->getPositionX() > second->getPositionX()) return false;
				return first->getPositionY() < second->getPositionY();
			}
		);
	}
	void addObject(GameObject* object) {
		m_objects->push_back(object);
	}
	template <It> void addObjects(It begin, It end) {
		for (auto it = begin; it != end; ++it)
			m_objects->push_back(*it);
	}

	unsigned int getCount() {
		return m_objects.size();
	}
	unsigned int getCurrentIndex() {
		return m_index;
	}

	void selectPrev() {
		if (m_objects.empty()) return;
		--m_index;
		if (m_index < 0) m_index = m_objects.size() - 1;
	}
	void selectNext() {
		if (m_objects.empty()) return;
		++m_index;
		if (m_index >= m_objects.size()) m_index = 0;
	}

};