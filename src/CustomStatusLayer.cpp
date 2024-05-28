#include "CustomStatusLayer.hpp"
#include "GUI/CCControlExtension/CCScale9Sprite.h"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/cocos/actions/CCActionEase.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include <Geode/ui/TextInput.hpp>
#include "StatusManager.hpp"
#include "OptionPicker.hpp"
CustomStatusLayer *CustomStatusLayer::create(StatusChangeLayer*changeLayer) {
    auto ret = new CustomStatusLayer();
    ret->m_mainLayer = CCLayer::create();
    ret->changeLayer = changeLayer;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
void CustomStatusLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);
}
void CustomStatusLayer::update(float dt){
    int charsLeft = 100 - this->statusTextInput->getString().length();
    this->charactersLeft->setString(fmt::format("{}",charsLeft).c_str());
    if (charsLeft > 50){
        this->charactersLeft->setColor({0,0,0});
        this->charactersLeft->setOpacity(100);
    } 
    if (charsLeft < 50){
        this->charactersLeft->setColor({0,0,0});
        this->charactersLeft->setOpacity(255);
    }
    if (charsLeft < 25){
        this->charactersLeft->setColor({255,0,0});
        this->charactersLeft->setOpacity(255);
    }
}
bool CustomStatusLayer::init() {
    if (!CCLayer::init()) return false;
    
    auto director = CCDirector::sharedDirector();

    this->registerWithTouchDispatcher();
    this->setKeyboardEnabled(true);
    this->scheduleUpdate();
    this->setTouchEnabled(true);
    handleTouchPriority(this);

    CCLayerColor *layerColor = CCLayerColor::create({0, 0, 0});
    layerColor->setOpacity(127);
    this->addChild(layerColor);

    auto centeredNode = CCNode::create();
    centeredNode->setPosition({director->getWinSize().width/2,director->getWinSize().height/2});

    auto bg = CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize({225.f,175.f});
    centeredNode->addChild(bg);

    auto title = CCLabelBMFont::create("Set Custom Status","goldFont.fnt");
    title->setPosition({0.f,67.f});
    title->setScale(0.7f);
    centeredNode->addChild(title);

    this->descriptor = CCLabelBMFont::create("Custom Status:","bigFont.fnt");
    this->descriptor->setPosition({0.f,47.f});
    this->descriptor->setScale(0.275f);
    centeredNode->addChild(this->descriptor);

    this->statusTextInput = geode::TextInput::create(175.f,"Enter your custom status here...","chatFont.fnt");
    this->statusTextInput->setMaxCharCount(100);
    this->statusTextInput->setPositionY(22.f);
    this->statusTextInput->setFilter("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,?!#@$%^&*()\"';:- ");
    auto SM = StatusManager::get();
    if (SM->getCurrentStatus().hasCustomStatus){
        this->statusTextInput->setString(SM->getCurrentStatus().customStatus);
    }
    centeredNode->addChild(statusTextInput);

    auto cancelButtonSprite = ButtonSprite::create("Cancel");
    auto bgContentSize = cancelButtonSprite->m_BGSprite->getContentSize();
    cancelButtonSprite->m_BGSprite->initWithFile("GJ_button_04.png");
    cancelButtonSprite->m_BGSprite->setContentSize(bgContentSize);
    cancelButtonSprite->setScale(0.7f);

    auto setButtonSprite = ButtonSprite::create("SET");
    setButtonSprite->setScale(0.7f);
    setButtonSprite->setScale(0.7f);

    auto cancelButton = CCMenuItemSpriteExtra::create(cancelButtonSprite,this,menu_selector(CustomStatusLayer::onCancel));
    cancelButton->setPosition({-30.f,-35.f});

    auto setButton = CCMenuItemSpriteExtra::create(setButtonSprite,this,menu_selector(CustomStatusLayer::onSet));
    setButton->setPosition({40.f,-35.f});

    auto buttonMenu = CCMenu::create();
    buttonMenu->addChild(cancelButton);
    buttonMenu->addChild(setButton);
    buttonMenu->setPosition(0.f,-29.f);
    centeredNode->addChild(buttonMenu);

    this->charactersLeft = CCLabelBMFont::create("0","chatFont.fnt");
    this->charactersLeft->setPosition({103.f,22.f});
    this->charactersLeft->setAnchorPoint({1.f,0.5f});
    this->charactersLeft->setScale(0.725f);
    centeredNode->addChild(charactersLeft);

    this->expirationPicker = OptionPicker::create({"Never","In 30 minutes","In 1 hour","In 4 hours","Tomorrow"});
    this->expirationPicker->setPositionY(-27.f);
    this->expirationPicker->setScale(0.8f);
    centeredNode->addChild(this->expirationPicker);

    auto expirationDescriptor = CCLabelBMFont::create("Expires:","bigFont.fnt");
    expirationDescriptor->setPosition({0.f,-12.f});
    expirationDescriptor->setScale(0.275f);
    centeredNode->addChild(expirationDescriptor);

    this->m_mainLayer->addChild(centeredNode);

    //this->m_mainLayer->setAnchorPoint({0.5, 0.5});
    this->m_mainLayer->setScale(0.f);
    this->m_mainLayer->runAction(
        CCEaseElasticOut::create(CCScaleTo::create(1.f,1.f))
    );
    this->addChild(m_mainLayer);
    this->setZOrder(1000);

    return true;
}
void CustomStatusLayer::onCancel(CCObject* sender){
    this->setTouchEnabled(false);
    this->removeFromParent();
}
void CustomStatusLayer::onSet(CCObject* sender){
    auto SM = StatusManager::get();
    auto currentStatus = SM->getCurrentStatus();
    SM->setCurrentStatus(currentStatus.type,this->statusTextInput->getString().c_str());
    this->setTouchEnabled(false);
    this->removeFromParent();
    changeLayer->setTouchEnabled(false);
    changeLayer->removeFromParent();
}
void CustomStatusLayer::keyDown(cocos2d::enumKeyCodes key) {
    if (key == cocos2d::enumKeyCodes::KEY_Escape){
        this->setTouchEnabled(false);
        this->removeFromParent();
    }
    if (key == cocos2d::enumKeyCodes::KEY_Enter){
        this->onSet(this);
    }
}
bool CustomStatusLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}