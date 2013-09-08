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
    virtual void setBodyStyle(const uint16& bodyStyle);
    virtual void setClothesStyle(const uint16& bodyStyle);
    virtual void setHairStyle(const uint16& bodyStyle);
    virtual void setWeaponStyle(const uint16& bodyStyle);

private:
    GUID_t guid_;
    ObjType type_;
    GameMap* currentMap_;
    cocos2d::Point point_;
    AvatarStyle avatarStyle_;
};

#endif