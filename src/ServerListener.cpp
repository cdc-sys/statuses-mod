#include "easywsclient.hpp"
#include <chrono>
#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#endif
#include "ServerListener.hpp"
#include "StatusManager.hpp"
#include <Geode/Geode.hpp>
#include <assert.h>
#include <memory>
#include <string>
#include <thread>



using namespace cocos2d;

using namespace easywsclient;

std::unique_ptr<WebSocket> ws(nullptr);

void ServerListener::connectAsync() {
    std::thread t = std::thread(connect);
    t.detach();
}

void ServerListener::connect() {

    open();

    using easywsclient::WebSocket;

#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        return;
    }
#endif

    if (ws) {
        StatusManager::get()->isWSOpen = true;
        matjson::Value object;
        matjson::Value dataObject;
        dataObject.set("token", geode::Mod::get()->getSavedValue<std::string>("token"));
        object.set("packet_type", "authorization");
        object.set("data", dataObject.dump());

        ws->send(object.dump());

        while (ws->getReadyState() != WebSocket::CLOSED) {
            WebSocket::pointer wsp = &*ws;
            ws->poll();
            ws->dispatch([wsp](const std::string &message) {
                //onMessage(message);
                onMessageThreaded(message);
                // geode::log::info("{}", message);
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // closed
    StatusManager::get()->isWSOpen = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    geode::Loader::get()->queueInMainThread([]() {
        if (StatusManager::get()->autoReconnect){
            ServerListener::connectAsync();
            status_mod::clearStatusSubscriptions();
        } });
    ws.release();

#ifdef _WIN32
    WSACleanup();
#endif
}

void ServerListener::onMessageThreaded(std::string message) {
    geode::log::info("{}", message);
    matjson::Value object = matjson::parse(message);
    if (!object.contains("packet_type") || !object.contains("data")) {
        return;
    }

    std::string packet_type = object["packet_type"].as_string();
    std::string packet_data = object["data"].as_string();
    if (packet_type == "auth_response") {
        matjson::Value data = matjson::parse(packet_data);
        if (!data.contains("success")) {
            return;
        }
        if (data["success"].as_bool()) {
            matjson::Value object;
            matjson::Value dataObject;
            object.set("packet_type", "get_user_settings");
            object.set("data", dataObject.dump());
            ws->send(object.dump());
        } else {
            geode::log::info("authorization failed");
        }
    }
    if (packet_type == "ping"){
        matjson::Value object;
        matjson::Value dataObject;
        object.set("packet_type", "pong");
        object.set("data", dataObject.dump());
        ws->send(object.dump());
    }
    if (packet_type == "auth_failed"){
            Mod::get()->setSavedValue<bool>("authenticated",false);
            StatusManager::get()->isWSOpen = false;
            StatusManager::get()->autoReconnect = false;
            StatusManager::get()->autoRunAuth = true;
            Loader::get()->queueInMainThread([](){
                auto flalert = createQuickPopup("Uh Oh!","You've been <cr>deauthorized</c> from <cg>Statuses</c>!\nWould you like to open your <co>profile</c> to <cy>authenticate</c> again?\n<cb>(To authenticate you need to press the Status Icon on your profile)</c>","Later","OPEN",
                [](auto,bool btn2){
                    if (btn2){
                        ProfilePage::create(GJAccountManager::get()->m_accountID,true)->show();
                    }
            });
            });
    }
    if (packet_type == "user_settings_response"){
            matjson::Value data = matjson::parse(packet_data);
            status_mod::Status status;
            if (!data.contains("status")) {
                return;
            }
            status.type = status_mod::getStatusTypeFromString(data["status"].as_string().c_str());
            status.hasCustomStatus = data.contains("customStatus");
            if (data.contains("customStatus")){
                status.customStatus = data["customStatus"].as_string().c_str();
                StatusManager::get()->setCurrentStatus(status.type,status.customStatus);
            }
            else{
                StatusManager::get()->setCurrentStatus(status.type);
            }
        }
    if (packet_type == "status_update"){
            matjson::Value data = matjson::parse(packet_data);
            if (!data.contains("accID")) {
                return;
            }
            int accountID = data["accID"].as_int();
            status_mod::Status status;
            if (!data.contains("status")) {
                return;
            }
            status.type = status_mod::getStatusTypeFromString(data["status"].as_string().c_str());
            status.hasCustomStatus = data.contains("customStatus");
            if (data.contains("customStatus")){
                status.customStatus = data["customStatus"].as_string().c_str();
            }
            StatusManager::get()->updateUserStatus(accountID,status);
        }

}


void ServerListener::sendMessage(std::string message) {
    if (StatusManager::get()->isWSOpen && ws != nullptr) {
        ws->send(message);
    }
}
void ServerListener::closeSocketAndDeauth() {
    if (StatusManager::get()->isWSOpen && ws != nullptr) {
        ws->close();
    }
    if (Mod::get()->getSavedValue<bool>("authenticated") == true){
        Mod::get()->setSavedValue<bool>("authenticated",false);
        Mod::get()->setSavedValue<std::string>("token","");
    }
    StatusManager::get()->reset();
}

void ServerListener::open() {

    StatusManager::get()->isWSOpen = true;

    ws.release();
    // ws.reset(WebSocket::from_url("ws://164.152.25.111:7438"));
    geode::log::info("connecting...");
    ws.reset(WebSocket::from_url(fmt::format("ws://ws.{}/", StatusManager::get()->domain)));
}
