#pragma once
#include <string>
#ifndef _UTILS_H_
#define _UTILS_H_
#include <Geode/Geode.hpp>
#include <vector>
using namespace geode::prelude;
namespace status_mod {
    #ifndef _TABLE_H
    #define _TABLE_H

    enum StatusType {
        ONLINE = 1,
        IDLE = 2,
        DO_NOT_DISTURB = 3,
        OFFLINE = 4,
        INVISIBLE = 5,
        CUSTOM = 6,
        BANNED = 7
    };

    #endif

    struct StatusInfo {
        std::string name = "";
        std::string desc = "";
    };
    struct Status {
        StatusType type = OFFLINE;
        bool hasCustomStatus = false;
        std::string customStatus = "";
    };
    static inline std::map<StatusType, StatusInfo> settableStatuses = {
        {status_mod::StatusType::ONLINE, {"Online", ""}},
        {status_mod::StatusType::IDLE, {"Idle", ""}},
        {status_mod::StatusType::DO_NOT_DISTURB, {"Do Not Disturb", "Tells people that you do not want to be talked to."}},
        {status_mod::StatusType::OFFLINE, {"Invisible", "Makes you appear offline."}},
        {status_mod::StatusType::CUSTOM, {"Custom", ""}},
    };

    static std::vector<int> listeningTo = {};

    void clearStatusSubscriptions();
    void subscribeToStatusUpdates(int accID);
    bool isSubscribedToStatusUpdates(int accID);
    const char* getStatusTypeString(status_mod::StatusType statusType);
    const char* getStatusFrameNameFromType(status_mod::StatusType statusType);
    StatusType getStatusTypeFromString(std::string statusType);
    void setupStatusMap();
} // namespace status_mod
    #endif