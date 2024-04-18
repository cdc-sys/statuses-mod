#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;
#include "StatusManager.hpp"
#include "StatusIconNode.hpp"
#include <Geode/modify/GJUserCell.hpp>
class $modify(MyUserCell,GJUserCell){
    void loadFromScore(GJUserScore* p0){
        GJUserCell::loadFromScore(p0);
        if (p0->m_accountID != GJAccountManager::get()->m_accountID){
            status_mod::subscribeToStatusUpdates(p0->m_accountID);
        }
        auto playerIcon = getChildOfType<SimplePlayer>(this->m_mainLayer,0);

        auto statusIcon = StatusIconNode::create(p0->m_accountID);
        
        statusIcon->setScale(0.55f);
        if (playerIcon){
            statusIcon->setPosition({15.f,-15.f});
            playerIcon->addChild(statusIcon);
        }
        statusIcon->setZOrder(1);
        
    }
};