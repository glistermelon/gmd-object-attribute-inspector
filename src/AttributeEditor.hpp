#pragma once

#include "include.hpp"

#include "ObjectAttribute.hpp"

class AttributeEditor : public geode::Popup<ObjectAttribute*> {

    bool setup(ObjectAttribute* objAttr) override;

public:

    static AttributeEditor* create(ObjectAttribute* objAttr);

};