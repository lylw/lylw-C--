#include "AppDelegate.h"
#include "GameMap.h"
#include "GameConfig.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    Director* director = Director::getInstance();
    EGLView* eglView = EGLView::getInstance();
    director->setProjection(kCCDirectorProjection2D);
    director->setDepthTest(true);

    director->setOpenGLView(eglView);
	
    // turn on display FPS
#if _DEBUG
    director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //load config
    MapConfig::getInstance().init();

    //����һ����ͼʵ��
    GameMap* gameMap = new GameMap(10000);
    gameMap->init();

    //������������
    cocos2d::Scene* scene = cocos2d::Scene::create();
    scene->addChild(gameMap);

    // run
    director->runWithScene(CCTransitionFade::create(1.5f, scene));

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}