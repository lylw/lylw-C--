#ifndef __GAME_Map_H__
#define __GAME_Map_H__

#include <cocos2d.h>
#include "GameDef.h"
#include "GamePath.h"
#include "CodePage.hpp"
#include "GameConfig.h"

USING_NS_CC;


class ObjPlayer;
class ObjCharacter;

class GameMap 
    : public cocos2d::Layer
{
public:
    GameMap(const MapID_t& mapId);
    virtual ~GameMap();

public:
    bool init();
    void addCharacter(ObjCharacter* character);
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

public:
    void objectEnterMap(ObjCharacter* character);
    void objectLeaveMap(ObjCharacter* character);
    uint32 getTiledFlagByPosition(const cocos2d::Point& point);

private:
    void onLoadCompleted();
    CCPoint tileCoordinateFromPos(CCPoint pos);
    void touchMap(const cocos2d::Point& touchPoint);
    //void repositionSprite(float dt);
    void showMapDiscription(float dt);

private:
    MapID_t mapId_;
    cocos2d::TMXTiledMap* tiledMap_;
    cocos2d::TMXLayer* flagLayer_;
    float mapX_, mapY_;
    ObjPlayer* player_;
    //CCSprite* heroSprite;
    ActionInterval* moveTo_;
    ActionInterval* repeat_;
    CCAnimate *animate_;
};

#endif