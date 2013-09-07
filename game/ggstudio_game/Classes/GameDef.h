#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include "Common.h"

//Game Type
typedef uint16 MapID_t;
typedef uint16 CharacterID_t;
typedef uint64 GUID_t;

//角色样式
struct AvatarStyle
{
    uint8 body;
    uint8 hair;
    uint32 clothes;
    uint32 weapon;
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
    ACTION_TYPE_WALK     = 3,
    ACTION_TYPE_RUN      = 4,
    ACTION_TYPE_ACTION   = 5,
    ACTION_TYPE_DIE      = 6,

    ACTION_TYPE_MAX_FLAG
};


#endif