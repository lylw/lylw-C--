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
    void addPlayer(ObjPlayer* character);
    void removePlayer(ObjPlayer* character);
    const cocos2d::Size& getMapSize() const { return tiledMap_->getMapSize(); }
    const cocos2d::Size& getTileSize() const { return tiledMap_->getTileSize(); }

public:
    void objectEnterMap(ObjCharacter* character);
    void objectLeaveMap(ObjCharacter* character);
    uint32 getTiledFlagByPosition(const cocos2d::Point& point);

private:
    void onLoadCompleted();
    CCPoint tileCoordinateFromPos(CCPoint pos);
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void touchMap(const cocos2d::Point& touchPoint);
    void showMapDiscription(float dt);

private:
    MapID_t mapId_;
    cocos2d::TMXTiledMap* tiledMap_;
    cocos2d::TMXLayer* flagLayer_;
    float mapX_, mapY_;
    ObjPlayer* player_;
    ActionInterval* moveTo_;
    ActionInterval* repeat_;
    CCAnimate *animate_;

    std::map<GUID_t, ObjPlayer*> playerList_;
};

#endif