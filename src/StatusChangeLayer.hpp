#pragma once
#ifndef _STATUSCHANGELAYER_H_
#define _STATUSCHANGELAYER_H_
#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

class StatusChangeLayer : public CCLayer {
    bool init() override;
    void keyDown(cocos2d::enumKeyCodes key) override;
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    void registerWithTouchDispatcher() override;
    void setupMenu();
    void keyBackClicked() override;
    CCLayer *m_mainLayer;
    CCMenuItemSpriteExtra *statusChangeButton;
    std::vector<cocos2d::enumKeyCodes> correctComb = {
        cocos2d::enumKeyCodes::KEY_Up,
        cocos2d::enumKeyCodes::KEY_Up,
        cocos2d::enumKeyCodes::KEY_Down,
        cocos2d::enumKeyCodes::KEY_Down,
        cocos2d::enumKeyCodes::KEY_Left,
        cocos2d::enumKeyCodes::KEY_Right,
        cocos2d::enumKeyCodes::KEY_Left,
        cocos2d::enumKeyCodes::KEY_Right,
        cocos2d::enumKeyCodes::KEY_B,
        cocos2d::enumKeyCodes::KEY_A,
        cocos2d::enumKeyCodes::KEY_Enter
    };
    std::vector<cocos2d::enumKeyCodes> currentComb = {};
    bool hintShowed = false;
    CCMenu* statusMenu;

public:
    void onStatus(CCObject *sender);
    static StatusChangeLayer *create(CCPoint pos, CCMenuItemSpriteExtra *statusChangeButton);
};
#endif