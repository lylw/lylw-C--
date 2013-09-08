#include "GameDef.h"
#include "GameMap.h"
#include "ResourceCachedManager.h"
#include "ObjPlayer.h"

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
    std::string resource = GamePath::MAP_DIR + MapConfig::getInstance().getMapFileName(10000);
    std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(resource.c_str());

    //���ص�ͼ
    tiledMap_ = new cocos2d::TMXTiledMap();
    FileUtils::sharedFileUtils()->addSearchPath(GamePath::MAP_DIR.c_str());
    tiledMap_->initWithTMXFile(fullPath.c_str());
    tiledMap_->setPosition(0, 0);
    this->addChild(tiledMap_, 0);

    //������ɫ
    player_ = new ObjPlayer(696969);
    player_->setBodyStyle(10001);

    CharacterFrameData* characterFrameData = ResourceCachedManager::getInstance().avatarStyleToFrameData(player_->avatarStyle());

    SpriteFrame* spriteFrame = characterFrameData->getSpriteFrameByDirection(DIRECTION_UP);
    player_->initWithSpriteFrame(spriteFrame);
    player_->setPosition(ccp(265, 148));

    //�������Ϊ�ŵף�֮��ͨ����ɫ�༭���༭ê�㣬��Ϊ������Щ��ɫ��ê�㲢���ڽ��£�
    player_->setAnchorPoint(ccp(0.5, 0));

    //�ѽ�ɫ��������Ӧ�Ĳ���
    tiledMap_->reorderChild(player_, MapLayer::MAP_LAYER_CHARACTER);
    tiledMap_->addChild(player_);

    cocos2d::Animation* animation = characterFrameData->getAnimationByDirection(DIRECTION_UP);
    animate_ = CCAnimate::create(animation);
    repeat_ = RepeatForever::create(animate_);
    ActionInterval* moveTo = MoveTo::create(2.5f, Point(265, 148));
    player_->runAction(moveTo);
    player_->runAction(repeat_);
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
    //��һ�����
    //std::string redPointPath = GamePath::ICONS_DIR + "red_point.jpg";
    //Sprite* sprite = Sprite::create(redPointPath.c_str());
    //sprite->setPosition(touchPoint);
    //tiledMap_->reorderChild(sprite, MapLayer::MAP_LAYER_CHARACTER);
    //tiledMap_->addChild(sprite);

    const Point& startPoint = player_->getPosition();
    const Point& targetPoint = touchPoint;

    //Ŀ���Ƿ��ԭ��һ��
    if (startPoint.equals(targetPoint)) return;

    //����Ƕ�
    float radians = ccpToAngle(ccpSub(targetPoint, startPoint));
    float degrees = -1 * CC_RADIANS_TO_DEGREES(radians); //ͨ���꽫���ȱ�ɽǶ�

    CCLOG("radians = %f, degrees = %f", radians, degrees);

    CharacterDirection direction = DIRECTION_UP;
    AvatarStyle avatarStyle;
    avatarStyle.body = 10001;

    CharacterFrameData* characterFrameData = ResourceCachedManager::getInstance().avatarStyleToFrameData(avatarStyle);
    if (degrees <= 22.5 && degrees >= -22.5) 
    {
        //right
        direction = DIRECTION_RIGHT;
    }
    else if (degrees > 22.5 && degrees < 67.5)
    {
        //bottomright
        direction = DIRECTION_RIGHT;
    }
    else if (degrees >= 67.5 && degrees <= 112.5)
    {
        //bottom
        direction = DIRECTION_DOWN;
    }
    else if (degrees > 112.5 && degrees < 157.5)
    {
        //bottomleft
        direction = DIRECTION_DOWN;
    }
    else if (degrees >= 157.5 || degrees <= -157.5)
    {
        //left
        direction = DIRECTION_LEFT;
    }
    else if (degrees < -22.5 && degrees > -67.5)
    {
        //topright
        direction = DIRECTION_UP;
    }
    else if (degrees <= -67.5 && degrees >= -112.5)
    {
        //top
        direction = DIRECTION_UP;
    }
    else if (degrees < -112.5 && degrees > -157.5)
    {
        //topleft
        direction = DIRECTION_UP;
    }

    cocos2d::Animation* animation = characterFrameData->getAnimationByDirection(direction);
    animate_ = CCAnimate::create(animation);
    repeat_ = RepeatForever::create(animate_);

    //���������
    float distance = targetPoint.getDistance(startPoint);

    moveTo_ = MoveTo::create(distance / 140.0f, targetPoint);
    repeat_ = RepeatForever::create(animate_);

    player_->stopAllActions();
    player_->runAction(moveTo_);
    player_->runAction(repeat_);
}


void GameMap::objectEnterMap(ObjCharacter* character)
{

} 