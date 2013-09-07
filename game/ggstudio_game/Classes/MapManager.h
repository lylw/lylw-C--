#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include <cocos2d.h>
#include "GameDef.h"
#include "Manager.h"

class GameMap;
class MapManager
    : public Manager<MapManager>
{
public:
    bool init();
    void destroy();
    GameMap* createMap(const MapID_t& mapId);

private:
    std::map<MapID_t, cocos2d::TMXTiledMap*> allocatedMaps_;
};

#endif