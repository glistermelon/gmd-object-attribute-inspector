#include "include.hpp"

#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <matjson.hpp>

#include "InspectorPopup.hpp"
#include "AttributeDocs.hpp"

$execute {
	
	keybinds::BindManager::get()->registerBindable({
		"show_info"_spr,
		"Show Object Attributes",
		"Show attribute info for selected objects in the level editor",
		{ keybinds::Keybind::create(KEY_F12, keybinds::Modifier::Alt) },
		"Object Attribute Viewer/GUI"
	});

}

$on_mod(Loaded) {
	AttributeDocs::attributeDocs = ([]() {
		decltype(AttributeDocs::attributeDocs) docs;
		std::ifstream ifs(Mod::get()->getResourcesDir().string() + "/" + "docs.json");
		std::stringstream json;
		json << ifs.rdbuf();
		ifs.close();
		std::string jsonStr = json.str();
		geode::log::info("json string '{0}'", jsonStr);
		auto data = matjson::parse(jsonStr);
		// fuck matjson let me iterate over key value pairs!
		for (int i = 0; i < 1000; ++i) {
			std::string k = std::to_string(i);
			if (!data.contains(k)) continue;
			docs[i] = data[k].as<AttributeDocs>();
		}
		return docs;
	})();
}

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(LevelEditorLayer) {

	void show_object_attrs() {
		if (!m_editorUI) return;
		CCArrayExt<GameObject*> objects = m_editorUI->getSelectedObjects();
		std::vector<GameObject*> objectVec;
		for (auto* obj : objects) objectVec.push_back(obj);
		auto objSelection = new ObjectSelection;
		objSelection->addObjects(objectVec.begin(), objectVec.end());
		auto* popup = InspectorPopup::create(objSelection, this);
		if (!popup) return;
		popup->show();
	}

	bool init(GJGameLevel* p0, bool p1) {

		if (!LevelEditorLayer::init(p0, p1)) return false;

		this->template addEventListener<keybinds::InvokeBindFilter>(
			[this](keybinds::InvokeBindEvent* event) {
				if (event->isDown()) {
					this->show_object_attrs();
				}
				return ListenerResult::Stop;
			},
			"show_info"_spr
		);

		return true;

	}

};

// TODO: dont open another popup if alt+f12 is pressed twice
// hide object info text when using object viewer
// be able to drag the view in the object viewer; add magnifying glass button to focus back on the object
// mini-window-viewer thing. should only work if ONE object is selected. so you can see changes as you make them
	// ^ make sure that the object index label updates the total selected counter when new objects are selected
// ability to toggle live updating of attributes
// sorting options for attribute list: by key, by name alphabetically, by type; + ability to reverse the list order
// dont crash if you press alt+f12 with nothing selected
// the other views (json, info, raw)
// editing attributes
// use layouts where applicable instead of manual positioning
// ability to hide certain attributes. imagine how annoying it would be to inspect relationships between attributes if theres like 50 ones you dont care about between them
// probably not in version 1.0, but enum support for 1.1 would be dope
