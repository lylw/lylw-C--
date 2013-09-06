#ifndef __RESOURCE_CACHED_MANAGER_H__
#define __RESOURCE_CACHED_MANAGER_H__

#include <cocos2d.h>
#include "Common.h"
#include "Singleton.h"
#include "GamePath.h"
#include "GameDef.h"

std::string ActionAnimateType[] = { "%s_%d_standard", "%s_%d_attack", "%s_%d_walk", "%s_%d_run", "%s_%d_action_%d", "%s_%d_die" };

class ResourceCachedManager
    : public Singleton<ResourceCachedManager>
{
private:
    ResourceCachedManager()
    {
        cocos2d::FileUtils::getInstance()->addSearchPath(GamePath::CHARACTER_DIR.c_str());
        cocos2d::FileUtils::getInstance()->addSearchPath(GamePath::MAP_DIR.c_str());
    }

public:
    void loadCharacterAnimation(ObjType objType, ActionType actionType)
    {
        // ��ͼƬ�����������浽ȫ�ֵ���������
        std::string hero_path = GamePath::CHARACTER_DIR + ".png";
        cocos2d::Texture2D *heroTexture = cocos2d::TextureCache::getInstance()->addImage(hero_path.c_str());
    }

private:
    std::string objTypeStrMapping_[OBJ_TYPE_MAX_FLAG];
    std::string actionTypeStrMapping_[ACTION_TYPE_MAX_FLAG];
};

#endif // __RESOURCE_CACHED_MANAGER_H__
