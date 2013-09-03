#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <cocos2d.h>
#include "GameType.h"
#include "GamePath.h"
#include "CodePage.hpp"

USING_NS_CC;

enum TileFlag
{
    TILE_FLAG_ACROSSABLE,   //可通行的
    TILE_FLAG_BLOCK,        //障碍物
    TILE_FLAG_COVER         //遮挡物
};

//图层
enum MapLayer
{
    MAP_LAYER_UNDERWORLD = -1,  //地底
    MAP_LAYER_GROUND     = 0,   //地表层
    MAP_LAYER_CHARACTER  = 1,   //角色层（包括NPC，怪物等）
    MAP_LAYER_OBJECTS    = 2,   //地图对象层
    MAP_LAYER_TILED_FLAG = 3    //图块标志层
};

class ObjCharacter;

class GameScene 
    : public cocos2d::Layer
{
public:
    GameScene(const SceneType& type)
        : tiledMap_(NULL), sceneType_(type)
    {
    }

    virtual ~GameScene()
    {
        tiledMap_->cleanup();
        SAFE_DELETE(tiledMap_);
    }

    bool init(void)
    {
        mapX_ = 0;
        mapY_ = 0;
        this->setTouchEnabled(true);
        onLoadCompleted();

        return true;
    }

    void onLoadCompleted()
    {
        std::string resource = GamePath::MAP_DIR + "standard.tmx";
        std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(resource.c_str());

        tiledMap_ = new CCTMXTiledMap();
        FileUtils::sharedFileUtils()->addSearchPath(GamePath::MAP_DIR.c_str());
        tiledMap_->initWithTMXFile(fullPath.c_str());
        tiledMap_->setPosition(0, 0);
        this->addChild(tiledMap_, 0);

        // 将图片生成纹理，保存到全局的纹理缓冲区
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
        animation->initWithSpriteFrames(animFrames, 0.1f);
        animFrames->release();

        CCSprite *heroSprite = CCSprite::createWithSpriteFrame(character_frames[0]);
        heroSprite->setPosition(ccp(500, 500));
        this->addChild(heroSprite);

        CCAnimate *animate = CCAnimate::create(animation);

        ActionInterval* repeat = RepeatForever::create(animate);
        ActionInterval* moveTo = MoveTo::create(2.5f, Point(500, 100));
        FiniteTimeAction* action = Spawn::create(animate, moveTo, NULL);
        heroSprite->runAction(CCSequence::create(action, NULL));
    }

    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
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
                        drawRedPoint(touchPoint);
                    }
                }
                else
                {
                    //如果图块不设置任何属性，则默认当作可以通行
                    drawRedPoint(touchPoint);
                }
            }
        }
    }

    CCPoint tileCoordinateFromPos(CCPoint pos)
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

    void drawRedPoint(cocos2d::Point touchPoint)
    {
        std::string redPointPath = GamePath::MAP_DIR + "red_point.jpg";
        Sprite* sprite = Sprite::create(redPointPath.c_str());
        sprite->setPosition(touchPoint);
        tiledMap_->reorderChild(sprite, MapLayer::MAP_LAYER_CHARACTER);
        tiledMap_->addChild(sprite);
    }

public:
    void objectEnterScene(ObjCharacter* character)
    {
    
    }

private:
    SceneType sceneType_;
    CCTMXTiledMap* tiledMap_;
    float mapX_, mapY_;
};

#endif