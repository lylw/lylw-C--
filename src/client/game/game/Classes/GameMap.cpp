#include "GameDef.h"
#include "GameMap.h"
#include "ResourceCachedManager.h"
#include "ObjPlayer.h"
#include <random>
#include "TextureUtils.h"
#include "StringUtil.h"

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

    flagLayer_ = tiledMap_->getLayer("flag_layer");
    if (flagLayer_ != nullptr)
    {
        flagLayer_->setVisible(false);
        cocos2d::Dictionary* dictionary = tiledMap_->getProperties();
        Array* arr = dictionary->allKeys();
        for (int i = 0; i < arr->count(); ++i)
        {
            CCLOG("key[%d] = %s", i, arr->objectAtIndex(i));
        }
    }

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

    schedule(schedule_selector(GameMap::showMapDiscription), 0, 1, 2.0f);
    onLoadCompleted();

    return true;
}

void GameMap::showMapDiscription(float dt)
{
    //先取得地图名吧
    cocos2d::String* mapNamePropertyPtr = tiledMap_->getProperty("_MapName");
    if (mapNamePropertyPtr != nullptr)
    {
        const char* mapName = mapNamePropertyPtr->getCString();
        LabelTTF* lable = LabelTTF::create(mapName, "@幼圆", 30.0f);

        RenderTexture* renderTexture = createStroke(lable, 0.7f, Color3B(0, 0, 120));

        float x = EGLView::getInstance()->getFrameSize().width - lable->getContentSize().height;
        float y = EGLView::getInstance()->getFrameSize().height - EGLView::getInstance()->getFrameSize().height / 4;

        lable->setPosition(ccp(x, y));
        lable->setRotation(90);

        renderTexture->setPosition(ccp(x, y));
        renderTexture->setRotation(90);
        this->addChild(renderTexture, 1);
        this->addChild(lable, 2);

        FadeIn* fadeIn = FadeIn::create(1.5f);


        cocos2d::Array* actionArray = cocos2d::Array::create();
        Sequence* sequence = Sequence::create(fadeIn, NULL);
        lable->runAction(sequence);
    }


    //然后取得描述
    cocos2d::String* mapDescriptionPtr = tiledMap_->getProperty("_MapDescription");
    if (mapDescriptionPtr != nullptr)
    {
        const char* mapDescription = mapDescriptionPtr->getCString();
        CCLOG("map discription = %s", mapDescription);
        std::vector<std::string> descriptionSentences;
        StringUtil::splitString(mapDescription, "\\n", descriptionSentences);

        if (descriptionSentences.empty())
            return;

        for (size_t i = 0; i < descriptionSentences.size(); ++i)
        {
            LabelTTF* lable = LabelTTF::create(descriptionSentences[i].c_str(), "@幼圆", 30.0f);
            RenderTexture* renderTexture = createStroke(lable, 0.7f, Color3B(0, 0, 0));

            float x = EGLView::getInstance()->getFrameSize().width - lable->getContentSize().height - (i == 0 ? 80 : (i + 1) * lable->getContentSize().height + 50);
            float y = EGLView::getInstance()->getFrameSize().height - EGLView::getInstance()->getFrameSize().height / 3;

            lable->setHorizontalAlignment(cocos2d::Label::HAlignment::LEFT);
            lable->setVerticalAlignment(cocos2d::Label::VAlignment::CENTER);
            lable->setPosition(ccp(x, y));
            lable->setRotation(90);

            renderTexture->setPosition(ccp(x, y));
            renderTexture->setRotation(90);
            this->addChild(renderTexture, 1);
            this->addChild(lable, 2);
        }
    
    }
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

   // schedule(schedule_selector(GameMap::repositionSprite));

    //创建一些随机角色
    std::default_random_engine generator;  
    std::uniform_int_distribution<int> r_avatar(20002, 20008);
    std::uniform_int_distribution<int> r_point_x(0, EGLView::getInstance()->getFrameSize().width);
    std::uniform_int_distribution<int> r_point_y(0, EGLView::getInstance()->getFrameSize().height);

    for (int i = 0; i < 100; ++i)
    {
        avatarStyle.body = r_avatar(generator);
        ObjPlayer* random_player = new ObjPlayer(i);
        random_player->init(avatarStyle);
        random_player->setPosition(cocos2d::Point(r_point_x(generator), r_point_y(generator)));
        addCharacter(random_player);
    }

}

/*void GameMap::repositionSprite(float dt)
{
    int zorder = tiledMap_->getMapSize().height - player_->getPositionY() / tiledMap_->getTileSize().height;
    player_->setZOrder(zorder);

    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(player_->getPositionX() - mapX_, player_->getPositionY() - mapY_));
    unsigned int gid = flagLayer_->tileGIDAt(mapPoint);
    cocos2d::Dictionary* dictionary = tiledMap_->getPropertiesForGID(gid);
    if (dictionary == nullptr || dictionary->count() == 0)
    {
        player_->setOpacity(255);
        return;
    }

    const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");
    if (tileFlag == nullptr || tileFlag->uintValue() != TILE_FLAG_COVER)
    {
        player_->setOpacity(255);
        return;
    }

    player_->setOpacity(180);

    //CCLOG("rezorder = %d",zorder);
}*/

uint32 GameMap::getTiledFlagByPosition(const cocos2d::Point& point)
{
    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(player_->getPositionX() - mapX_, player_->getPositionY() - mapY_));
    unsigned int gid = flagLayer_->tileGIDAt(mapPoint);
    cocos2d::Dictionary* dictionary = tiledMap_->getPropertiesForGID(gid);
    if (dictionary == nullptr || dictionary->count() == 0)
    {
        return 0;
    }

    const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");
    if (tileFlag != nullptr)
    {
        return tileFlag->uintValue();
    }
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
                if (tileFlag != nullptr && tileFlag->uintValue() != TILE_FLAG_BLOCK)
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