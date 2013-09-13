#ifndef __GAME_Map_H__
#define __GAME_Map_H__

#include <cocos2d.h>
#include "GameDef.h"
#include "GamePath.h"
#include "CodePage.hpp"
#include "GameConfig.h"

USING_NS_CC;

enum TileFlag
{
    TILE_FLAG_ACROSSABLE,   //可通行的
    TILE_FLAG_BLOCK,        //障碍物
    TILE_FLAG_COVER         //遮挡物
};

//图层
enum MapLayer
{
    MAP_LAYER_UNDERWORLD = -1,  //地底
    MAP_LAYER_GROUND     = 0,   //地表层
    MAP_LAYER_CHARACTER  = 1,   //角色层（包括NPC，怪物等）
    MAP_LAYER_OBJECTS    = 2,   //地图对象层
    MAP_LAYER_TILED_FLAG = 3    //图块标志层
};

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

private:
    void onLoadCompleted();
    CCPoint tileCoordinateFromPos(CCPoint pos);
    void touchMap(const cocos2d::Point& touchPoint);
    void repositionSprite(float dt);



private:
    MapID_t mapId_;
    cocos2d::TMXTiledMap* tiledMap_;
    float mapX_, mapY_;
    ObjPlayer* player_;
    //CCSprite* heroSprite;
    ActionInterval* moveTo_;
    ActionInterval* repeat_;
    CCAnimate *animate_;
};

#endif