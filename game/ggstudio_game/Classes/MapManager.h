#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include "GameDef.h"
#include "Singleton.h"

class GameMap;
class TMXTiledMap;
class MapManager
    : public Singleton<MapManager>
{
public:
    bool init();
    GameMap* createMap(const MapID_t& mapId);

private:
    std::map<MapID_t, TMXTiledMap*> allocatedMaps_;
};

#endif