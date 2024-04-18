#include <Geode/Geode.hpp>
#include "utils.hpp"
#include "StatusIconNode.hpp"
using namespace geode::prelude;
#include "StatusManager.hpp"
#include <Geode/modify/CommentCell.hpp>

class $modify(MyCommentCell,CommentCell){
    void loadFromComment(GJComment*p0){
        CommentCell::loadFromComment(p0);
        if (p0->m_accountID != GJAccountManager::get()->m_accountID){
            status_mod::subscribeToStatusUpdates(p0->m_accountID);
        }
        bool smallCommentsMode = this->m_height == 36;
        //geode::log::info("{}",smallCommentsMode);
        auto usernameLabel = this->m_mainLayer->getChildByID("username-label");
        auto playerIcon = getChildOfType<SimplePlayer>(this->m_mainLayer, 0);
        auto statusIcon = StatusIconNode::create(p0->m_accountID);
        if (this->m_accountComment){
            if (usernameLabel){
                statusIcon->setPosition({(usernameLabel->getPositionX()+usernameLabel->getScaledContentSize().width)+8.5f,usernameLabel->getPositionY()-2.f});
                this->m_mainLayer->addChild(statusIcon);
                statusIcon->setScale(0.55f);
            }
        }
        else{
            if (playerIcon){
            statusIcon->setPosition({playerIcon->getPositionX()+15.f*playerIcon->getScale(),playerIcon->getPositionY()-15.f*playerIcon->getScale()});
            this->m_mainLayer->addChild(statusIcon);
            statusIcon->setScale((smallCommentsMode ? 0.35f : 0.45f));
            statusIcon->setZOrder(1);
            }
        }
        
        
    }
};