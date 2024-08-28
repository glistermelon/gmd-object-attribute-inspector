#pragma once

#include "include.hpp"

class DynamicListItem : public GenericListCell {

public:

    virtual void updateListItem(int index) = 0;

    virtual bool listBefore(DynamicListItem* other) = 0;

    inline DynamicListItem(CCSize size) : GenericListCell("", size) {}

};

class DynamicListView : public CCNode {

    ListView* m_listView = nullptr;
    std::vector<DynamicListItem*> m_items;

    std::array<float, 3> m_initArgs;

    bool init(CCArray* items, float itemHeight, float width, float height) {
        if (!CCNode::init()) return false;
        for (auto item : CCArrayExt<DynamicListItem*>(items))
            m_items.push_back(item);
        m_initArgs = { itemHeight, width, height };
        if (!this->updateItems()) return false;
        return true;
    }

public:

    static DynamicListView* create(CCArray* items, float itemHeight, float width, float height) {
        auto list = new DynamicListView();
        if (list->init(items, itemHeight, width, height)) {
            list->autorelease();
            return list;
        }
        delete list;
        return nullptr;
    }

    ListView* getListView() { return m_listView; }

    bool updateItems() {
        if (m_listView) m_listView->getParent()->removeChild(m_listView);
        std::sort(m_items.begin(), m_items.end(), [](auto a, auto b) { return a->listBefore(b); });
        CCArrayExt<DynamicListItem*> items;
        for (auto item : m_items) items.push_back(item);
        m_listView = ListView::create(items.inner(), m_initArgs[0], m_initArgs[1], m_initArgs[2]);
        if (!m_listView) return false;
        for (int i = 0; i < m_items.size(); ++i) m_items[i]->updateListItem(i);
        return true;
    }

    void addItem(DynamicListItem* item) {
        m_items.push_back(item);
        this->updateItems();
    }

    void removeItem(DynamicListItem* item) {
        auto it = std::find(m_items.begin(), m_items.end(), item);
        if (it == m_items.end()) return;
        m_items.erase(it);
        this->updateItems();
    }

};