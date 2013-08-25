#include "StudioLogoScene.h"
#include <cocos-ext.h>
#include "CodePage.hpp"

USING_NS_CC;

Scene* StudioLogoScene::scene()
{
    Scene *scene = Scene::create();
    StudioLogoScene *layer = StudioLogoScene::create();

    scene->addChild(layer);
    return scene;
}

bool StudioLogoScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    CCSprite* logoSprite = CCSprite::create("title/ggstudio.png");
    logoSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    logoSprite->setOpacity(0);

    CCSequence* sequence = CCSequence::create(
        CCDelayTime::create(1.5f),
        CCFadeIn::create(0.5f),
        CCDelayTime::create(1.5f),
        CCFadeOut::create(0.5f),
        NULL);

    CCTextFieldTTF* input = CCTextFieldTTF::textFieldWithPlaceHolder(a2u("ÊäÈëÕÊºÅ").c_str(), "ËÎÌå", 16.0f);
    input->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    input->attachWithIME();

    CCTextFieldTTF* input2 = CCTextFieldTTF::textFieldWithPlaceHolder(a2u("ÊäÈëÃÜÂë").c_str(), "ËÎÌå", 16.0f);
    input2->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - 120));
    input2->attachWithIME();
    //input->setDelegate(this);

    this->addChild(input);
    this->addChild(input2);
    //this->addChild(logoSprite);

    input->attachWithIME();

    logoSprite->runAction(sequence);

    return true;
}