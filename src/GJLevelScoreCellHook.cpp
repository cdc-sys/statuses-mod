#include "utils.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "StatusIconNode.hpp"
#include "StatusManager.hpp"
#include <Geode/modify/GJLevelScoreCell.hpp>
class $modify(MyLevelScoreCell, GJLevelScoreCell){
    void loadFromScore(GJUserScore * p0){
        GJLevelScoreCell::loadFromScore(p0);
        if (p0->m_accountID != GJAccountManager::get()->m_accountID) {
            status_mod::subscribeToStatusUpdates(p0->m_accountID);
        }
        auto playerIcon = getChildOfType<SimplePlayer>(this->m_mainLayer, 0);

        auto statusIcon = StatusIconNode::create(p0->m_accountID);

        statusIcon->setScale(0.45f);
        if (playerIcon) {
            statusIcon->setPosition({13.f, -13.f});
            playerIcon->addChild(statusIcon);
        }
        statusIcon->setZOrder(1);
        
    }
}
;