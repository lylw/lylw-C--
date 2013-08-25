#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameView.h"

/*
���������г�����Ļ��࣬���峡���Ļ���������
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