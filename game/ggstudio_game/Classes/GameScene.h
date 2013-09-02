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

class ObjCharacter;

class GameScene 
    : public cocos2d::Layer
{
public:
    GameScene(const SceneType& type)
        : sceneType_(type)
    {
    }

    virtual ~GameScene()
    {
        tiledMap_->cleanup();
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
        tiledMap_ = CCTMXTiledMap::create(resource.c_str());
        tiledMap_->setPosition(0,0);
        this->addChild(tiledMap_, 1);

        //cocos2d::String* str = tiledMap_->getLayer("事件层")->getProperty("帐篷入口");
        //MessageBoxA(0, str->getCString(), "", 0);

        /*CCTMXObjectGroup *objGroup = tiledMap_->objectGroupNamed("events");
        cocos2d::CCDictionary* spawnPoints = objGroup->objectNamed("zp_entry");
        const cocos2d::String* tag = spawnPoints->valueForKey("tag");
        MessageBoxA(0, u2a(tag->getCString()).c_str(), "", 0);*/

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
            //取得对象层
            CCTMXLayer* objects_layer = tiledMap_->layerNamed("flag_layer");

            //根据地图坐标获得格子图块的ID
            unsigned int gid = objects_layer->tileGIDAt(mapPoint);

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
        tiledMap_->reorderChild(sprite, 1);
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