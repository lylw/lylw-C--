#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include "GameDef.h"
#include "Singleton.h"

class GameMap;
class MapManager
    : public Singleton<MapManager>
{
public:
    GameMap* createMap(const MapID_t& mapId);
};

#endif