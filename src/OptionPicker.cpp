#include "OptionPicker.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"

bool OptionPicker::init(){
    if (!CCNode::init()) return false;

    this->itemTextLabel = CCLabelBMFont::create(items[0].c_str(),"bigFont.fnt");
    this->itemTextLabel->limitLabelWidth(100, 0.75f, 0.2f);

    this->currentItem = 0;
    this->maxItems = items.size()-1;
    
    auto leftArrowSprite = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
    auto rightArrowSprite = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
    leftArrowSprite->setFlipX(true);
    leftArrowSprite->setScale(0.50f);
    rightArrowSprite->setScale(0.50f);

    this->arrowLeft = CCMenuItemSpriteExtra::create(leftArrowSprite,this,menu_selector(OptionPicker::onPrevious));
    this->arrowLeft->setVisible(false);
    this->arrowLeft->setPositionX(-75.f);
    this->arrowRight = CCMenuItemSpriteExtra::create(rightArrowSprite,this,menu_selector(OptionPicker::onNext));
    this->arrowRight->setPositionX(75.f);

    auto arrowMenu = CCMenu::create();
    arrowMenu->addChild(this->arrowLeft);
    arrowMenu->addChild(this->arrowRight);
    arrowMenu->setPosition({0,0});

    this->addChild(this->itemTextLabel);
    this->addChild(arrowMenu);

    return true;
}

std::string OptionPicker::getPickedItem(){
    return this->items[this->currentItem];
}

int OptionPicker::getPickedItemID(){
    return this->currentItem;
}

void OptionPicker::onNext(CCObject*sender){
    if (this->currentItem != this->maxItems){
      this  ->currentItem++;
        this->itemTextLabel->setString(this->items[this->currentItem].c_str());
        this->itemTextLabel->limitLabelWidth(100, 0.75f, 0.2f);
        if (this->currentItem != 0){
            this->arrowLeft->setVisible(true);
        }
        if (this->currentItem == this->maxItems){
            this->arrowRight->setVisible(false);
        }
    }
}

void OptionPicker::onPrevious(CCObject*sender){
    if (this->currentItem != 0){
        this->currentItem--;
        this->itemTextLabel->setString(this->items[this->currentItem].c_str());
        this->itemTextLabel->limitLabelWidth(100, 0.75f, 0.2f);
        if (this->currentItem == 0){
            this->arrowLeft->setVisible(false);
        }
        if (this->currentItem != this->maxItems){
            this->arrowRight->setVisible(true);
        }
    }
}

OptionPicker *OptionPicker::create(std::vector<std::string> items) {
    auto ret = new OptionPicker();
    ret->items = items;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}