#include "StatusManager.hpp"
#include "ServerListener.hpp"
StatusManager *StatusManager::get() {
    static StatusManager a;
    return &a;
}
status_mod::Status StatusManager::getCurrentStatus() {
    return this->currentStatus;
}
status_mod::Status StatusManager::getStatusForUser(int accID) {
    if (accID == GJAccountManager::get()->m_accountID) {
        return this->currentStatus;
    }
    return userStatuses[accID];
}
void StatusManager::updateUserStatus(int accID, status_mod::Status status) {
    this->userStatuses[accID] = status;
}
void StatusManager::addStatusSprite(int accID, CCSprite *sprite) {
    // this->statusSprites[accID].push_back(sprite);
}
void StatusManager::removeStatusSprite(int accID, CCSprite *sprite) {
    /*std::vector<CCSprite*> sprites = statusSprites[accID];
    auto index = std::find(sprites.begin(),sprites.end(), sprite);
        if (index != sprites.end()){
            sprites.erase(index);
    }*/
}
void StatusManager::reset(){
    this->userStatuses.clear();
    this->currentStatus = status_mod::Status(); 
    this->autoReconnect = false;
    status_mod::clearStatusSubscriptions();
}
void StatusManager::setCurrentStatus(status_mod::StatusType type, std::string customStatusText, bool autoIdle) {
    status_mod::Status newStatus;
    if (!customStatusText.empty()) {
        newStatus.hasCustomStatus = true;
        newStatus.customStatus = customStatusText;
    }
    else{
        newStatus.hasCustomStatus = false;
        newStatus.customStatus = "";
    }
    newStatus.type = type;
    this->currentStatus = newStatus;
    matjson::Value object;
    matjson::Value dataObject;
    dataObject.set("status", status_mod::getStatusTypeString(type));
    if (!customStatusText.empty()) {
        dataObject.set("customStatus", customStatusText);
    }
    dataObject.set("auto_idle", autoIdle);
    object.set("packet_type", "update_user_settings");
    object.set("data", dataObject.dump());
    ServerListener::sendMessage(object.dump());
}