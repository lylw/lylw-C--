#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include "Common.h"

//Game Type
typedef uint16 MapID_t;
typedef uint64 GUID_t;

//行走图方向（4方向/8方向）
#define CHARACTER_MODE_EIGHT_DIRECTION
enum CharacterDirection
{
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,

    //是否使用八方向
#if defined (CHARACTER_MODE_EIGHT_DIRECTION)
    DIRECTION_LEFT_DOWN,
    DIRECTION_RIGHT_DOWN,
    DIRECTION_LEFT_UP,
    DIRECTION_RIGHT_UP,
#endif

    DIRECTION_MAX_FLAG
};

namespace CharacterParameter
{
    static const uint8 kWalkFrameCount = 8;         //角色行走动画帧数
    static const float kWalkFrameDelay = 0.1f;      //角色行走动画每帧的间隔时间（秒）
}

//角色样式
struct AvatarStyle
{
    uint16 body;
    uint16 hair;
    uint16 clothes;
    uint16 weapon;
};

/*
对象实体类型
    · player    玩家角色
    · npc       npc
    · monster   怪物（敌人）
*/
enum ObjType
{
    OBJ_TYPE_INVALID = 0,
    OBJ_TYPE_PLAYER  = 1,
    OBJ_TYPE_NPC     = 2,
    OBJ_TYPE_MONSTER = 3,

    OBJ_TYPE_MAX_FLAG
};

/*
基本动作动画类型
    · standard  待机动作
    · attack    攻击动作
    · move      移动动画
    · walk      行走动作
    · run       跑步动作
    · action    特殊动作（通常用于技能动作）
    · die       死亡
*/

enum ActionType
{
    ACTION_TYPE_INVALID  = 0,
    ACTION_TYPE_STANDARD = 1,
    ACTION_TYPE_ATTACK   = 2,
    ACTION_TYPE_MOVING   = 3,
    ACTION_TYPE_WALKING  = 4,
    ACTION_TYPE_RUNNING  = 5,
    ACTION_TYPE_ACTION   = 6,
    ACTION_TYPE_DIE      = 7,

    ACTION_TYPE_MAX_FLAG
};

/*
图块属性类型
    · acrossable 可穿透的地形
    · block      障碍地形
    · cover      遮挡地形
*/
enum TileFlag
{
    TILE_FLAG_ACROSSABLE,   //可通行的
    TILE_FLAG_BLOCK,        //障碍物
    TILE_FLAG_COVER         //遮挡物
};


#endif