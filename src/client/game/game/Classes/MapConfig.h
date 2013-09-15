#ifndef __Map_CONFIG_H__
#define __Map_CONFIG_H__

#include <map>
#include "Singleton.h"
#include "GameDef.h"

struct MapFileInfo
{
    MapFileInfo(const MapID_t& mapid, const std::string& file)
        : id(mapid), filename(file)
    {
    }

    MapID_t id;
    std::string filename;
};

class TMXTiledMap;
class MapConfig
    : public Singleton<MapConfig>
{
public:
    bool init();
    std::string getMapFileName(MapID_t id) const;

private:
    bool loadConfig();

private:
    std::map<MapID_t, MapFileInfo> tiledMapList_;
};

#endif // !__Map_CONFIG_H__
