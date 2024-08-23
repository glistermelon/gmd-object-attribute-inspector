#pragma once

#include "include.hpp"

#include "AttributeDocs.hpp"
#include "TypeContainer.hpp"

#include <string>

class ObjectAttribute : public TypeContainer {


	GameObject* m_object;
	int m_key;
	AttributeDocs* m_docs;

public:

	ObjectAttribute() : m_object(nullptr), m_key(-1) {}
	ObjectAttribute(GameObject* object, int key, std::string unparsedValue);

	inline decltype(m_key) getKey() { return m_key; }
	inline AttributeDocs* getDocs() { return m_docs; }

	void editCallback(CCObject*);

};