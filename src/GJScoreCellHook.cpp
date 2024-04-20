#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;
#include "StatusManager.hpp"
#include "StatusIconNode.hpp"
#include <Geode/modify/GJScoreCell.hpp>
class $modify(MyScoreCell,GJScoreCell){
    void loadFromScore(GJUserScore* p0){
        GJScoreCell::loadFromScore(p0);
        if (p0->m_accountID != GJAccountManager::get()->m_accountID){
            status_mod::subscribeToStatusUpdates(p0->m_accountID);
        }
        auto playerIcon = this->m_mainLayer->getChildByID("player-icon");

        auto statusIcon = StatusIconNode::create(p0->m_accountID);
        
        statusIcon->setScale(0.55f);
        if (playerIcon){
            statusIcon->setPosition({playerIcon->getPositionX()+15.f,playerIcon->getPositionY()-15.f});
            //playerIcon->addChild(statusIcon);
        }
        statusIcon->setZOrder(1);
        this->m_mainLayer->addChild(statusIcon);
    }
};