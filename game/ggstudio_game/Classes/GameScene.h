#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <cocos2d.h>
#include "GameType.h"
#include "GamePath.h"
#include "CodePage.hpp"

USING_NS_CC;

enum TileFlag
{
    TILE_FLAG_ACROSSABLE,   //��ͨ�е�
    TILE_FLAG_BLOCK,        //�ϰ���
    TILE_FLAG_COVER         //�ڵ���
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

        //cocos2d::String* str = tiledMap_->getLayer("�¼���")->getProperty("�������");
        //MessageBoxA(0, str->getCString(), "", 0);

        /*CCTMXObjectGroup *objGroup = tiledMap_->objectGroupNamed("events");
        cocos2d::CCDictionary* spawnPoints = objGroup->objectNamed("zp_entry");
        const cocos2d::String* tag = spawnPoints->valueForKey("tag");
        MessageBoxA(0, u2a(tag->getCString()).c_str(), "", 0);*/

    }

    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
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
            //ȡ�ö����
            CCTMXLayer* objects_layer = tiledMap_->layerNamed("flag_layer");

            //���ݵ�ͼ�����ø���ͼ���ID
            unsigned int gid = objects_layer->tileGIDAt(mapPoint);

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