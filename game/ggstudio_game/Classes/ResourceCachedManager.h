#ifndef __RESOURCE_CACHED_MANAGER_H__
#define __RESOURCE_CACHED_MANAGER_H__

#include <cocos2d.h>
#include "Common.h"
#include "Singleton.h"
#include "GamePath.h"
#include "GameDef.h"

static const uint8 WALK_FRAME_COUNT = 4;   //角色行走动画帧数

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
    void loadCharacterAnimation(const AvatarStyle& avatarStyle)
    {
        char characterResFilename[255] = {0};
        sprintf(characterResFilename, "body_%d_walk.png", avatarStyle.body);
        
        // 将图片生成纹理，保存到全局的纹理缓冲区
        std::string texturePath = GamePath::CHARACTER_DIR + characterResFilename;
        cocos2d::Texture2D* characterTexture = cocos2d::TextureCache::getInstance()->addImage(texturePath.c_str());

        //取得动画帧
        cocos2d::SpriteFrame* characterFrames[4];
        cocos2d::Array* animFrames = cocos2d::Array::create();

        /*const uint32& frameWidth = characterTexture->getContentSize().width / 4;
        const uint32& frameHeight = characterTexture->getContentSize().height / 4;

        for (int i = 0; i < 4; ++i)
        {
            characterFrames[i] = 
                cocos2d::SpriteFrame::createWithTexture(characterTexture, cocos2d::CCRectMake(frameWidth * i, 0, frameWidth, frameHeight));
            animFrames->addObject(characterFrames[i]);
        }

        cocos2d::Animation* animation = new cocos2d::Animation();
        animation->initWithSpriteFrames(animFrames, 0.2f);
        animFrames->release();

        heroSprite = CCSprite::createWithSpriteFrame(character_frames[0]);
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame();


        animate_ = CCAnimate::create(animation);*/
    }

};

#endif // __RESOURCE_CACHED_MANAGER_H__
