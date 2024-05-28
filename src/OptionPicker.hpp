#pragma once
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#ifndef _OPTIONPICKER_H_
#define _OPTIONPICKER_H_
#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;

class OptionPicker : public CCNode {
    bool init() override;
    void onNext(CCObject*sender);
    void onPrevious(CCObject*sender);
    CCMenuItemSpriteExtra* arrowLeft;
    CCMenuItemSpriteExtra* arrowRight;
    CCLabelBMFont* itemTextLabel;
    int currentItem;
    int maxItems;
    std::vector<std::string> items;
public:
    int getPickedItemID();
    std::string getPickedItem();
    static OptionPicker *create(std::vector<std::string> items);
};
#endif