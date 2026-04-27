#include "PropDoc.hpp"

std::optional<PropDoc> PropDoc::getDoc(PropKey key) {
    auto it = s_propDocs.find(key);
    if (it == s_propDocs.end()) return std::nullopt;
    return it->second;
}

void PropDoc::loadDocs(matjson::Value docsJson) {
    for (auto [key, val] : docsJson) {
        if (
            auto deserializeResult = val.as<PropDoc>();
            deserializeResult.isOk()
        ) {
            s_propDocs[PropKey::createFromPropString(key)] = deserializeResult.unwrap();
        }
    }
}