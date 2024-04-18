#include "utils.hpp"
#include "ServerListener.hpp"
void status_mod::subscribeToStatusUpdates(int accID) {
    if (status_mod::isSubscribedToStatusUpdates(accID)) {
        return;
    }

    matjson::Value object;
    matjson::Value dataObject;
    dataObject.set("accountID", fmt::format("{}", accID));
    object.set("packet_type", "subscribe_to_status_updates");
    object.set("data", dataObject.dump());

    auto senderThread = std::thread([object]() { ServerListener::sendMessage(object.dump()); });
    senderThread.detach();

    listeningTo.push_back(accID);
}
void status_mod::clearStatusSubscriptions() {
    listeningTo.clear();
}
bool status_mod::isSubscribedToStatusUpdates(int accID) {
    if (std::find(listeningTo.begin(), listeningTo.end(), accID) != listeningTo.end()) {
        return true;
    }
    return false;
}

const char *status_mod::getStatusTypeString(status_mod::StatusType statusType) {
    switch (statusType) {
    case status_mod::StatusType::ONLINE:
        return "online";
        break;
    case status_mod::StatusType::IDLE:
        return "idle";
        break;
    case status_mod::StatusType::DO_NOT_DISTURB:
        return "dnd";
        break;
    case status_mod::StatusType::OFFLINE:
        return "offline";
        break;
    case status_mod::StatusType::INVISIBLE:
        return "invisible";
        break;
    case status_mod::StatusType::CUSTOM:
        return "custom";
        break;
    default:
        return "offline";
        break;
    }
}
const char *status_mod::getStatusFrameNameFromType(status_mod::StatusType statusType) {
    switch (statusType) {
    case status_mod::StatusType::ONLINE:
        return "status_online.png"_spr;
        break;
    case status_mod::StatusType::IDLE:
        return "status_idle.png"_spr;
        break;
    case status_mod::StatusType::DO_NOT_DISTURB:
        return "status_dnd.png"_spr;
        break;
    case status_mod::StatusType::OFFLINE:
        return "status_offline.png"_spr;
        break;
    case status_mod::StatusType::INVISIBLE:
        return "status_offline.png"_spr;
        break;
    case status_mod::StatusType::CUSTOM:
        return "status_custom.png"_spr;
        break;
    default:
        return "status_offline.png"_spr;
        break;
    }
}
status_mod::StatusType status_mod::getStatusTypeFromString(std::string statusType) {
    if (statusType == "online") {
        return status_mod::StatusType::ONLINE;
    } else if (statusType == "idle") {
        return status_mod::StatusType::IDLE;
    } else if (statusType == "dnd") {
        return status_mod::StatusType::DO_NOT_DISTURB;
    } else if (statusType == "offline") {
        return status_mod::StatusType::OFFLINE;
    } else if (statusType == "invisible") {
        return status_mod::StatusType::INVISIBLE;
    } else if (statusType == "custom") {
        return status_mod::StatusType::CUSTOM;
    } else {
        return status_mod::StatusType::OFFLINE;
    }
}