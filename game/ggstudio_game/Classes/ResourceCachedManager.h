#ifndef __RESOURCE_CACHED_MANAGER_H__
#define __RESOURCE_CACHED_MANAGER_H__

#include <cocos2d.h>
#include "Common.h"
#include "Manager.h"
#include "GamePath.h"
#include "GameDef.h"

// CharacterFrameWrapper
// ���ã����ڹ���һ����ɫ����ͼ��֡����
//       ����֡��һ����ά������ɣ��÷������������ͼ��֡����
class CharacterFrameData 
    : public cocos2d::Object
{
public:
    //ͨ��֡���鴴��һ���Զ��ͷŵ��µ�֡���ݶ���
    //ע������̳���cocos2d::Object��ͨ������autorelease����һ�����ü���
    static CharacterFrameData* createWithArray(cocos2d::Array** arr)
    {
        CharacterFrameData* characterFrameData = new CharacterFrameData();
        characterFrameData->initWithFrameArray(arr);
        characterFrameData->autorelease();
        return characterFrameData;
    }

    //ͨ����ɫ����ͼ�����ȡ�÷�������߶���
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
    //��ʼ��֡���飬��ResourceCachedManager����Ԫ
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
    //��avatar��ʽ��þ���֡
    CharacterFrameData* avatarStyleToFrameData(const AvatarStyle& avatarStyle, bool autoload = true)
    {
        std::string characterResFilename = avatarStyleToResName(avatarStyle);

        //����ʽ�Ƿ��Ѽ���
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
    //����avatar��ʽ���ؽ�ɫ����ͼ����֡����
    CharacterFrameData* loadCharacterSpriteFrames(const AvatarStyle& avatarStyle)
    {
        //��avatar��ʽȡ����Դ����
        std::string characterResFilename = avatarStyleToResName(avatarStyle);

        //����ʽ�Ƿ��Ѽ���
        auto iter = characterResourceList_.find(characterResFilename);
        if (iter != characterResourceList_.end())
            return &iter->second;

        // ��ͼƬ�����������浽ȫ�ֵ���������
        std::string texturePath = GamePath::CHARACTER_DIR + characterResFilename + ".png";
        cocos2d::Texture2D* characterTexture = cocos2d::TextureCache::getInstance()->addImage(texturePath.c_str());

        //ȡ�ö���֡
        cocos2d::SpriteFrame* characterFrames[DIRECTION_MAX_FLAG][CharacterParameter::kWalkFrameCount];
        cocos2d::Array* animFrames[DIRECTION_MAX_FLAG];
        for (int i = 0; i < DIRECTION_MAX_FLAG; ++i)
            animFrames[i] = cocos2d::Array::create();

        //ȡ��ÿһ֡����ͼƬ�Ŀ��
        const uint32& frameWidth = characterTexture->getContentSize().width / CharacterParameter::kWalkFrameCount;
        const uint32& frameHeight = characterTexture->getContentSize().height / DIRECTION_MAX_FLAG;

        for (int i = 0; i < CharacterParameter::kWalkFrameCount; ++i)
        {
            for (int j = 0; j < DIRECTION_MAX_FLAG; ++j)
            {
                //ͨ��������֡
                characterFrames[j][i] = 
                    cocos2d::SpriteFrame::createWithTexture(characterTexture, cocos2d::CCRectMake(frameWidth * j, frameHeight * i, frameWidth, frameHeight));

                //��֡��������
                //cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(characterFrames[j][i], characterResFilename.c_str());

                //��ÿ֡���뵽����
                animFrames[i]->addObject(characterFrames[j][i]);
            }
        }

        return CharacterFrameData::createWithArray(animFrames);
    }

    //ͨ��avatar��ʽת��Ϊ��Ӧ���ļ�����
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
