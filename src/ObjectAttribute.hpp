class ObjectAttribute : public TypeContainer {


	GameObject* m_object;
	int m_key;
	AttributeDocs* m_docs;

public:

	ObjectAttribute() : m_object(nullptr), m_key(-1) {}
	ObjectAttribute(GameObject* object, int key, std::string unparsedValue) : m_object(object), m_key(key) {
		m_docs = AttributeDocs::getDocs(key);
		if (m_docs) {
			m_type = m_docs->getType();
			if (!setValue(unparsedValue)) {
				setUnknownValue(unparsedValue);
				geode::log::warn(
					"Doc type mismatch for k-v pair {0} : \"{1}\" (expected type: {2})",
					key, unparsedValue, attrtype::getTypeLabel(m_type)
				);
			}
		}
		else setUnknownValue(unparsedValue);
	}

	decltype(m_key) getKey() { return m_key; }
	AttributeDocs* getDocs() { return m_docs; }

};