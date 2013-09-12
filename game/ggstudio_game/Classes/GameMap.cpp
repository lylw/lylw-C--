#include "GameDef.h"
#include "GameMap.h"
#include "ResourceCachedManager.h"
#include "ObjPlayer.h"
#include <random>

GameMap::GameMap(const MapID_t& mapId)
    : tiledMap_(NULL), mapId_(mapId), player_(NULL)
{
}

GameMap::~GameMap()
{
    tiledMap_->cleanup();
    SAFE_DELETE(tiledMap_);
}

bool GameMap::init(void)
{
    mapX_ = 0;
    mapY_ = 0;
    this->setTouchEnabled(true);
    onLoadCompleted();

    return true;
}

void GameMap::onLoadCompleted()
{
    std::string resource = GamePath::MAP_DIR + MapConfig::getInstance().getMapFileName(mapId_);
    std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(resource.c_str());

    //加载地图
    tiledMap_ = new cocos2d::TMXTiledMap();
    FileUtils::sharedFileUtils()->addSearchPath(GamePath::MAP_DIR.c_str());
    tiledMap_->initWithTMXFile(fullPath.c_str());
    //tiledMap_->setPosition(0, 0);
    Size s = tiledMap_->getContentSize();
    tiledMap_->setPosition(Point(-s.width / 2, 0));
    this->addChild(tiledMap_, 0);

    AvatarStyle avatarStyle;
    avatarStyle.body = 20001;

    //创建角色
    player_ = new ObjPlayer(696969);
    player_->init(avatarStyle);

    TMXLayer* layer = tiledMap_->getLayer("Trees");
    if (layer != NULL)
    {
        //layer->appendChild(player_);
        player_ = (ObjPlayer*)layer->getTileAt(Point(29,29));
        //layer->addChild(player_, player_->getPositionY());
    }

    //把角色调整到相应的层中
    //tiledMap_->reorderChild(player_, MapLayer::MAP_LAYER_CHARACTER);
    //tiledMap_->addChild(player_, player_->getPositionY());


    //创建一些随机角色
    std::default_random_engine generator;  
    std::uniform_int_distribution<int> r_avatar(20002, 20008);

    std::uniform_int_distribution<int> r_point_x(0, 960);
    std::uniform_int_distribution<int> r_point_y(0, 640);

    for (int i = 0; i < 200; ++i)
    {
        avatarStyle.body = r_avatar(generator);
        ObjPlayer* random_player = new ObjPlayer(i);
        random_player->init(avatarStyle);
        random_player->setPosition(cocos2d::Point(r_point_x(generator), r_point_y(generator)));

        //tiledMap_->addChild(random_player);
        //tiledMap_->reorderChild(random_player, tiledMap_->getContentSize().height - random_player->getPositionY());
        //layer->addChild(random_player, random_player->getPositionY(), random_player->getPositionY());
    }

}

void GameMap::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    //获取第一个触摸点
    CCTouch* touch = reinterpret_cast<CCTouch *>(pTouches->anyObject());


    //获取触摸坐标
    //注意，升级到cocos2d-3.0 beta版本后，getLocationInView()是没有参数的。
    CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

#if _DEBUG
    touchMap(touchPoint);
    return;
#endif

    //由触摸坐标转到地图坐标
    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(touchPoint.x - mapX_, touchPoint.y - mapY_));

    //如果当前格子不为空
    if (mapPoint.x != -1)
    {
        //取得图块标志层
        CCTMXLayer* flag_layer = tiledMap_->layerNamed("flag_layer");
        if (flag_layer == NULL)
        {
            return;
        }

        //根据地图坐标获得格子图块的ID
        unsigned int gid = flag_layer->tileGIDAt(mapPoint);

        //图块不为空
        if (gid != 1)
        {
            //根据图块ID获取图块的所有属性
            cocos2d::Dictionary* dictionary = tiledMap_->getPropertiesForGID(gid);
            if (dictionary != NULL && dictionary->count() != 0)
            {
                const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");

                //如果没有设置block属性或者设置了block属性但不等于1，则表示当前图块是可以通行的
                if (tileFlag == NULL || tileFlag->uintValue() == 0)
                {
                    touchMap(touchPoint);
                }
            }
            else
            {
                //如果图块不设置任何属性，则默认当作可以通行
                touchMap(touchPoint);
            }
        }
    }
}

CCPoint GameMap::tileCoordinateFromPos(CCPoint pos)
{
    int cox, coy;
    CCTMXLayer *ly = tiledMap_->getLayer("objects_layer");
    if (ly == NULL)
    {
        return ccp(-1, -1);
    }
    CCSize szLayer = ly->getLayerSize();
    CCSize szTile = tiledMap_->getTileSize();

    cox = pos.x / szTile.width;
    coy = szLayer.height - pos.y / szTile.height;

    if ((cox >= 0) && (cox < szLayer.width) && (coy >= 0) && (coy < szLayer.height))
    {
        return ccp(cox, coy);
    }
    else
    {
        return ccp(-1, -1);
    }
}

void GameMap::touchMap(const cocos2d::Point& touchPoint)
{
    const Point& startPoint = player_->getPosition();
    const Point& targetPoint = touchPoint;

    //目标是否和原点一样
    if (startPoint.equals(targetPoint)) return;

    player_->moveTo(targetPoint);
}

void GameMap::objectEnterMap(ObjCharacter* character)
{

} 