#include "StatusIconNode.hpp"
#include "StatusManager.hpp"
#include "utils.hpp"
bool StatusIconNode::init() {
    if (!CCNode::init())
        return false;
    lastStatusType = StatusManager::get()->getStatusForUser(this->accountID).type;
    statusIcon = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(StatusManager::get()->getStatusForUser(this->accountID).type));
    this->addChild(statusIcon);
    this->setAnchorPoint({0.5, 0.5});
    this->scheduleUpdate();
    return true;
}
void StatusIconNode::update(float dt) {
    //geode::log::info("Updated for id {}",this->accountID);
    if (!statusIcon) {
        return;
    }
    if (StatusManager::get()->getStatusForUser(this->accountID).type == lastStatusType){
        return;
    }
    if (accountID != GJAccountManager::get()->m_accountID) {
        statusIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(status_mod::getStatusFrameNameFromType(StatusManager::get()->getStatusForUser(this->accountID).type)));
    } else {
        statusIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(status_mod::getStatusFrameNameFromType(StatusManager::get()->getCurrentStatus().type)));
    }
    lastStatusType = StatusManager::get()->getStatusForUser(this->accountID).type;
}
StatusIconNode *StatusIconNode::create(int accountID) {
    auto ret = new StatusIconNode();
    ret->accountID = accountID;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}