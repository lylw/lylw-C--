#include "GameView.h"

GameView::GameView()
{

}

GameView::~GameView()
{

}

bool GameView::initWithName(const std::string& name)
{
    if (!name.empty())
    {
        viewName_ = name;

        return true;
    }

    return false;
}

GameView* GameView::create(const std::string& name)
{
    if (!name.empty())
    {
        GameView* scene = new GameView();
        if (scene != NULL && scene->initWithName(name))
        {
            scene->autorelease();
            return scene;
        }
        else
        {
            delete scene;
            scene = NULL;
        }
    }

    return NULL;
}

void GameView::addGameLayerNode(cocos2d::CCNode* layer, uint8 type)
{
    basicLayerNodes_[type].addChild(layer);
}

void GameView::removeGameLayerNode(uint8 type, int32 tag, bool cleanup/* = true*/)
{
    basicLayerNodes_[type].removeChildByTag(tag, cleanup);
}

cocos2d::CCNode* GameView::getGameLayerNodeByTag(uint8 type, int32 tag)
{
    return basicLayerNodes_[type].getChildByTag(tag);
}

cocos2d::CCNode* GameView::getBasicLayersByType(uint8 type)
{
    return &basicLayerNodes_[type];
}