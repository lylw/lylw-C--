#include "TestTileScene.h"
#include "CodePage.hpp"
#include "GamePath.h"

USING_NS_CC;

Scene* TestTileScene::scene()
{
    Scene *scene = Scene::create();
    TestTileScene *layer = TestTileScene::create();

    scene->addChild(layer);
    return scene;
}

bool TestTileScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    std::string resource = GamePath::MAP_DIR + "standard.tmx";
    CCTMXTiledMap* map = new CCTMXTiledMap();
    map->initWithTMXFile(resource.c_str());

    map->setPosition(0,0);
    this->addChild(map, 1);

    return true;
}
