#include <Geode/Geode.hpp>
class Auth : public cocos2d::CCObject {
    public:
    void authenticationStart();
    static Auth* create();
};