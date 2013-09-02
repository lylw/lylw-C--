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

    this->setTouchEnabled(true);
    this->onLoad();
    return true;
}

void StudioLogoScene::onLoad()
{
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

    this->addChild(logoSprite);
    logoSprite->runAction(sequence);
}

void StudioLogoScene::onExit()
{

}

void StudioLogoScene::onDestroy()
{

}