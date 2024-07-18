#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "StatusIconNode.hpp"
#include "utils.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "StatusChangeLayer.hpp"
#include "StatusManager.hpp"
#include "Auth.h"
#include <Geode/modify/ProfilePage.hpp>

class $modify(MyProfilePage, ProfilePage) {
    struct Fields {
        CCSprite *statusSprite;
        bool statusSpriteAdded = false;
        int accID;
    };
    void onChangeStatus(CCObject * sender) {
        if (Mod::get()->getSavedValue<bool>("authenticated") != true || StatusManager::get()->authFailed){
            auto auth = Auth::create();
            auth->authenticationStart();
            auth->retain();
            this->onClose(sender);
            return;
        }
        auto btn = static_cast<CCNode *>(sender);
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto statusChangeLayer = StatusChangeLayer::create(btn->convertToWorldSpace({0,0}), static_cast<CCMenuItemSpriteExtra *>(sender));
        scene->addChild(statusChangeLayer);
    }
    void onClose(CCObject * sender) {

        ProfilePage::onClose(sender);
    }
    void loadPageFromUserInfo(GJUserScore * info) {
        // FLAlertLayer::create("h",fmt::format("Account ID: {}\nUser ID: {}",info->m_accountID,info->m_userID),"OK")->show();
        ProfilePage::loadPageFromUserInfo(info);
        if (this->m_fields->statusSpriteAdded) {
            return;
        }
        this->m_fields->accID = info->m_accountID;
        if (info->m_accountID != GJAccountManager::get()->m_accountID) {
            status_mod::subscribeToStatusUpdates(info->m_accountID);
        }

        if (this->m_ownProfile) {
            this->m_fields->statusSprite = CCSprite::createWithSpriteFrameName(status_mod::getStatusFrameNameFromType(StatusManager::get()->getCurrentStatus().type));
            this->m_fields->statusSprite->setScale(.75f);
            CCMenuItemSpriteExtra *changeStatusBtn = CCMenuItemSpriteExtra::create(this->m_fields->statusSprite, this, menu_selector(MyProfilePage::onChangeStatus));
            auto usernameMenu = static_cast<cocos2d::CCMenu *>(this->m_mainLayer->getChildByIDRecursive("username-menu"));
            //auto changeStatusMenu = CCMenu::create();
            //changeStatusMenu->addChild(changeStatusBtn);
            //changeStatusMenu->setPosition((usernameLabel->getPositionX() - usernameLabel->getScaledContentSize().width / 2) - (10.5f + offset), usernameLabel->getPositionY() - 2);
            //this->m_mainLayer->addChild(changeStatusMenu);
            usernameMenu->addChild(changeStatusBtn);
            usernameMenu->updateLayout();
        } else {
            auto statusIcon = StatusIconNode::create(info->m_accountID);
            statusIcon->setScale(.75f);
            //auto usernameLabel = static_cast<cocos2d::CCLabelBMFont *>(this->m_mainLayer->getChildByIDRecursive("username-label"));
            auto usernameMenu = static_cast<cocos2d::CCMenu *>(this->m_mainLayer->getChildByIDRecursive("username-menu"));
            //auto changeStatusMenu = CCMenu::create();
            //changeStatusMenu->addChild(statusIcon);
            //changeStatusMenu->setPosition((usernameLabel->getPositionX() - usernameLabel->getScaledContentSize().width / 2) - (10.5f + offset), usernameLabel->getPositionY() - 2);
            //this->m_mainLayer->addChild(changeStatusMenu);
            usernameMenu->addChild(statusIcon);
            usernameMenu->updateLayout();
        }
        this->m_fields->statusSpriteAdded = true;
    }
};