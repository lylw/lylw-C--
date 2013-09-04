#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#include <cocos2d.h>
#include "GameDef.h"

class GameMap;
class ObjCharacter : public cocos2d::Sprite
{
public:
    ObjCharacter(GUID_t guid, const CharacterID_t& characterType);
    virtual ~ObjCharacter();

protected:
    virtual void moveTo(const cocos2d::Point& target);

private:
    GUID_t guid_;
    GameMap* currentMap_;
    cocos2d::Point mapCoordinate_;
};

#endif