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
    TILE_FLAG_ACROSSABLE,   //��ͨ�е�
    TILE_FLAG_BLOCK,        //�ϰ���
    TILE_FLAG_COVER         //�ڵ���
};

//ͼ��
enum MapLayer
{
    MAP_LAYER_UNDERWORLD = -1,  //�ص�
    MAP_LAYER_GROUND     = 0,   //�ر��
    MAP_LAYER_CHARACTER  = 1,   //��ɫ�㣨����NPC������ȣ�
    MAP_LAYER_OBJECTS    = 2,   //��ͼ�����
    MAP_LAYER_TILED_FLAG = 3    //ͼ���־��
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