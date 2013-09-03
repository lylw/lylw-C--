#include "MapManager.h"
#include "GameConfig.h"
#include "GamePath.h"
#include "GameMap.h"

bool MapManager::init()
{
    //增加地图搜索目录
    cocos2d::FileUtils::getInstance()->addSearchPath(GamePath::MAP_DIR.c_str());

    return true;
}

GameMap* MapManager::createMap(const MapID_t& mapId)
{
    const std::string& filename = MapConfig::getInstance().getMapFileName(mapId);

    return new GameMap(mapId);
}