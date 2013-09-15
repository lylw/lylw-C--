#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#include <cocos2d.h>
#include "GameDef.h"

class GameMap;
class CharacterFrameData;
class ObjCharacter : public cocos2d::Sprite
{
public:
    ObjCharacter(const GUID_t& guid, const ObjType& objType = ObjType::OBJ_TYPE_INVALID);
    virtual ~ObjCharacter();

public:
    void init(const AvatarStyle& avatarStyle);

public:
    void setGUID(const GUID_t& guid);
    const AvatarStyle& avatarStyle() const;
    
    void setBodyStyle(const uint16& bodyStyle);
    void setClothesStyle(const uint16& bodyStyle);
    void setHairStyle(const uint16& bodyStyle);
    void setWeaponStyle(const uint16& bodyStyle);

public:
    void moveTo(const cocos2d::Point& target);
    void moveFinished();

private:
    GUID_t guid_;
    ObjType type_;
    GameMap* currentMap_;
    AvatarStyle avatarStyle_;
    uint16 moveSpeed_;
    CharacterDirection direction_;
    CharacterDirection lastDirection_;
    bool isMoving_;
    bool reMoving_;

private:
    cocos2d::Sequence* moveSequenceAction_;
    cocos2d::MoveTo* moveAction_;
    cocos2d::RepeatForever* walkRepeatAction_;

};

#endif