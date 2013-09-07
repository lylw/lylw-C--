#ifndef __RESOURCE_CACHED_MANAGER_H__
#define __RESOURCE_CACHED_MANAGER_H__

#include <cocos2d.h>
#include "Common.h"
#include "Manager.h"
#include "GamePath.h"
#include "GameDef.h"

// CharacterFrameWrapper
// 作用：用于管理一个角色行走图的帧数据
//       所有帧由一个二维数组组成，用方向常量获得行走图的帧动画
class CharacterFrameData 
    : public cocos2d::Object
{
public:
    //通过帧数组创建一个自动释放的新的帧数据对象
    //注：该类继承自cocos2d::Object，通过调用autorelease增加一次引用计数
    static CharacterFrameData* createWithArray(cocos2d::Array** arr)
    {
        CharacterFrameData* characterFrameData = new CharacterFrameData();
        characterFrameData->initWithFrameArray(arr);
        characterFrameData->autorelease();
        return characterFrameData;
    }

    //通过角色行走图方向获取该方向的行走动画
    cocos2d::Animation* getAnimationByDirection(const CharacterDirection& characterDirection)
    {
        cocos2d::Array* frames = characterFrames_[characterDirection];
        return cocos2d::Animation::createWithSpriteFrames(frames, CharacterParameter::kWalkFrameDelay);
    }

    cocos2d::SpriteFrame* getSpriteFrameByDirection(const CharacterDirection& characterDirection)
    {
        cocos2d::Object* frame = characterFrames_[characterDirection][0].objectAtIndex(0);
        return static_cast<cocos2d::SpriteFrame*>(frame);
    }

private:
    //初始化帧数组，和ResourceCachedManager是友元
    void initWithFrameArray(cocos2d::Array** arr)
    {
        CCARRAY_VERIFY_TYPE(*arr, cocos2d::SpriteFrame*);

        for (int i = 0; i < CharacterParameter::kWalkFrameCount; ++i)
            characterFrames_[i] = arr[i];
        //characterFrames_ = arr;
    }

    cocos2d::Array* characterFrames_[DIRECTION_MAX_FLAG];
};

struct ActionFrameData
{

};

class ResourceCachedManager
    : public Manager<ResourceCachedManager>
{
public:
    bool init()
    {
        cocos2d::FileUtils::getInstance()->addSearchPath(GamePath::CHARACTER_DIR.c_str());
        cocos2d::FileUtils::getInstance()->addSearchPath(GamePath::MAP_DIR.c_str());

        return true;
    }

    void destroy()
    {
    }

public:
    //由avatar样式获得精灵帧
    CharacterFrameData* avatarStyleToFrameData(const AvatarStyle& avatarStyle, bool autoload = true)
    {
        std::string characterResFilename = avatarStyleToResName(avatarStyle);

        //该样式是否已加载
        auto iter = characterResourceList_.find(characterResFilename);
        if (iter != characterResourceList_.end())
            return &iter->second;

        if (autoload)
        {
            return loadCharacterSpriteFrames(avatarStyle);
        }

        return NULL;
    }

private:
    //根据avatar样式加载角色行走图精灵帧数组
    CharacterFrameData* loadCharacterSpriteFrames(const AvatarStyle& avatarStyle)
    {
        //由avatar样式取得资源名称
        std::string characterResFilename = avatarStyleToResName(avatarStyle);

        //该样式是否已加载
        auto iter = characterResourceList_.find(characterResFilename);
        if (iter != characterResourceList_.end())
            return &iter->second;

        // 将图片生成纹理，保存到全局的纹理缓冲区
        std::string texturePath = GamePath::CHARACTER_DIR + characterResFilename + ".png";
        cocos2d::Texture2D* characterTexture = cocos2d::TextureCache::getInstance()->addImage(texturePath.c_str());

        //取得动画帧
        cocos2d::SpriteFrame* characterFrames[DIRECTION_MAX_FLAG][CharacterParameter::kWalkFrameCount];
        cocos2d::Array* animFrames[DIRECTION_MAX_FLAG];
        for (int i = 0; i < DIRECTION_MAX_FLAG; ++i)
            animFrames[i] = cocos2d::Array::create();

        //取得每一帧精灵图片的宽高
        const uint32& frameWidth = characterTexture->getContentSize().width / CharacterParameter::kWalkFrameCount;
        const uint32& frameHeight = characterTexture->getContentSize().height / DIRECTION_MAX_FLAG;

        for (int i = 0; i < CharacterParameter::kWalkFrameCount; ++i)
        {
            for (int j = 0; j < DIRECTION_MAX_FLAG; ++j)
            {
                //通过纹理创建帧
                characterFrames[j][i] = 
                    cocos2d::SpriteFrame::createWithTexture(characterTexture, cocos2d::CCRectMake(frameWidth * j, frameHeight * i, frameWidth, frameHeight));

                //把帧缓存起来
                //cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(characterFrames[j][i], characterResFilename.c_str());

                //把每帧放入到数组
                animFrames[i]->addObject(characterFrames[j][i]);
            }
        }

        return CharacterFrameData::createWithArray(animFrames);
    }

    //通过avatar样式转换为相应的文件名称
    std::string avatarStyleToResName(const AvatarStyle& avatarStyle)
    {
        char characterResFilename[255] = {0};
        sprintf(characterResFilename, "body_%d_walk", avatarStyle.body);

        return characterResFilename;
    }

private:
    std::map<std::string, CharacterFrameData> characterResourceList_;
};

#endif // __RESOURCE_CACHED_MANAGER_H__
