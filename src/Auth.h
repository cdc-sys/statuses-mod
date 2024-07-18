#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
using namespace geode::prelude;

class Auth : public cocos2d::CCObject {
    protected:
    EventListener<web::WebTask>codeRequest;
    web::WebTask codeReqTask;
    public:
    void authenticationStart();
    static Auth* create();
};