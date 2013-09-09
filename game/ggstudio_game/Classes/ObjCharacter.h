#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#include <cocos2d.h>
#include "GameDef.h"

class GameMap;
class ObjCharacter : public cocos2d::Sprite
{
public:
    ObjCharacter(const GUID_t& guid, const ObjType& objType = ObjType::OBJ_TYPE_INVALID);
    virtual ~ObjCharacter();

public:
    void setGUID(const GUID_t& guid);
    const AvatarStyle& avatarStyle() const;
    
    void setBodyStyle(const uint16& bodyStyle);
    void setClothesStyle(const uint16& bodyStyle);
    void setHairStyle(const uint16& bodyStyle);
    void setWeaponStyle(const uint16& bodyStyle);

public:
    void setWalkAnimate(cocos2d::Animate* moveAnimate);
    void moveTo(const cocos2d::Point& target);

private:
    GUID_t guid_;
    ObjType type_;
    GameMap* currentMap_;
    AvatarStyle avatarStyle_;

private:
    cocos2d::ActionInterval* moveAction_;
    cocos2d::ActionInterval* walkRepeatAction_;
};

#endif