#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;
#include "StatusManager.hpp"
#include <Geode/modify/AppDelegate.hpp>
class $modify(MyAppDelegate,AppDelegate){
    void applicationDidEnterBackground(){
        AppDelegate::applicationDidEnterBackground();
        auto status = StatusManager::get()->getCurrentStatus();
        auto newStatus = status;
        if (status.type == status_mod::StatusType::ONLINE){
            newStatus.type = status_mod::StatusType::IDLE;
            if (newStatus.hasCustomStatus){
                StatusManager::get()->setCurrentStatus(newStatus.type,newStatus.customStatus,true);
            }
            else{
                StatusManager::get()->setCurrentStatus(newStatus.type,nullptr,true);
            }
            StatusManager::get()->automaticIdle = true;
        }
    }
    void applicationWillEnterForeground(){
        AppDelegate::applicationWillEnterForeground();
        auto status = StatusManager::get()->getCurrentStatus();
        auto newStatus = status;
        if (StatusManager::get()->automaticIdle){
            newStatus.type = status_mod::StatusType::ONLINE;
            if (newStatus.hasCustomStatus){
                StatusManager::get()->setCurrentStatus(newStatus.type,newStatus.customStatus);
            }
            else{
                StatusManager::get()->setCurrentStatus(newStatus.type);
            }
            StatusManager::get()->automaticIdle = false;
        }
    }
    
};
