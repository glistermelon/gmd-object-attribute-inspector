#include <Geode/Geode.hpp>

#include <matjson.hpp>

#include <sstream>
#include <map>
#include <fstream>

using namespace geode::prelude;

#define ccc3BFromccc4B(ccc4B) ccColor3B({ ccc4B.r, ccc4B.g, ccc4B.b })

constexpr ccColor4B LIGHTER_BROWN_4B({ 0xBF, 0x72, 0x3E, 0xff });
constexpr ccColor3B LIGHTER_BROWN_3B = ccc3BFromccc4B(LIGHTER_BROWN_4B);
constexpr ccColor4B LIGHT_BROWN_4B({ 0xB0, 0x5C, 0x34, 0xff });
constexpr ccColor3B LIGHT_BROWN_3B = ccc3BFromccc4B(LIGHT_BROWN_4B);
constexpr ccColor4B BROWN_4B({ 0xA0, 0x54, 0x34, 0xff });
constexpr ccColor3B BROWN_3B = ccc3BFromccc4B(BROWN_4B);
constexpr ccColor4B DARK_BROWN_4B({ 0x82, 0x40, 0x21, 0xff });
constexpr ccColor3B DARK_BROWN_3B = ccc3BFromccc4B(DARK_BROWN_4B);
constexpr ccColor4F DARK_BROWN_4F({ (float)DARK_BROWN_4B.r / 255.f, (float)DARK_BROWN_4B.g / 255.f, (float)DARK_BROWN_4B.b / 255.f, (float)DARK_BROWN_4B.a / 255.f });
constexpr float PADDING_HORIZONTAL = 50.f;
constexpr float PADDING_VERTICAL = 50.f;

enum {
	INFO_VIEW,
	JSON_VIEW,
	RAW_VIEW,
	NORMAL_VIEW // "Detailed"
};

class ObjectView : public geode::Border {

	CCClippingNode* mask = nullptr;

	bool init(float size) {
		
		if (!Border::init(ccColor4B{ 0, 0, 0, 0 }, CCSize(size, size))) return false;

		this->setAnchorPoint(ccp(0.5f, 0.5f));
		this->ignoreAnchorPointForPosition(false);

		return true;

	}

public:

	static ObjectView* create(float size) {
		auto* obj = new ObjectView();
		if (obj->init(size)) {
			obj->autorelease();
			return obj;
		}
		delete obj;
		return nullptr;
	}

	void setObject(GameObject* object, LevelEditorLayer* editorLayer) {

		CCLayer* objLayer = static_cast<GJBaseGameLayer*>(editorLayer)->m_objectLayer;
		CCNode* objLayerParent = objLayer->getParent();
		CCPoint objPos = objLayerParent->convertToNodeSpace(objLayer->convertToWorldSpace(object->getPosition()));
		CCSize editLayerSize = editorLayer->getContentSize();
		CCRect rect = this->boundingBox();
		CCPoint moveTo = objLayerParent->convertToNodeSpace(this->convertToWorldSpace(
			ccp(this->getContentWidth() / 2, this->getContentHeight() / 2)
		));
		objLayer->setPosition(objLayer->getPosition() - objPos + moveTo);

	}

};

enum AttributeType {
	ATTR_TYPE_INT,
	ATTR_TYPE_FLOAT,
	ATTR_TYPE_BOOL,
	ATTR_TYPE_ARRAY,
	ATTR_TYPE_COLOR,
	ATTR_TYPE_STRING,
	ATTR_TYPE_UNKNOWN
};

namespace attrtype {

	gd::string getTypeLabel(AttributeType type) {
		switch (type) {
			case ATTR_TYPE_INT:
				return "integer";
			case ATTR_TYPE_FLOAT:
				return "float";
			case ATTR_TYPE_BOOL:
				return "boolean";
			case ATTR_TYPE_ARRAY:
				return "array";
			case ATTR_TYPE_COLOR:
				return "color";
			case ATTR_TYPE_STRING:
				return "string";
			default:
				return "unknown";
		}
	}

	gd::string getTypeLabelShort(AttributeType type) {
		switch (type) {
			case ATTR_TYPE_INT:
				return "int";
			case ATTR_TYPE_FLOAT:
				return "float";
			case ATTR_TYPE_BOOL:
				return "bool";
			case ATTR_TYPE_ARRAY:
				return "array";
			case ATTR_TYPE_COLOR:
				return "color";
			case ATTR_TYPE_STRING:
				return "string";
			default:
				return "unknown";
		}
	}
	
	ccColor3B getTypeLabelColor(AttributeType type) {
		switch (type) {
			case ATTR_TYPE_INT:
				return { 204, 51, 255 };  // purple
			case ATTR_TYPE_FLOAT:
				return { 255, 51, 0 };    // red
			case ATTR_TYPE_BOOL:
				return { 0, 0, 255 };     // blue
			case ATTR_TYPE_ARRAY:
				return { 255, 153, 51 };  // orange
			case ATTR_TYPE_COLOR:
				return { 102, 255, 255 }; // bright blue
			case ATTR_TYPE_STRING:
				return { 51, 204, 51 };   // green
			default:
				return { 200, 200, 200 }; // grey
		}
	}

	AttributeType fromJsonString(std::string s) {
		if (s == "int") return ATTR_TYPE_INT;
		if (s == "float") return ATTR_TYPE_FLOAT;
		if (s == "bool") return ATTR_TYPE_BOOL;
		if (s == "array") return ATTR_TYPE_ARRAY;
		if (s == "color") return ATTR_TYPE_COLOR;
		if (s == "string") return ATTR_TYPE_STRING;
		geode::log::warn("fromJsonString encountered unexpected type string: {0}", s);
		return ATTR_TYPE_UNKNOWN;
	}

}

class AttributeDocs {

	friend struct matjson::Serialize<AttributeDocs>;

	std::string m_name = "Object ID"; // todo: get rid of this
	std::string m_description = "this is a description"; // todo: get rid of this
	AttributeType m_type = ATTR_TYPE_INT; // todo: change to ATTR_TYPE_UNKNOWN

public:

	static std::map<int, AttributeDocs> attributeDocs;

	static AttributeDocs* getDocs(int attr) {
		auto it = attributeDocs.find(attr);
		return it == attributeDocs.end() ? nullptr : &it->second;
	}

	std::string getName() { return m_name; }
	std::string getDescription() { return m_description; }
	AttributeType getType() { return m_type; }

};

template <> struct matjson::Serialize<AttributeDocs> {
	static AttributeDocs from_json(const matjson::Value& val) {
		auto doc = AttributeDocs();
		doc.m_name = val["name"].as_string();
		doc.m_description = val["desc"].is_null() ? "" : val["desc"].as_string();
		doc.m_type = val["type"].is_null() ? ATTR_TYPE_UNKNOWN : attrtype::fromJsonString(val["type"].as_string());
		return doc;
	}
};

decltype(AttributeDocs::attributeDocs) AttributeDocs::attributeDocs;

struct ColorHSV {

	int h;
	int s;
	int v;
	bool s_on;
	bool v_on;
	
	std::string hexCode() {
		std::stringstream ss;
		ss << "todo"; // https://docs.geode-sdk.org/classes/geode/ColorPickPopup maybe
		return ss.str();
	}

};

class TypeContainer {

	union {
		int m_intVal;
		float m_floatVal;
		bool m_boolVal;
		ColorHSV m_colorVal;
	};
	std::string m_strVal; // also used for unknown type

protected:

	AttributeType m_type;

public:

	TypeContainer() {
		m_type = ATTR_TYPE_UNKNOWN;
	}
	TypeContainer(int val) {
		m_intVal = val;
		m_type = ATTR_TYPE_INT;
	}
	TypeContainer(float val) {
		m_floatVal = val;
		m_type = ATTR_TYPE_FLOAT;
	}
	TypeContainer(bool val) {
		m_boolVal = val;
		m_type = ATTR_TYPE_BOOL;
	}
	TypeContainer(ColorHSV val) {
		m_colorVal = val;
		m_type = ATTR_TYPE_COLOR;
	}
	TypeContainer(std::string val) {
		m_strVal = val;
		m_type = ATTR_TYPE_STRING;
	}

	decltype(m_type) getType() { return m_type; }

	std::optional<int> getIntValue() { return m_type == ATTR_TYPE_INT ? m_intVal : std::optional<int>(); }
	std::optional<float> getFloatValue() { return m_type == ATTR_TYPE_FLOAT ? m_floatVal : std::optional<float>(); }
	std::optional<bool> getBoolValue() { return m_type == ATTR_TYPE_BOOL ? m_boolVal : std::optional<bool>(); }
	std::optional<ColorHSV> getColorValue() { return m_type == ATTR_TYPE_COLOR ? m_colorVal : std::optional<ColorHSV>(); }
	std::optional<std::string> getStringValue() { return m_type == ATTR_TYPE_STRING ? m_strVal : std::optional<std::string>(); }

	bool setValue(std::string unparsed) {

		auto stripBegin = unparsed.begin();
		auto stripEnd = unparsed.rbegin();
		while (stripBegin != unparsed.end() && std::isspace(*stripBegin)) ++stripBegin;
		while (stripEnd != unparsed.rend() && std::isspace(*stripEnd)) ++stripEnd;
		unparsed = std::string(stripBegin, stripEnd.base());
		if (unparsed.empty()) return false;
		for (auto it = unparsed.begin(); it != unparsed.end(); ++it) {
			if (std::isupper(*it)) *it = std::tolower(*it);
		}

		if (m_type == ATTR_TYPE_INT || m_type == ATTR_TYPE_FLOAT) {
			try {
				if (m_type == ATTR_TYPE_INT) m_intVal = std::stoi(unparsed);
				else m_floatVal = std::stof(unparsed);
				return true;
			}
			catch (std::invalid_argument) {
				return false;
			}
			catch (std::out_of_range) {
				return false;
			}
		}
		else if (m_type == ATTR_TYPE_BOOL) {
			if (unparsed == "true" || unparsed == "1") m_boolVal = true;
			else if (unparsed == "0" || unparsed == "-1") m_boolVal = false;
			else return false;
			return true;
		}
		else if (m_type == ATTR_TYPE_COLOR) return false; // use setColor
		else {
			m_strVal = unparsed;
			return true;
		}

	}

	std::string getValueLabel() {
		switch (m_type) {
			case ATTR_TYPE_INT:
				return std::to_string(m_intVal);
			case ATTR_TYPE_FLOAT:
				return std::to_string(m_floatVal);
			case ATTR_TYPE_BOOL:
				return m_boolVal ? "true" : "false";
			case ATTR_TYPE_COLOR:
				return m_colorVal.hexCode();
			default:
				return m_strVal;
		}
	}

	void setIntValue(int v) { m_intVal = v; m_type = ATTR_TYPE_INT; }
	void setFloatValue(float v) { m_floatVal = v; m_type = ATTR_TYPE_FLOAT; }
	void setBoolValue(bool v) { m_boolVal = v; m_type = ATTR_TYPE_BOOL; }
	void setColorValue(ColorHSV v) { m_colorVal = v; m_type = ATTR_TYPE_COLOR; }
	void setStringValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_STRING; }
	void setUnknownValue(std::string v) { m_strVal = v; m_type = ATTR_TYPE_UNKNOWN; }

	bool setColor(ColorHSV newColor) {
		if (m_type != ATTR_TYPE_COLOR) return false;
		m_colorVal = newColor;
		return true;
	}

};

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

const CCSize ATTR_LISTING_SIZE = CCSize({ 300.f, 60.f });

class AttributeListing : public GenericListCell {

	ObjectAttribute* m_attr;
	
	std::vector<CCScale9Sprite*> textBoxes;

	AttributeListing() : GenericListCell("", ATTR_LISTING_SIZE) {}

public:

	void updateColors(bool evenIndex) {
		for (auto sprite : textBoxes) sprite->setColor(evenIndex ? LIGHT_BROWN_3B : LIGHTER_BROWN_3B);
	}

private:

	bool init(ObjectAttribute* objAttr) {

		m_attr = objAttr;

		// GenericListCell::setBorderColor(DARK_BROWN_4B);

		m_mainLayer->setContentSize(ATTR_LISTING_SIZE);

		auto menu = CCMenu::create();
		menu->setAnchorPoint(ccp(0.f, 0.f));
		menu->setPosition(ccp(0.f, 0.f));
		menu->setContentSize(m_mainLayer->getContentSize());
		m_mainLayer->addChild(menu);
		
		std::string keyStr = std::to_string(m_attr->getKey());
		std::string valueStr = m_attr->getValueLabel();

		auto keyLabel = CCLabelBMFont::create(keyStr.c_str(), "chatFont.fnt", 30.f);
		auto valueLabel = CCLabelBMFont::create(valueStr.c_str(), "chatFont.fnt", 100.f);

		auto keyTextLabel = CCLabelBMFont::create("Key:", "chatFont.fnt");
		keyTextLabel->setScale(0.7f);
		auto valueTextLabel = CCLabelBMFont::create("Value:", "chatFont.fnt");
		valueTextLabel->setScale(0.7f);

		auto keyBox = CCScale9Sprite::create("square02b_001.png");
		keyBox->setContentSize(CCSize(keyLabel->getContentWidth() / 0.6f, 35.f));
		keyBox->setScale(0.6f);
		auto valueBox = CCScale9Sprite::create("square02b_001.png");
		valueBox->setContentSize(CCSize(valueLabel->getContentWidth() / 0.6f, 35.f));
		valueBox->setScale(0.6f);

		textBoxes.push_back(keyBox);
		textBoxes.push_back(valueBox);

		keyBox->setColor(m_secondaryColor);
		valueBox->setColor(m_secondaryColor);

		keyBox->setContentWidth(57.f);
		valueBox->setContentWidth(325.f);

		keyLabel->setScale(1 / 0.6f);
		valueLabel->setScale(1 / 0.6f);
		keyLabel->setPosition(ccp(keyBox->getContentWidth() / 2, keyBox->getContentHeight() / 2));
		valueLabel->setPosition(ccp(valueBox->getContentWidth() / 2, valueBox->getContentHeight() / 2));

		keyBox->addChild(keyLabel);
		valueBox->addChild(valueLabel);

		keyTextLabel->setPosition(ccp(15.f, 16.f));
		keyBox->setPosition(ccp(46.f, 16.f));
		valueTextLabel->setPosition(ccp(84.f, 16.f));
		valueBox->setPosition(ccp(198.f, 16.f));

		m_mainLayer->addChild(keyTextLabel);
		m_mainLayer->addChild(keyBox);
		m_mainLayer->addChild(valueTextLabel);
		m_mainLayer->addChild(valueBox);

		auto buttons = CCMenu::create();
		auto trashSprite = EditorButtonSprite::createWithSprite("trash.png"_spr, 0.8f, EditorBaseColor::DarkGray);
		auto editSprite = EditorButtonSprite::createWithSprite("pencil.png"_spr, 0.8f, EditorBaseColor::BrightGreen);
		trashSprite->setScale(0.7f);
		editSprite->setScale(0.7f);
		auto trash = CCMenuItemSpriteExtra::create(trashSprite, this, nullptr);
		auto edit = CCMenuItemSpriteExtra::create(editSprite, this, nullptr);
		buttons->addChild(trash);
		buttons->addChild(edit);
		buttons->setContentWidth(250.f);
		buttons->setPosition(ccp(46.f, 31.f));
		buttons->setAnchorPoint(ccp(0.f, 0.f));
		auto layout = RowLayout::create();
		layout->setAxisReverse(true);
		layout->setAxisAlignment(AxisAlignment::End);
		layout->setAutoScale(false);
		buttons->setLayout(layout);
		buttons->updateLayout();
		menu->addChild(buttons);

		auto docs = m_attr->getDocs();

		std::string nameStr = docs ? docs->getName() : "Unknown Attribute";
		auto name = CCLabelBMFont::create(nameStr.c_str(), "bigFont.fnt");
		name->setAnchorPoint(ccp(0.f, 0.f));
		name->setPosition(ccp(5.f, 31.f));
		name->limitLabelWidth(175.f, 0.7f, 0.1f);
		m_mainLayer->addChild(name);

		auto type = m_attr->getType();

		std::stringstream infoStream;
		if (docs) {
			infoStream << "Name: " << nameStr << "\n";
			infoStream << "Type: " << attrtype::getTypeLabel(type) << "\n";
			infoStream << m_attr->getDocs()->getDescription();
		}
		std::string infoStr = infoStream.str();
		auto info = InfoAlertButton::create("Attribute Documentation", infoStr.c_str(), 1.f);
		info->setAnchorPoint(ccp(0.f, 0.f));
		info->setPosition(ccp(0.f, 0.f));
		auto infoContainer = CCMenu::create();
		infoContainer->setAnchorPoint(ccp(0.f, 0.f));
		infoContainer->setPosition(ccp(name->getPositionX() + name->getScaledContentWidth() + 2.f, name->getPositionY() + 9.f));
		infoContainer->setScale(0.5f);
		infoContainer->addChild(info);
		infoContainer->setContentSize(info->getContentSize());
		menu->addChild(infoContainer);

		if (!docs) infoContainer->setVisible(false);

		gd::string typeStr = attrtype::getTypeLabelShort(type);
		if (typeStr == "unknown") typeStr = "";
		auto typeLabel = CCLabelBMFont::create(typeStr.c_str(), "bigFont.fnt");
		typeLabel->setColor(attrtype::getTypeLabelColor(type));
		typeLabel->setAnchorPoint(ccp(0.f, 0.f));
		typeLabel->setPosition(ccp(name->getPositionX() + name->getScaledContentWidth() + 2.f, name->getPositionY()));
		typeLabel->setScale(0.325f);
		m_mainLayer->addChild(typeLabel);

		return true;

	}

public:

	static AttributeListing* create(ObjectAttribute* objAttr) {
		auto* listing = new AttributeListing();
		if (listing->init(objAttr)) {
			listing->autorelease();
			return listing;
		}
		delete listing;
		return nullptr;
	}

};

class InspectorPopup : public geode::Popup<std::vector<GameObject*>&&, LevelEditorLayer*> {

	std::vector<CCMenuItemToggler*> viewButtons;
	CCMenuItemToggler* typeButton;
	CCMenuItemToggler* docsButton;

protected:

	bool setup(std::vector<GameObject*>&& gameObjects, LevelEditorLayer* editorLayer) override {

		auto* newParent = CCClippingNode::create(CCNode::create());
		newParent->setContentSize(CCScene::get()->getContentSize());
		for (auto* child : CCArrayExt<CCNode*>(this->getChildren())) {
			newParent->addChild(child);
			this->removeChild(child);
		}
		this->addChild(newParent);

		//auto* list = ObjectView::create();
		//if (!list) return false;
		//this->addChild(list);
		//list->setPosition(CCPoint(PADDING_HORIZONTAL, PADDING_VERTICAL));

		//auto* dataViewOpt = CCMenuItemToggle::create();

		CCPoint padding(PADDING_HORIZONTAL, PADDING_VERTICAL);

		auto* menu = CCMenu::create();
		if (!menu) return false;
		menu->setAnchorPoint(ccp(0.f, 0.f));
		menu->setPosition(padding);

		constexpr std::string_view buttonLabels[] = { "Basic Info", "Raw", "JSON", "Detailed" };
		constexpr int buttonTags[] = { INFO_VIEW, RAW_VIEW, JSON_VIEW, NORMAL_VIEW };

		for (int i = 0; i < 4; ++i) {

			float yPos = 20.f * i;

			auto* button = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this](CCMenuItemToggler* from) {
				for (auto* other : this->viewButtons) {
					if (other != from && other->isOn()) {
						other->toggle(false);
						other->setEnabled(true);
					}
					from->setEnabled(false);
					from->toggle(true);
				}
			});
			if (!button) return false;
			button->setTag(buttonTags[i]);
			button->setPosition(ccp(0.f, yPos));
			menu->addChild(button);
			viewButtons.push_back(button);
			if (i == 0) {
				button->setEnabled(false);
				button->toggle(true);
			}

			auto* label = CCLabelBMFont::create(buttonLabels[i].data(), "bigFont.fnt");
			if (!label) return false;
			label->setScale(0.5f);
			label->setAnchorPoint(ccp(0.f, 0.5f));
			label->setPosition(ccp(12.f, yPos));
			menu->addChild(label);

			float labelEnd = label->getPositionX() + label->getScaledContentWidth();

		}
		this->addChild(menu);

		typeButton = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [](CCMenuItemToggler*) {});
		if (!typeButton) return false;
		typeButton->setPosition(ccp(0.f, viewButtons.back()->getPositionY() + viewButtons.back()->getScaledContentHeight() / 2.f + 30.f));
		menu->addChild(typeButton);

		auto* typeLabel = CCLabelBMFont::create("Type Handling", "bigFont.fnt");
		if (!typeLabel) return false;
		typeLabel->setAnchorPoint(ccp(0.f, 0.5f));
		typeLabel->setPosition(ccp(12.f, typeButton->getPositionY()));
		typeLabel->setScale(0.5f);
		menu->addChild(typeLabel);

		auto* segment = CCDrawNode::create();
		if (!segment) return false;
		segment->setAnchorPoint(ccp(0.f, 0.f));
		segment->setPosition(ccp(
			menu->getPositionX() - 10.f,
			padding.y + typeButton->getPositionY() - 20.f
		));
		segment->setContentWidth(typeLabel->boundingBox().getMaxX() + 20.f);
		segment->drawSegment(ccp(0.f, 0.f), ccp(segment->getContentWidth(), 0.f), 1.f, DARK_BROWN_4F);
		segment->setZOrder(1);
		this->addChild(segment);

		float objViewSize = m_mainLayer->getContentHeight() - (typeButton->getPositionY() + typeButton->getScaledContentHeight()) - 50.f;
		auto* objView = ObjectView::create(objViewSize);
		if (!objView) return false;
		objView->setPosition(ccp(
			segment->getPositionX() + segment->getContentWidth() / 2.f,
			padding.y + typeLabel->boundingBox().getMaxY() + (m_mainLayer->getContentHeight() - typeLabel->boundingBox().getMaxY()) / 2 - 15.f
		));
		this->addChild(objView);
		objView->setObject(gameObjects[0], editorLayer);

		auto* mask = CCDrawNode::create();
		mask->ignoreAnchorPointForPosition(true);
		mask->setContentSize(newParent->getContentSize());
		auto rect = objView->boundingBox();
		mask->drawRect(
			ccp(rect.getMinX() + 1.f, rect.getMinY() + 1.f), ccp(rect.getMaxX() - 1.f, rect.getMaxY() - 1.f),
			ccColor4F{ 0.f, 0.f, 0.f, 1.f }, 0.f, ccColor4F{ 0.f, 0.f, 0.f, 0.f }
		);
		newParent->setStencil(mask);
		newParent->setInverted(true);
		
		this->setOpacity(0);
		
		auto* background = CCLayerColor::create(ccColor4B { 0, 0, 0, 105 });
		background->setZOrder(-1);
		newParent->addChild(background);

		CCArray* listItems = CCArray::create();
		for (auto object : gameObjects) {
			gd::string saveStr = object->getSaveString(static_cast<GJBaseGameLayer*>(editorLayer));
			std::stringstream saveStream(saveStr);
			std::string k, v; // todo: breaking geode style guidelines?
			bool key = true;
			while (std::getline(saveStream, key ? k : v, ',')) {
				if (!key) {
					auto objAttr = new ObjectAttribute(object, std::stoi(k), v);
					listItems->addObject(AttributeListing::create(objAttr));
				}
				key = !key;
			}
			break;
		}

		bool evenIndex = true;
		for (auto listItem : CCArrayExt<AttributeListing*>(listItems)) {
			listItem->updateColors(evenIndex);
			evenIndex = !evenIndex;
		}
		auto list = ListView::create(listItems, ATTR_LISTING_SIZE.height, ATTR_LISTING_SIZE.width, 235.f);
		list->setPosition(ccp(215.f, 40.f));
		list->setZOrder(1);
		list->setPrimaryCellColor(LIGHTER_BROWN_3B);
		list->setSecondaryCellColor(LIGHT_BROWN_3B);
		this->addChild(list);

		auto listBorder = Border::create(nullptr, { 0, 0, 0, 0 }, list->getContentSize());
		listBorder->setPosition(list->getPosition());
		listBorder->setAnchorPoint(list->getAnchorPoint());
		listBorder->setZOrder(list->getZOrder() + 1);
		this->addChild(listBorder);

		auto listBackground = CCLayerColor::create(LIGHTER_BROWN_4B);
		listBackground->setPosition(list->getPosition());
		listBackground->setAnchorPoint(list->getAnchorPoint());
		listBackground->setZOrder(list->getZOrder() - 1);
		listBackground->setContentSize(list->getContentSize());
		this->addChild(listBackground);

		return true;
	}

public:

	static InspectorPopup* create(std::vector<GameObject*>&& objects, LevelEditorLayer* editorLayer) {
		auto* popup = new InspectorPopup();
		CCSize size = CCDirector::sharedDirector()->getWinSize() - CCSize(PADDING_HORIZONTAL, PADDING_VERTICAL);
		if (popup->initAnchored(size.width, size.height, std::move(objects), editorLayer)) {
			popup->autorelease();
			return popup;
		}
		delete popup;
		return nullptr;
	}

};

#include <geode.custom-keybinds/include/Keybinds.hpp>

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
		auto* popup = InspectorPopup::create(std::move(objectVec), this);
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
// multiple objects; switch between with arrow keys by object viewer
// hide object info text when using object viewer
// be able to drag the view in the object viewer; add magnifying glass button to focus back on the object
// mini-window-viewer thing. should only work if ONE object is selected. so you can see changes as you make them
// ability to toggle live updating of attributes
// sorting options for attribute list: by key, by name alphabetically, by type; + ability to reverse the list order
// dont crash if you press alt+f12 with nothing selected
// the other views (json, info, raw)
// editing attributes
// use layouts where applicable instead of manual positioning
// ability to hide certain attributes. imagine how annoying it would be to inspect relationships between attributes if theres like 50 ones you dont care about between them
