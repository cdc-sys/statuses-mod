#pragma once
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "Geode/ui/TextInput.hpp"
#include "OptionPicker.hpp"
#include "StatusChangeLayer.hpp"
#ifndef _CUSTOMSTATUSLAYER_H_
#define _CUSTOMSTATUSLAYER_H_
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class CustomStatusLayer : public CCLayer {
    bool init() override;
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    void registerWithTouchDispatcher() override;
    void update(float dt) override;
    void keyDown(cocos2d::enumKeyCodes key) override;
    void onCancel(CCObject*sender);
    void onSet(CCObject*sender);
    void onSwitchPage(CCObject*sender);
    CCLayer *m_mainLayer;
    bool expirationPage;
    CCSprite* customStatusPageDot;
    CCSprite* expirationPageDot;
    CCLabelBMFont* descriptor;
    OptionPicker* expirationPicker;
    CCMenuItemSpriteExtra* arrowLeft;
    CCMenuItemSpriteExtra* arrowRight;
    StatusChangeLayer* changeLayer;
    CCLabelBMFont* charactersLeft;
    geode::TextInput* statusTextInput;
public:
    static CustomStatusLayer *create(StatusChangeLayer*changeLayer);
};
#endif