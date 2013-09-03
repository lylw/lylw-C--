#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#include <cocos2d.h>
#include "GameType.h"

class ObjCharacter : public cocos2d::Sprite
{
public:
    ObjCharacter(const CharacterType& characterType);
    virtual ~ObjCharacter();

public:
    void moveTo(const cocos2d::Point& target);


};

#endif