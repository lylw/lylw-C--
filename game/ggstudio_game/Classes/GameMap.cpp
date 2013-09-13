#include "GameDef.h"
#include "GameMap.h"
#include "ResourceCachedManager.h"
#include "ObjPlayer.h"
#include <random>

GameMap::GameMap(const MapID_t& mapId)
    : tiledMap_(nullptr), mapId_(mapId), player_(nullptr), mapX_(0), mapY_(0)
{
}

GameMap::~GameMap()
{
    tiledMap_->cleanup();
    SAFE_DELETE(tiledMap_);
}

bool GameMap::init(void)
{
    this->setTouchEnabled(true);

    std::string resource = GamePath::MAP_DIR + MapConfig::getInstance().getMapFileName(mapId_);
    std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(resource.c_str());

    //加载地图
    tiledMap_ = new cocos2d::TMXTiledMap();
    FileUtils::getInstance()->addSearchPath(GamePath::MAP_DIR.c_str());
    tiledMap_->initWithTMXFile(fullPath.c_str());
    tiledMap_->setPosition(0, 0);
    this->addChild(tiledMap_, 0);

    //重置所有图块zorder
    Size mapSize = tiledMap_->getMapSize();
    Object* layer_obj = nullptr;
    CCARRAY_FOREACH(tiledMap_->getChildren(), layer_obj)
    {
        TMXLayer* layer = static_cast<TMXLayer*>(layer_obj);

        for(uint32 y = 0; y < layer->getLayerSize().height; ++y)
        {
            int zorder = mapSize.height - y;
            for(uint32 x = 0; x < layer->getLayerSize().width; ++x)
            {
                cocos2d::Sprite* sprite = layer->getTileAt(ccp(x, y));
                if (sprite != nullptr)
                {
                    sprite->setZOrder(zorder);
                    //sprite->setColor(cocos2d::Color3B(100, zorder*2.5, 100));   //颜色用于区分地图图层的层次
                    //CCLOG("current tile(x = %d, y=%d, zorder=%d)", x, y, zorder);
                }
            }
        }
    }


    onLoadCompleted();

    return true;
}

void GameMap::addCharacter(ObjCharacter* character)
{
    int zorder = tiledMap_->getMapSize().height - character->getPosition().y / tiledMap_->getTileSize().height;
    tiledMap_->addChild(character, zorder);
}

void GameMap::onLoadCompleted()
{
    AvatarStyle avatarStyle;
    avatarStyle.body = 20001;

    //创建角色
    player_ = new ObjPlayer(696969);
    player_->init(avatarStyle);
    player_->setPosition(ccp(73, 99));

    //把角色调整到相应的层中
    //tiledMap_->reorderChild(player_, MapLayer::MAP_LAYER_CHARACTER);
    this->addCharacter(player_);

    schedule(schedule_selector(GameMap::repositionSprite));

    //创建一些随机角色
    std::default_random_engine generator;  
    std::uniform_int_distribution<int> r_avatar(20002, 20008);
    std::uniform_int_distribution<int> r_point_x(0, EGLView::getInstance()->getFrameSize().width);
    std::uniform_int_distribution<int> r_point_y(0, EGLView::getInstance()->getFrameSize().height);

    for (int i = 0; i < 200; ++i)
    {
        avatarStyle.body = r_avatar(generator);
        ObjPlayer* random_player = new ObjPlayer(i);
        random_player->init(avatarStyle);
        random_player->setPosition(cocos2d::Point(r_point_x(generator), r_point_y(generator)));
        addCharacter(random_player);
    }

}

void GameMap::repositionSprite(float dt)
{
    int zorder = tiledMap_->getMapSize().height - player_->getPosition().y / tiledMap_->getTileSize().height;
    player_->setZOrder(zorder-1);
    CCLOG("rezorder = %d",zorder);

}

void GameMap::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    //获取第一个触摸点
    CCTouch* touch = reinterpret_cast<CCTouch *>(pTouches->anyObject());


    //获取触摸坐标
    //注意，升级到cocos2d-3.0 beta版本后，getLocationInView()是没有参数的。
    CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

    //由触摸坐标转到地图坐标
    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(touchPoint.x - mapX_, touchPoint.y - mapY_));

    //如果当前格子不为空
    if (mapPoint.x != -1)
    {
        //取得图块标志层
        CCTMXLayer* flag_layer = tiledMap_->layerNamed("flag_layer");
        if (flag_layer == nullptr)
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
            if (dictionary != nullptr && dictionary->count() != 0)
            {
                const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");

                //如果没有设置block属性或者设置了block属性但不等于1，则表示当前图块是可以通行的
                if (tileFlag == nullptr || tileFlag->uintValue() == 0)
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
    cocos2d::Size szMapSize = tiledMap_->getMapSize();
    cocos2d::Size szTile = tiledMap_->getTileSize();

    int cox = pos.x / szTile.width;
    int coy = szMapSize.height - pos.y / szTile.height;

    if ((cox >= 0) && (cox < szMapSize.width) && (coy >= 0) && (coy < szMapSize.height))
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