#include "StatusChangeLayer.hpp"
#include "StatusManager.hpp"
#include "CustomStatusLayer.hpp"
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
        auto customStatusLayer = CustomStatusLayer::create(this);
        CCDirector::sharedDirector()->getRunningScene()->addChild(customStatusLayer);
    } else {
        auto statusType = status_mod::getStatusTypeFromString(node->getID());
        // status_mod::updateStatus({statusType,false,nullptr});
        StatusManager::get()->setCurrentStatus(statusType,StatusManager::get()->getCurrentStatus().customStatus);
        auto newSprite = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(statusType));
        newSprite->setScale(.75f);
        // newSprite->setAnchorPoint({0.5f,0.5f});
        statusChangeButton->setNormalImage(newSprite);
        statusChangeButton->getNormalImage()->setAnchorPoint({0.5f, 0.5f});
        statusChangeButton->getNormalImage()->setPosition({8.625f, 8.625f});
        statusChangeButton->setContentSize({17.25f, 17.25f});
        this->removeFromParent();
    }
}
void StatusChangeLayer::setupMenu() {
    auto bg = CCScale9Sprite::create("GJ_square04.png");
    // bg->setColor({50,50,50});
    bg->setPositionY(this->m_mainLayer->getContentHeight());
    bg->setContentWidth(215.f);
    if (StatusManager::get()->getCurrentStatus().hasCustomStatus){
        bg->setContentHeight(268.f);
    }
    else {
        bg->setContentHeight(248.f);
    }
    bg->setAnchorPoint({0, 1});
    statusMenu = CCMenu::create();
    statusMenu->setAnchorPoint({0, 1});
    // menu->setPositionY(bg->getPositionY()-5);
    statusMenu->setPositionY(-5);
    statusMenu->setPositionX(5);
    statusMenu->setZOrder(1);
    float buttonYPosOffset = 0;
    for (auto &[key, value] : status_mod::settableStatuses) {
        //geode::log::info("{}", value.name);
        if (key == status_mod::StatusType::CUSTOM){
            if (StatusManager::get()->getCurrentStatus().hasCustomStatus){
                value.desc = StatusManager::get()->getCurrentStatus().customStatus;
            }
            else {
                value.desc = "";
            }
        }
        auto buttonNode = CCNode::create();
        CCScale9Sprite *statusBtnBg;
        if (key == StatusManager::get()->getCurrentStatus().type) {
            statusBtnBg = CCScale9Sprite::create("GJ_square03.png");
        } else {
            statusBtnBg = CCScale9Sprite::create("GJ_square02.png");
        }
        statusBtnBg->setContentWidth(205.f);
        if (value.desc.empty()) {
            statusBtnBg->setContentHeight(35.f);
        } else {
            statusBtnBg->setContentHeight(55.f);
        }
        statusBtnBg->setAnchorPoint({0, 0});
        buttonNode->setContentSize(statusBtnBg->getContentSize());
        buttonNode->addChild(statusBtnBg);

        auto statusNameLabel = CCLabelBMFont::create(value.name.c_str(), "bigFont.fnt");
        statusNameLabel->setScale(0.4f);
        statusNameLabel->setAnchorPoint({0, 0.5});
        statusNameLabel->setPosition({statusBtnBg->getContentWidth() / 7.f, ((value.desc.empty()) ? statusBtnBg->getContentHeight() / 2.f : statusBtnBg->getContentHeight() / 1.5f)});
        buttonNode->addChild(statusNameLabel);

        auto statusSprite = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(key));
        statusSprite->setPositionY(((value.desc.empty()) ? statusBtnBg->getContentHeight() / 2.f : statusBtnBg->getContentHeight() / 1.5f));
        statusSprite->setPositionX(16.f);
        statusSprite->setScale(0.725f);
        buttonNode->addChild(statusSprite);

        if (!value.desc.empty()) {
            auto descriptionText = CCLabelBMFont::create(value.desc.c_str(), "chatFont.fnt");
            descriptionText->setPosition({statusBtnBg->getContentWidth() / 7.f, statusBtnBg->getContentHeight() / 2.5f});
            descriptionText->setAnchorPoint({0, 0.5});
            // descriptionText->setScale(-((descriptionText->getContentWidth()/325)-1.f)+0.5f);
            descriptionText->setScale(0.75f);
            descriptionText->limitLabelWidth(150.f, 0.75f, 0.15f);
            buttonNode->addChild(descriptionText);
        }

        auto btn = CCMenuItemSpriteExtra::create(buttonNode, this, menu_selector(StatusChangeLayer::onStatus));
        btn->setPositionX(btn->getContentWidth() / 2);
        btn->setPositionY(this->m_mainLayer->getContentHeight() + buttonYPosOffset - (btn->getContentHeight() / 2));
        btn->setAnchorPoint({0.5, 0.5});
        btn->setID(status_mod::getStatusTypeString(key));
        btn->m_scaleMultiplier = 1.1f;
        /*
        if (key == status_mod::StatusType::CUSTOM) {
            btn->setEnabled(false);
            statusBtnBg->setColor({175, 175, 175});
            statusNameLabel->setColor({175, 175, 175});
            statusSprite->setColor({175, 175, 175});
        }
        */
        buttonYPosOffset -= statusBtnBg->getContentHeight() + 5;
        statusMenu->addChild(btn);
    }
    m_mainLayer->addChild(statusMenu);
    m_mainLayer->addChild(bg);
}
bool StatusChangeLayer::init() {
    if (!CCLayer::init()) return false;
    
    this->registerWithTouchDispatcher();
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
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