#include <common/PropDoc.hpp>

$on_mod(Loaded) {
	if (
		auto readDocsJsonResult = geode::utils::file::readJson(geode::Mod::get()->getResourcesDir() / "docs.json");
		readDocsJsonResult.isOk()
	) {
		PropDoc::loadDocs(readDocsJsonResult.unwrap());
	}
}