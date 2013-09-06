#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#include <cocos2d.h>
#include "GameDef.h"

class GameMap;
class ObjCharacter : public cocos2d::Sprite
{
protected:
    ObjCharacter(const GUID_t& guid, const ObjType& objType);
    virtual ~ObjCharacter();

protected:
    bool init(cocos2d::Texture2D* texture);
    bool onLoadComplete();


private:
    GUID_t guid_;
    ObjType type_;
    GameMap* currentMap_;
    cocos2d::Point mapCoordinate_;
};

#endif