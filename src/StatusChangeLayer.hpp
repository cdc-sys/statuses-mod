#pragma once
#ifndef _STATUSCHANGELAYER_H_
#define _STATUSCHANGELAYER_H_
#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

class StatusChangeLayer : public CCLayer {
    bool init() override;
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    void registerWithTouchDispatcher() override;
    void setupMenu();
    void keyBackClicked() override;
    void onClearCustomStatus(CCObject* sender);
    CCLayer *m_mainLayer;
    CCMenuItemSpriteExtra *statusChangeButton;
    CCMenu* statusMenu;

public:
    void onStatus(CCObject *sender);
    static StatusChangeLayer *create(CCPoint pos, CCMenuItemSpriteExtra *statusChangeButton);
};
#endif