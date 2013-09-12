#include "GamePath.h"
#include "MapConfig.h"
#include "tinyxml.h"
#include <cocos2d.h>

bool MapConfig::init()
{
    return loadConfig();
}

bool MapConfig::loadConfig()
{
    std::string configFile = GamePath::CONFIG_DIR + "maplist.xml";

    TiXmlDocument document;
    bool loadResult = document.LoadFile(configFile.c_str());
    if (!loadResult)
    {
        return false;
    }

    TiXmlElement* rootElement = document.RootElement();
    if (rootElement != nullptr && !rootElement->NoChildren())
    {
        TiXmlElement* mapElement = rootElement->FirstChildElement();
        for (; mapElement != nullptr; mapElement = mapElement->NextSiblingElement())
        {
            int mapid;
            mapElement->Attribute("id", &mapid);
            const char* filename = mapElement->Attribute("file");

            tiledMapList_.insert(std::make_pair(mapid, MapFileInfo(mapid, filename)));
        }
    }

    return true;
}

std::string MapConfig::getMapFileName(MapID_t id) const
{
    auto iter = tiledMapList_.find(id);
    if (iter != tiledMapList_.end())
    {
        return iter->second.filename;
    }

    return "";
}