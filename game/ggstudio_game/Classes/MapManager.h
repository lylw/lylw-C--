#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include <cocos2d.h>
#include "GameDef.h"
#include "Singleton.h"

class GameMap;
class MapManager
    : public Singleton<MapManager>
{
public:
    bool init();
    GameMap* createMap(const MapID_t& mapId);

private:
    std::map<MapID_t, cocos2d::TMXTiledMap*> allocatedMaps_;
};

#endif