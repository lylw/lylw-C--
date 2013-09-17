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

    //���ص�ͼ
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

    //��������ͼ��zorder
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
                    //sprite->setColor(cocos2d::Color3B(100, zorder*2.5, 100));   //��ɫ�������ֵ�ͼͼ��Ĳ��
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
    //��ȡ�õ�ͼ����
    cocos2d::String* mapNamePropertyPtr = tiledMap_->getProperty("_MapName");
    if (mapNamePropertyPtr != nullptr)
    {
        const char* mapName = mapNamePropertyPtr->getCString();
        LabelTTF* lable = LabelTTF::create(mapName, "@��Բ", 30.0f);

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


    //Ȼ��ȡ������
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
            LabelTTF* lable = LabelTTF::create(descriptionSentences[i].c_str(), "@��Բ", 30.0f);
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

    //������ɫ
    player_ = new ObjPlayer(696969);
    player_->init(avatarStyle);
    player_->setPosition(ccp(73, 99));

    //�ѽ�ɫ��������Ӧ�Ĳ���
    //tiledMap_->reorderChild(player_, MapLayer::MAP_LAYER_CHARACTER);
    this->addCharacter(player_);

   // schedule(schedule_selector(GameMap::repositionSprite));

    //����һЩ�����ɫ
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
    //��ȡ��һ��������
    CCTouch* touch = reinterpret_cast<CCTouch *>(pTouches->anyObject());

    //��ȡ��������
    //ע�⣬������cocos2d-3.0 beta�汾��getLocationInView()��û�в����ġ�
    CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

    //�ɴ�������ת����ͼ����
    CCPoint mapPoint = this->tileCoordinateFromPos(ccp(touchPoint.x - mapX_, touchPoint.y - mapY_));

    //�����ǰ���Ӳ�Ϊ��
    if (mapPoint.x != -1)
    {
        //ȡ��ͼ���־��
        CCTMXLayer* flag_layer = tiledMap_->layerNamed("flag_layer");
        if (flag_layer == nullptr)
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
            if (dictionary != nullptr && dictionary->count() != 0)
            {
                const cocos2d::String* tileFlag = dictionary->valueForKey("tile_flag");

                //���û������block���Ի���������block���Ե�������1�����ʾ��ǰͼ���ǿ���ͨ�е�
                if (tileFlag != nullptr && tileFlag->uintValue() != TILE_FLAG_BLOCK)
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

    //Ŀ���Ƿ��ԭ��һ��
    if (startPoint.equals(targetPoint)) return;

    player_->moveTo(targetPoint);
}

void GameMap::objectEnterMap(ObjCharacter* character)
{

} 