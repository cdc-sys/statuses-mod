#pragma once
#ifndef _STATUSICONNODE_H_
#define _STATUSICONNODE_H_
#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;

class StatusIconNode : public CCNode {
    bool init() override;
    void update(float dt) override;
    int accountID;
    CCSprite *statusIcon;
    status_mod::StatusType lastStatusType;

public:
    static StatusIconNode *create(int accountID);
};
#endif