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
        GameView* Map = new GameView();
        if (Map != nullptr && Map->initWithName(name))
        {
            Map->autorelease();
            return Map;
        }
        else
        {
            delete Map;
            Map = nullptr;
        }
    }

    return nullptr;
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