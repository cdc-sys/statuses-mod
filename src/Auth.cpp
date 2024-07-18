#include "Geode/binding/GJAccountManager.hpp"
#include "Geode/ui/Notification.hpp"
#include "LoadingOverlay.hpp"
#include "ServerListener.hpp"
#include "StatusManager.hpp"
#include "Auth.h"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJAccountManager.hpp>
LoadingOverlay* loading;
class MyUploadDelegate : public UploadMessageDelegate {
    protected:
    EventListener<web::WebTask>tokenRequest;
public:
    void uploadMessageFailed(int p0) override {
        loading->fadeOut();
        FLAlertLayer::create("Oops!", "Sending message failed due to an unknown error, the mod will be <cr>disabled</c>.", "OK")->show();
        return;
    }
    void uploadMessageFinished(int p0) override {
        loading->changeStatus("Authenticating: Getting Token.. [3/3]");
        /*web::AsyncWebRequest()
            .post(fmt::format("https://{}/authorize/{}/verify", StatusManager::get()->domain, GJAccountManager::get()->m_accountID))
            .json()
            .then([this](matjson::Value const& object) {
			std::string error;
			std::string text;
			std::string token;
			if (object.contains("error")){
				if (object["error"].type() != matjson::Type::Null){
					error = object["error"].as_string();
				}
			}
			if (object.contains("text")){
				if (object["text"].type() != matjson::Type::Null){
					text = object["text"].as_string();
				}
			}
			if (object.contains("token")){
				if (object["token"].type() != matjson::Type::Null){
					token = object["token"].as_string();
				}
			}
			// geode::log::debug("{}",error);
			// geode::log::debug("{}",text);
			// geode::log::debug("{}",token);
			if (!token.empty()){
				loading->changeStatus(nullptr);
				//Notification::create("[Statuses] Successfully Authorized!",CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),1.f)->show();
				Mod::get()->setSavedValue<bool>("authenticated",true);
				Mod::get()->setSavedValue<std::string>("token",token);
				//geode::log::info("{}",text);
				//geode::log::info("{}",token);
				StatusManager::get()->authFailed = false;
				StatusManager::get()->autoReconnect = true;
				ServerListener::connectAsync();
                std::thread openPopupsWithDelay = std::thread([](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                    Loader::get()->queueInMainThread([]() {
                        loading->fadeOut();
                        ProfilePage::create(GJAccountManager::get()->m_accountID,true)->show();
                    });
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    Loader::get()->queueInMainThread([]() {
                        FLAlertLayer::create("Success!","You've been successfully <cg>authorized</c>.","OK")->show();
                    });
                });
                openPopupsWithDelay.detach();
                
			}
			else{
				loading->fadeOut();
				FLAlertLayer::create("Oops!",fmt::format("Authentication failed:\n<cr>{}</c>",error),"OK")->show();
				StatusManager::get()->authFailed = true;
				return;
			} })
            .expect([](std::string const& error) {
			loading->fadeOut();
			FLAlertLayer::create("Oops!",fmt::format("Authentication failed:\n<cr>{}</c>",error),"OK")->show();
			StatusManager::get()->authFailed = true;
			return; });*/
        auto req = web::WebRequest();

        this->tokenRequest.bind([this](web::WebTask::Event* e){
            if (auto res = e->getValue()){
                if (res->ok()){
                    auto object = res->json()->as_object();
                    std::string error;
			std::string text;
			std::string token;
			if (object.contains("error")){
				if (object["error"].type() != matjson::Type::Null){
					error = object["error"].as_string();
				}
			}
			if (object.contains("text")){
				if (object["text"].type() != matjson::Type::Null){
					text = object["text"].as_string();
				}
			}
			if (object.contains("token")){
				if (object["token"].type() != matjson::Type::Null){
					token = object["token"].as_string();
				}
			}
			// geode::log::debug("{}",error);
			// geode::log::debug("{}",text);
			// geode::log::debug("{}",token);
			if (!token.empty()){
				loading->changeStatus(nullptr);
				//Notification::create("[Statuses] Successfully Authorized!",CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),1.f)->show();
				Mod::get()->setSavedValue<bool>("authenticated",true);
				Mod::get()->setSavedValue<std::string>("token",token);
				//geode::log::info("{}",text);
				//geode::log::info("{}",token);
				StatusManager::get()->authFailed = false;
				StatusManager::get()->autoReconnect = true;
				ServerListener::connectAsync();
                std::thread openPopupsWithDelay = std::thread([](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                    Loader::get()->queueInMainThread([]() {
                        loading->fadeOut();
                        ProfilePage::create(GJAccountManager::get()->m_accountID,true)->show();
                    });
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    Loader::get()->queueInMainThread([]() {
                        FLAlertLayer::create("Success!","You've been successfully <cg>authorized</c>.","OK")->show();
                    });
                });
                openPopupsWithDelay.detach();
                
			}
			else{
				loading->fadeOut();
				FLAlertLayer::create("Oops!",fmt::format("Authentication failed:\n<cr>{}</c>",error),"OK")->show();
				StatusManager::get()->authFailed = true;
				return;
			}
                }
                else{
                    loading->fadeOut();
			        FLAlertLayer::create("Oops!",fmt::format("Authentication failed:\n<cr>{}</c>",res->code()),"OK")->show();
			        StatusManager::get()->authFailed = true;
			        return; 
                }
            }
        });

        auto reqTask = req.post(fmt::format("https://{}/authorize/{}/verify", StatusManager::get()->domain, GJAccountManager::get()->m_accountID));

        this->tokenRequest.setFilter(reqTask);
    }
    static MyUploadDelegate* create() {
        MyUploadDelegate* ret = new MyUploadDelegate();
        if (ret) {
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
class $modify(AuthHook,MenuLayer){
    bool init(){
        if (!MenuLayer::init()) return false;
        if (GJAccountManager::get()->m_accountID == 0){
            return true;
        }
        if (Mod::get()->getSavedValue<bool>("authenticated") == true&&StatusManager::get()->autoReconnect&&StatusManager::get()->isWSOpen == false) {
            ServerListener::connectAsync();
        }
        else{
            if (Mod::get()->getSavedValue<bool>("auth-info-popup-showed") != true){
                std::thread openAuthPopup = std::thread([this](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    Loader::get()->queueInMainThread([this](){
                        this->showAuthPopup();
                    });
                });
                openAuthPopup.detach();
            }
        }
        return true;
    }
    void showAuthPopup(){
        createQuickPopup("Welcome!","Hello! \nI see you've just <cj>installed</c> the <cg>Statuses Mod</c>, would you like to go to your <co>profile</c> to <cy>authenticate</c>?\n<cb>(To authenticate you need to press the Status Icon on your profile)</c>","Later","OPEN",
            [](auto,bool btn2){
                if (btn2){
                    ProfilePage::create(GJAccountManager::get()->m_accountID,true)->show();
                }
            });
        Mod::get()->setSavedValue<bool>("auth-info-popup-showed",true);
    }
};
    
void Auth::authenticationStart() {
        bool isLoggedIn = false;
        auto GAM = GJAccountManager::get();
        if (GAM->m_accountID == 0) {
            FLAlertLayer::create("Oops!", "You need to <cg>log in</c> to use this mod.", "OK")->show();
            return;
        }
        loading = LoadingOverlay::create("Authenticating: Getting Code.. [1/3]");
        loading->show();
        auto req = web::WebRequest();

        this->codeRequest.bind([this](web::WebTask::Event* e){
            if (auto res = e->getValue()){
                if (res->ok()){
                    auto object = res->json()->as_object();
                    auto GAM = GJAccountManager::get();
			        loading->changeStatus("Authenticating: Sending Message.. [2/3]");
    	            std::string authCode = object["authCode"].as_string();
			        auto GLM = GameLevelManager::get();
			        GLM->m_uploadMessageDelegate = MyUploadDelegate::create();
			        GLM->uploadUserMessage(28167925,authCode,"hi this is for auth plz delete");
			        StatusManager::get()->authFailed = false;
                    this->release();
                }
                else{
                    loading->fadeOut();
			        FLAlertLayer::create("Oops!",fmt::format("Authentication failed:\n<cr>{}</c>",res->code()),"OK")->show();
			        return; 
                }
            } else if(auto progress = e->getProgress()){
                geode::log::info("{}",progress->downloadProgress());
            } else if (e->isCancelled()){
                geode::log::info("guh??");
            }
        });
        geode::log::info("https://{}/authorize/{}/get_code", StatusManager::get()->domain, GJAccountManager::get()->m_accountID);
        this->codeReqTask = req.get(fmt::format("https://{}/authorize/{}/get_code", StatusManager::get()->domain, GJAccountManager::get()->m_accountID));

        this->codeRequest.setFilter(this->codeReqTask);
}
Auth* Auth::create() {
        auto ret = new Auth();
        if (ret) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

class $modify(DeauthHook,GJAccountManager){
    /*void unlinkFromAccount(){
        GJAccountManager::unlinkFromAccount();
        if (!Mod::get()->getSavedValue<bool>("authenticated")){
            return;
        }
        ServerListener::closeSocketAndDeauth();
        Notification::create(" [Statuses] Logged out, after you log in please authenticate again.",CCSprite::createWithSpriteFrameName("exMark_001.png"))->show();
    }*/
};
