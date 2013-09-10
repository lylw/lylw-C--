#include "ObjCharacter.h"
#include "ResourceCachedManager.h"

ObjCharacter::ObjCharacter(const GUID_t& guid, const ObjType& objType/* = ObjType::OBJ_TYPE_INVALID*/)
    : guid_(guid), 
    type_(objType), 
    moveSequenceAction_(nullptr),
    moveAction_(nullptr), 
    walkRepeatAction_(nullptr), 
    moveSpeed_(140.0f),
    isMoving_(false),
    reMoving_(false),
    direction_(DIRECTION_UP),
    lastDirection_(DIRECTION_UP)
{
}

ObjCharacter::~ObjCharacter()
{

}

void ObjCharacter::setGUID(const GUID_t& guid)
{
    guid_ = guid;
}

const AvatarStyle& ObjCharacter::avatarStyle() const
{
    return avatarStyle_;
}

void ObjCharacter::setBodyStyle(const uint16& bodyStyle)
{
    avatarStyle_.body = bodyStyle;
}

void ObjCharacter::setClothesStyle(const uint16& clothesStyle)
{
    avatarStyle_.clothes = clothesStyle;
}

void ObjCharacter::setHairStyle(const uint16& hairStyle)
{
    avatarStyle_.hair = hairStyle;
}

void ObjCharacter::setWeaponStyle(const uint16& weaponStyle)
{
    avatarStyle_.weapon = weaponStyle;
}


void ObjCharacter::moveTo(const cocos2d::Point& target)
{
    //判断是开始走路还是在走路过程中重新发走路指令
    reMoving_ = (isMoving_ == true);

    //计算角度
    float radians = cocos2d::ccpToAngle(cocos2d::ccpSub(target, this->getPosition()));
    float degrees = -1 * CC_RADIANS_TO_DEGREES(radians); //通过宏将弧度变成角度

    CCLOG("radians = %f, degrees = %f", radians, degrees);

    if (degrees <= 22.5 && degrees >= -22.5) 
    {
        //right
        direction_ = DIRECTION_RIGHT;
    }
    else if (degrees > 22.5 && degrees < 67.5)
    {
        //right down 
#if defined(CHARACTER_MODE_EIGHT_DIRECTION)
        direction_ = DIRECTION_RIGHT_DOWN;
#else
        direction_ = DIRECTION_RIGHT;
#endif
    }
    else if (degrees >= 67.5 && degrees <= 112.5)
    {
        //down
        direction_ = DIRECTION_DOWN;
    }
    else if (degrees > 112.5 && degrees < 157.5)
    {
        //left down 
#if defined(CHARACTER_MODE_EIGHT_DIRECTION)
        direction_ = DIRECTION_LEFT_DOWN;
#else
        direction_ = DIRECTION_LEFT;
#endif
    }
    else if (degrees >= 157.5 || degrees <= -157.5)
    {
        //left
        direction_ = DIRECTION_LEFT;
    }
    else if (degrees < -22.5 && degrees > -67.5)
    {
        //right up 
#if defined(CHARACTER_MODE_EIGHT_DIRECTION)
        direction_ = DIRECTION_RIGHT_UP;
#else
        direction_ = DIRECTION_RIGHT;
#endif
    }
    else if (degrees <= -67.5 && degrees >= -112.5)
    {
        //top
        direction_ = DIRECTION_UP;
    }
    else if (degrees < -112.5 && degrees > -157.5)
    {
        //left up 
#if defined(CHARACTER_MODE_EIGHT_DIRECTION)
        direction_ = DIRECTION_LEFT_UP;
#else
        direction_ = DIRECTION_LEFT;
#endif
    }
    else
    {
        direction_ = DIRECTION_UP;
    }

    //如果在重新下走路指令时改变了方向，则改变为不同方向行走动画
    if (reMoving_ == true && (direction_ != lastDirection_))
    {
        CharacterFrameData* characterFrameData = ResourceCachedManager::getInstance().avatarStyleToFrameData(avatarStyle_);
        cocos2d::Animate* moveAnimate = characterFrameData->getAnimateByDirection(direction_);
        //if (walkRepeatAction_ == NULL)
        //{
            walkRepeatAction_ = cocos2d::RepeatForever::create(moveAnimate);
        //    walkRepeatAction_->setTag(ACTION_TYPE_WALKING);
        //}

        lastDirection_ = direction_;
        //walkRepeatAction_->initWithAction(moveAnimate);
        //walkRepeatAction_->retain();
        CCLOG("walkRepeatAction_ RetainCount = %d", walkRepeatAction_->retainCount());
    }

    //取得两点距离
    float distance = target.getDistance(this->getPosition());

    //创建移动行为
    //if (moveAction_ == NULL)
    //{
        moveAction_ = cocos2d::MoveTo::create(distance / moveSpeed_, target);
    //}
    
    //moveAction_->initWithDuration(distance / moveSpeed_, target);
    //moveAction_->retain();
    //CCLOG("moveAction_ RetainCount = %d", moveAction_->retainCount());

    //this->stopAllActions();
    this->stopAction(moveSequenceAction_);
    moveSequenceAction_ = cocos2d::Sequence::create(
        moveAction_, cocos2d::CallFunc::create(this, callfunc_selector(ObjCharacter::moveFinished)), NULL);

    this->runAction(moveSequenceAction_);

    if (reMoving_ == true && (direction_ != lastDirection_))
    {
        this->runAction(walkRepeatAction_);
    }

    isMoving_ = true;
}

void ObjCharacter::moveFinished()
{
    CharacterFrameData* characterFrameData = ResourceCachedManager::getInstance().avatarStyleToFrameData(avatarStyle_);
    cocos2d::SpriteFrame* spriteFrame = characterFrameData->getSpriteFrameByDirection(direction_);
    this->setDisplayFrame(spriteFrame);
    this->stopAction(walkRepeatAction_);

    isMoving_ = false;
    reMoving_ = false;
}