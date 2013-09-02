#ifndef __STUDIO_LOGO_SCENE_H__
#define __STUDIO_LOGO_SCENE_H__

#include <cocos2d.h>
#include "GameScene.h"

class StudioLogoScene
    : public GameScene
{
public:
    virtual bool init();  
    static cocos2d::Scene* scene();
    CREATE_FUNC(StudioLogoScene);

public:
    void onLoad();
    void onExit();
    void onDestroy();
};

#endif // __STUDIO_LOGO_SCENE_H__
