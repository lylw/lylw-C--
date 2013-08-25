#ifndef __STUDIO_LOGO_SCENE_H__
#define __STUDIO_LOGO_SCENE_H__

#include <cocos2d.h>

class StudioLogoScene : public cocos2d::Layer
{
public:
    virtual bool init();  
    static cocos2d::Scene* scene();
    CREATE_FUNC(StudioLogoScene);
};

#endif // __STUDIO_LOGO_SCENE_H__
