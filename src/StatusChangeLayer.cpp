#include "StatusChangeLayer.hpp"
#include "StatusManager.hpp"
#include "utils.hpp"
void StatusChangeLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -9999, true);
}
void StatusChangeLayer::keyBackClicked() {
    this->setTouchEnabled(false);
    this->removeFromParent();
}
void StatusChangeLayer::onStatus(CCObject *sender) {
    this->setTouchEnabled(false);
    CCNode *node = static_cast<CCNode *>(sender);
    if (node->getID() == "custom") {
        FLAlertLayer::create("Coming Soon...", "This <cy>feature</c> is coming in <cj>v1.1.0</c>..\nStay tuned!\n<cb>(Good job on finding the Easter Egg :3)</c>", "OK")->show();
    } else {
        auto statusType = status_mod::getStatusTypeFromString(node->getID());
        // status_mod::updateStatus({statusType,false,nullptr});
        StatusManager::get()->setCurrentStatus(statusType);
        auto newSprite = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(statusType));
        newSprite->setScale(.75f);
        // newSprite->setAnchorPoint({0.5f,0.5f});
        statusChangeButton->setNormalImage(newSprite);
        statusChangeButton->getNormalImage()->setAnchorPoint({0.5f, 0.5f});
        statusChangeButton->getNormalImage()->setPosition({8.625f, 8.625f});
        statusChangeButton->setContentSize({17.25f, 17.25f});
    }
    this->removeFromParent();
}
void StatusChangeLayer::setupMenu() {
    auto bg = CCScale9Sprite::create("GJ_square04.png");
    // bg->setColor({50,50,50});
    bg->setPositionY(this->m_mainLayer->getContentHeight());
    bg->setContentWidth(215.f);
    bg->setContentHeight(248.f);
    bg->setAnchorPoint({0, 1});
    statusMenu = CCMenu::create();
    statusMenu->setAnchorPoint({0, 1});
    // menu->setPositionY(bg->getPositionY()-5);
    statusMenu->setPositionY(-5);
    statusMenu->setPositionX(5);
    statusMenu->setZOrder(1);
    float buttonYPosOffset = 0;
    for (auto &[key, value] : status_mod::settableStatuses) {
        geode::log::info("{}", value.name);

        auto buttonNode = CCNode::create();
        CCScale9Sprite *statusBtnBg;
        if (key == StatusManager::get()->getCurrentStatus().type) {
            statusBtnBg = CCScale9Sprite::create("GJ_square03.png");
        } else {
            statusBtnBg = CCScale9Sprite::create("GJ_square02.png");
        }
        statusBtnBg->setContentWidth(205.f);
        if (!strcmp(value.desc,"")) {
            statusBtnBg->setContentHeight(35.f);
        } else {
            statusBtnBg->setContentHeight(55.f);
        }
        statusBtnBg->setAnchorPoint({0, 0});
        buttonNode->setContentSize(statusBtnBg->getContentSize());
        buttonNode->addChild(statusBtnBg);

        auto statusNameLabel = CCLabelBMFont::create(value.name, "bigFont.fnt");
        statusNameLabel->setScale(0.4f);
        statusNameLabel->setAnchorPoint({0, 0.5});
        statusNameLabel->setPosition({statusBtnBg->getContentWidth() / 7.f, ((!strcmp(value.desc,"")) ? statusBtnBg->getContentHeight() / 2.f : statusBtnBg->getContentHeight() / 1.5f)});
        buttonNode->addChild(statusNameLabel);

        auto statusSprite = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(key));
        statusSprite->setPositionY(((!strcmp(value.desc,"")) ? statusBtnBg->getContentHeight() / 2.f : statusBtnBg->getContentHeight() / 1.5f));
        statusSprite->setPositionX(16.f);
        statusSprite->setScale(0.725f);
        buttonNode->addChild(statusSprite);

        if (strcmp(value.desc,"")) {
            auto descriptionText = CCLabelBMFont::create(value.desc, "chatFont.fnt");
            descriptionText->setPosition({statusBtnBg->getContentWidth() / 7.f, statusBtnBg->getContentHeight() / 2.5f});
            descriptionText->setAnchorPoint({0, 0.5});
            // descriptionText->setScale(-((descriptionText->getContentWidth()/325)-1.f)+0.5f);
            descriptionText->setScale(0.75f);
            descriptionText->limitLabelWidth(150.f, 0.75f, 0.2f);
            buttonNode->addChild(descriptionText);
        }

        auto btn = CCMenuItemSpriteExtra::create(buttonNode, this, menu_selector(StatusChangeLayer::onStatus));
        btn->setPositionX(btn->getContentWidth() / 2);
        btn->setPositionY(this->m_mainLayer->getContentHeight() + buttonYPosOffset - (btn->getContentHeight() / 2));
        btn->setAnchorPoint({0.5, 0.5});
        btn->setID(status_mod::getStatusTypeString(key));
        btn->m_scaleMultiplier = 1.1f;
        if (key == status_mod::StatusType::CUSTOM) {
            btn->setEnabled(false);
            statusBtnBg->setColor({175, 175, 175});
            statusNameLabel->setColor({175, 175, 175});
            statusSprite->setColor({175, 175, 175});
        }
        buttonYPosOffset -= statusBtnBg->getContentHeight() + 5;
        statusMenu->addChild(btn);
    }
    m_mainLayer->addChild(statusMenu);
    m_mainLayer->addChild(bg);
}
void StatusChangeLayer::keyDown(cocos2d::enumKeyCodes key) {
    if (this->currentComb.size() == this->correctComb.size()){
        return;
    }
    bool goodKey = false;
    this->currentComb.push_back(key);
    for (int i = 0; i<this->currentComb.size();i++){
        if (this->currentComb[i] == this->correctComb[i]){
            geode::log::info("input {} correct | {} {}",i,(int)this->currentComb[i],(int)this->correctComb[i]);
            goodKey = true;
        }
        else{
            goodKey = false;
            this->currentComb.clear();
        }
        if (i == this->correctComb.size()-1){
            FMODAudioEngine::sharedEngine()->playEffect("gold02.ogg");
            Notification::create((Mod::get()->getSavedValue<bool>("konami-egg-found") ? "You did it! Again..." : "Congrats! You found the Easter Egg!"),CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
            Mod::get()->setSavedValue<bool>("konami-egg-found",true);
            auto customBtn = statusMenu->getChildByID("custom");
            if (customBtn){
                static_cast<CCMenuItemSpriteExtra*>(customBtn)->setEnabled(true);
                customBtn->setScale(1.25f);
                customBtn->runAction(CCSequence::create(
                    CCEaseExponentialOut::create(CCScaleTo::create(1,1.f)),
                    nullptr
                ));
                customBtn->runAction(CCRepeatForever::create(CCSequence::create(
                    CCEaseExponentialInOut::create(CCMoveBy::create(0.5f,ccp(0,2.f))),
                    CCEaseExponentialInOut::create(CCMoveBy::create(0.5f,ccp(0,-2.5f))),
                    CCEaseExponentialInOut::create(CCMoveBy::create(0.5f,ccp(0,.5f))),
                    nullptr
                )));
            }
            return;
        }
    }
    if (goodKey){
        FMODAudioEngine::sharedEngine()->playEffect("counter003.ogg");
    }
    else{
        if (!this->hintShowed){
            if (Mod::get()->getSavedValue<bool>("konami-egg-found")){
                Notification::create("You've seen this before...",nullptr)->show();
            }
            else{
                Notification::create("Did you hear anything...?",nullptr)->show();
            }
            this->hintShowed = true;
        }
        FMODAudioEngine::sharedEngine()->playEffect("chestClick.ogg");
    }
}
bool StatusChangeLayer::init() {
    if (!CCLayer::init()) return false;
    
    this->registerWithTouchDispatcher();
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
    handleTouchPriority(this);

    CCLayerColor *layerColor = CCLayerColor::create({0, 0, 0});
    layerColor->setOpacity(127);
    this->addChild(layerColor);

    this->m_mainLayer->setAnchorPoint({0, 1});
    this->addChild(m_mainLayer);
    this->setupMenu();
    this->setZOrder(999);

    return true;
}
StatusChangeLayer *StatusChangeLayer::create(CCPoint pos, CCMenuItemSpriteExtra *statusChangeButton) {
    auto ret = new StatusChangeLayer();
    ret->m_mainLayer = CCLayer::create();
    ret->m_mainLayer->setPosition({pos.x, pos.y - ret->m_mainLayer->getContentHeight()});
    ret->statusChangeButton = statusChangeButton;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
bool StatusChangeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}