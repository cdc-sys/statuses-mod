#include "utils.hpp"
#include <unordered_map>
class StatusManager {
private:
    status_mod::Status currentStatus;
    std::unordered_map<int, status_mod::Status> userStatuses;

public:
    const char* domain = "gdstatus.prevter.me";
    bool isWSOpen = false;
    bool autoRunAuth = true;
    bool autoReconnect = true;
    bool authFailed = false;
    bool automaticIdle = false;
    status_mod::Status getCurrentStatus();
    status_mod::Status getStatusForUser(int accID);
    // unused (todo: remove)
    void updateUserStatus(int accID, status_mod::Status status);
    // unused (todo: remove)
    void addStatusSprite(int accID, CCSprite* sprite);

    // Only use this function when unlinking from a GD Account!
    // It removes every user status in userStatuses and resets the current status to status_mod::StatusInfo::OFFLINE!
    // Oh and it also unfollows every user status (locally)
    void reset();

    void removeStatusSprite(int accID, CCSprite* sprite);
    void setCurrentStatus(status_mod::StatusType type, std::string customStatusText = "", bool autoIdle = false);
    static StatusManager* get();
};