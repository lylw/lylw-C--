#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include <cocos2d.h>
#include "Common.h"

enum ViewLayerType
{
    VIEW_LAYER_GAME,
    VIEW_LAYER_UI,
    VIEW_LAYER_DIALOG,

    VIEW_LAYER_MAX_FLAG
};

class GameView : public cocos2d::CCLayer
{
public:
    GameView();
    virtual ~GameView();

public:
    virtual bool initWithName(const std::string& name);
    static GameView* create(const std::string& name);

public:
    //添加一个元素到基本层
    void addGameLayerNode(cocos2d::CCNode* layerNode, uint8 type);

    //移除层
    void removeGameLayerNode(uint8 type, int32 tag, bool cleanup = true);

    //通过tag获取层
    cocos2d::CCNode* getGameLayerNodeByTag(uint8 type, int32 tag);

    //获取基本层
    cocos2d::CCNode* getBasicLayersByType(uint8 type);

private:
    std::string viewName_;

    //基本图层（游戏，UI和对话框）
    cocos2d::CCNode basicLayerNodes_[VIEW_LAYER_MAX_FLAG];
};


#endif