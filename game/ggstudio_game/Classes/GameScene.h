#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameView.h"

/*
该类是所有场景类的基类，定义场景的基本方法。
*/

enum GameSceneType
{
    SCENE_TYPE_BEGIN,
    SCENE_TYPE_LOGIN,
    
};

class GameScene : public GameView
{
public:
    GameScene()
    {
    }
    virtual ~GameScene()
    {
    }

    virtual void onEnterScene()
    {
    }

    virtual void onLeaveScene()
    {
    }

    virtual uint8 getSceneType() const
    {
        return SCENE_TYPE_BEGIN;
    }

    virtual void onUpdate()
    {
    }
};

#endif