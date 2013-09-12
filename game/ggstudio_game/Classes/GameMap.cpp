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

    //���ص�ͼ
    tiledMap_ = new cocos2d::TMXTiledMap();
    FileUtils::sharedFileUtils()->addSearchPath(GamePath::MAP_DIR.c_str());
    tiledMap_->initWithTMXFile(fullPath.c_str());
    //tiledMap_->setPosition(0, 0);
    Size s = tiledMap_->getContentSize();
    tiledMap_->setPosition(Point(-s.width / 2, 0));
    this->addChild(tiledMap_, 0);

    AvatarStyle avatarStyle;
    avatarStyle.body = 20001;

    //������ɫ
    player_ = new ObjPlayer(696969);
    player_->init(avatarStyle);

    TMXLayer* layer = tiledMap_->getLayer("Trees");
    if (layer != NULL)
    {
        //layer->appendChild(player_);
        player_ = (ObjPlayer*)layer->getTileAt(Point(29,29));
        //layer->addChild(player_, player_->getPositionY());
    }

    //�ѽ�ɫ��������Ӧ�Ĳ���
    //tiledMap_->reorderChild(player_, MapLayer::MAP_LAYER_CHARACTER);
    //tiledMap_->addChild(player_, player_->getPositionY());


    //����һЩ�����ɫ
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
    //��ȡ��һ��������
    CCTouch* touch = reinterpret_cast<CCTouch *>(pTouches->anyObject());


    //��ȡ��������
    //ע�⣬������cocos2d-3.0 beta�汾��getLocationInView()��û�в����ġ�
    CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

#if _DEBUG
    touchMap(touchPoint);
    return;
#endif

    //�ɴ�������ת����ͼ����
    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(touchPoint.x - mapX_, touchPoint.y - mapY_));

    //�����ǰ���Ӳ�Ϊ��
    if (mapPoint.x != -1)
    {
        //ȡ��ͼ���־��
        CCTMXLayer* flag_layer = tiledMap_->layerNamed("flag_layer");
        if (flag_layer == NULL)
        {
            return;
        }

        //���ݵ�ͼ�����ø���ͼ���ID
        unsigned int gid = flag_layer->tileGIDAt(mapPoint);

        //ͼ�鲻Ϊ��
        if (gid != 1)
        {
            //����ͼ��ID��ȡͼ�����������
            cocos2d::Dictionary* dictionary = tiledMap_->getPropertiesForGID(gid);
            if (dictionary != NULL && dictionary->count() != 0)
            {
                const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");

                //���û������block���Ի���������block���Ե�������1�����ʾ��ǰͼ���ǿ���ͨ�е�
                if (tileFlag == NULL || tileFlag->uintValue() == 0)
                {
                    touchMap(touchPoint);
                }
            }
            else
            {
                //���ͼ�鲻�����κ����ԣ���Ĭ�ϵ�������ͨ��
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

    //Ŀ���Ƿ��ԭ��һ��
    if (startPoint.equals(targetPoint)) return;

    player_->moveTo(targetPoint);
}

void GameMap::objectEnterMap(ObjCharacter* character)
{

} 