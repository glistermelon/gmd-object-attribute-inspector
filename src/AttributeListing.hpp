const CCSize ATTR_LISTING_SIZE = CCSize(300.f, 60.f);

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
