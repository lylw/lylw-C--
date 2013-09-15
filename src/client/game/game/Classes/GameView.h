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
    //���һ��Ԫ�ص�������
    void addGameLayerNode(cocos2d::CCNode* layerNode, uint8 type);

    //�Ƴ���
    void removeGameLayerNode(uint8 type, int32 tag, bool cleanup = true);

    //ͨ��tag��ȡ��
    cocos2d::CCNode* getGameLayerNodeByTag(uint8 type, int32 tag);

    //��ȡ������
    cocos2d::CCNode* getBasicLayersByType(uint8 type);

private:
    std::string viewName_;

    //����ͼ�㣨��Ϸ��UI�ͶԻ���
    cocos2d::CCNode basicLayerNodes_[VIEW_LAYER_MAX_FLAG];
};


#endif