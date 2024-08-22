#include "ObjectAttribute.hpp"

ObjectAttribute::ObjectAttribute(GameObject* object, int key, std::string unparsedValue) : m_object(object), m_key(key) {
    m_docs = AttributeDocs::getDocs(key);
    if (m_docs) {
        m_type = m_docs->getType();
        if (!setValue(unparsedValue)) {
            this->setUnknownValue(unparsedValue);
            geode::log::warn(
                "Doc type mismatch for k-v pair {0} : \"{1}\" (expected type: {2})",
                key, unparsedValue, attrtype::getTypeLabel(m_type)
            );
        }
    }
    else this->setUnknownValue(unparsedValue);
}