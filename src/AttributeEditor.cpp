#include "AttributeEditor.hpp"

#include "attr.hpp"

template <class Opt, class T = std::string, auto Stringify = do_nothing<std::string>>
class EnumSelectList : public SelectList<T, Stringify> {

    using Super = SelectList<T, Stringify>;

    std::vector<Opt> m_options;

    bool init(float width, std::vector<Opt> const& options, std::vector<T> const& labels, utils::MiniFunction<void(Opt)> onChange) {
        if (options.size() != labels.size() || options.empty()) return false;
        if (!Super::init(width, labels, [this, &onChange](const T&, size_t) { onChange(this->getSelectedOption()); })) return false;
        m_options = options;
        return true;
    }

public:

    static EnumSelectList* create(float width, std::vector<Opt> options, std::vector<T> const& labels, utils::MiniFunction<void(Opt)> onChange = [](Opt) {}) {
        auto* list = new EnumSelectList();
        if (list->init(width, options, labels, onChange)) {
            list->autorelease();
            return list;
        }
        delete list;
        return nullptr;
    }

    Opt getSelectedOption() {
        return m_options[Super::m_index];
    }

};

bool AttributeEditor::setup(ObjectAttribute* objAttr) {

    auto primaryButtons = CCMenu::create();
    auto cancel = CCMenuItemSpriteExtra::create(ButtonSprite::create("Cancel"), nullptr, nullptr);
    auto commit = CCMenuItemSpriteExtra::create(ButtonSprite::create("Commit"), nullptr, nullptr);
    primaryButtons->addChild(cancel);
    primaryButtons->addChild(commit);
    primaryButtons->setLayout(RowLayout::create());
    primaryButtons->updateLayout();
    m_mainLayer->addChild(primaryButtons);

    auto input = TextInput::create(150.f, "New Value", "chatFont.fnt");
    m_mainLayer->addChild(input);

    auto typeOptions = attrtype::getTypes();
    typeOptions.pop_back(); // ATTR_TYPE_UNKNOWN
    auto typeLabels = attrtype::getTypeStrings<gd::string>();
    auto typeInput = EnumSelectList<AttributeType, gd::string>::create(100.f, typeOptions, typeLabels);
    m_mainLayer->addChild(typeInput);

    return true;

}

AttributeEditor* AttributeEditor::create(ObjectAttribute* objAttr) {
    auto* editor = new AttributeEditor();
    if (editor->init(200.f, 100.f, objAttr)) {
        editor->autorelease();
        return editor;
    }
    delete editor;
    return nullptr;
}