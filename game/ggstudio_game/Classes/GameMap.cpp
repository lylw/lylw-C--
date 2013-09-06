#include "GameMap.h"

GameMap::GameMap(const MapID_t& mapId)
    : tiledMap_(NULL), mapId_(mapId)
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
    std::string resource = GamePath::MAP_DIR + MapConfig::getInstance().getMapFileName(10000);
    std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(resource.c_str());

    tiledMap_ = new CCTMXTiledMap();
    FileUtils::sharedFileUtils()->addSearchPath(GamePath::MAP_DIR.c_str());
    tiledMap_->initWithTMXFile(fullPath.c_str());
    tiledMap_->setPosition(0, 0);
    this->addChild(tiledMap_, 0);

    // ��ͼƬ�����������浽ȫ�ֵ���������
    std::string hero_path = GamePath::CHARACTER_DIR + "038-Mage06.png";
    CCTexture2D *heroTexture = CCTextureCache::sharedTextureCache()->addImage(hero_path.c_str());

    CCSpriteFrame* character_frames[4];
    cocos2d::Array* animFrames = cocos2d::Array::create();

    const uint32& frameWidth = heroTexture->getContentSize().width / 4;
    const uint32& frameHeight = heroTexture->getContentSize().height / 4;

    for (int i = 0; i < 4; ++i)
    {
        character_frames[i] = 
            CCSpriteFrame::createWithTexture(heroTexture, cocos2d::CCRectMake(frameWidth * i, 0, frameWidth, frameHeight));
        animFrames->addObject(character_frames[i]);
    }

    CCAnimation *animation = new CCAnimation();
    animation->initWithSpriteFrames(animFrames, 0.2f);
    animFrames->release();

    heroSprite = CCSprite::createWithSpriteFrame(character_frames[0]);
    heroSprite->setPosition(ccp(265, 148));
    tiledMap_->reorderChild(heroSprite, MapLayer::MAP_LAYER_CHARACTER);
    tiledMap_->addChild(heroSprite);

    animate_ = CCAnimate::create(animation);

    repeat_ = RepeatForever::create(animate_);
    ActionInterval* moveTo = MoveTo::create(2.5f, Point(265, 148));
    heroSprite->runAction(moveTo);
    heroSprite->runAction(repeat_);
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
                    drawRedPoint(touchPoint);
                }
            }
            else
            {
                //���ͼ�鲻�����κ����ԣ���Ĭ�ϵ�������ͨ��
                drawRedPoint(touchPoint);
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

void GameMap::drawRedPoint(cocos2d::Point touchPoint)
{
    std::string redPointPath = GamePath::ICONS_DIR + "red_point.jpg";
    Sprite* sprite = Sprite::create(redPointPath.c_str());
    sprite->setPosition(touchPoint);
    //tiledMap_->reorderChild(sprite, MapLayer::MAP_LAYER_CHARACTER);
    //tiledMap_->addChild(sprite);

    moveTo_ = MoveTo::create(2.5f, touchPoint);
    repeat_ = RepeatForever::create(animate_);

    heroSprite->stopAllActions();
    heroSprite->runAction(moveTo_);
    heroSprite->runAction(repeat_);
}


void GameMap::objectEnterMap(ObjCharacter* character)
{

} 