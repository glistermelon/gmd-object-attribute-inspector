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

	struct Fields {
		bool m_noObjectsAlertOpen = false;
	};

	void show_object_attrs() {
		if (!m_editorUI || m_fields->m_noObjectsAlertOpen) return;
		CCArrayExt<GameObject*> objects = m_editorUI->getSelectedObjects();
		if (objects.size() == 0) {
			createQuickPopup(
				"No Objects Selected!",
				"You need to select something to use the attribute inspector.",
				"OK", nullptr,
				[this](auto, bool) { m_fields->m_noObjectsAlertOpen = false; }
			)->show();
			m_fields->m_noObjectsAlertOpen = true;

		}

		auto* popup = InspectorPopup::create();
		if (!popup) return; // cleanup the other stuff too man
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

	CCArray* createObjectsFromString(gd::string& str, bool p1, bool p2) {
		geode::log::info("create {0} {1} {2}", p1, p2, str);
		return LevelEditorLayer::createObjectsFromString(str, p1, p2);
	}

};

#include <Geode/modify/EditorUI.hpp>
class $modify(EditorUI) {
	void keyDown(enumKeyCodes key) {
		if (!InspectorPopup::get()) EditorUI::keyDown(key);
	}
};

/*

	TODO FOR 1.0
	* [DONE] make the delete button on attribute listings work
	* [PARTIAL] add a button/system for adding new attributes to an object
	* warning if type decided in GameObjectWrapper constructor doesn't match the docs
	* [DONE] currently, when you commit an attribute edit, you have to reopen the popup for the new value to appear in listing. fix that
	* [DONE] fix the weird bug where every time you switch between objects you have selected it adds another attribute view node (the node that has the 3 view node type things inside it)
	* add functionality to the "enable type handling" button
		* basically just disable 90% of the features when it isnt checked
			* dont show types in listing
			* editor should always just use raw text input, and shouldnt show the type-switcher-thing
			* json values in json view should always just be the raw strings
	* refactoring, cleanup, check style guidelines
		* init functions should also call superclass init (CCNode::init too!)
		* move any sizable functions out of headers (looking at GameObjectWrapper here)
		* decide: should tiny functions (currently marked inline and left in headers) stay in headers? dunno yet, but probably not
		* most of the code in main.cpp needs to be shilled out to the cpp for the class it deals with
		* i think there are some calls to functions in geode::log that need to be removed
		* memory management
		* review usage of ObjectSelection. it seems a little unnecessary maybe?
		* file names, organization (looking at attr.hpp and attr.cpp)
	* credit smjs for the color stuff i stole from him
		* also try to add more macros there because right now for every color you've pasted a bunch of stuff
  		* maybe put this in README or something similar as well as code comments?
  	* actually deal with those standard geode things
   		* README.md, about.md, changelog.md, logo.png, mod.json, support.md
	
	TODO FOR 1.1
	* support for enums and arrays
	* attribute filtering, ordering, and hiding
		* by key, by name alphabetically, by type; + ability to reverse the list order
	
	TODO FOR 1.2
	* mini-window-viewer thing. should only work if ONE object is selected. so you can see changes as you make them
	
	TODO FOR 1.3 (maybe)
	* more features for actually documenting types, names, descriptions, etc. and contributing?

*/
