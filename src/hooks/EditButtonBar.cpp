#include "EditButtonBar.hpp"

#include <Geode/binding/GameManager.hpp>

void EditButtonBarExtension::addButton(cocos2d::CCNode* button) {
    m_buttonArray->addObject(button);
    auto gameManager = GameManager::get();
    this->reloadItems(
        gameManager->getIntGameVariable("0049"),
        gameManager->getIntGameVariable("0050")
    );
}