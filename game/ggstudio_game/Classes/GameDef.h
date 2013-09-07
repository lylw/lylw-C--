#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include "Common.h"

//Game Type
typedef uint16 MapID_t;
typedef uint16 CharacterID_t;
typedef uint64 GUID_t;

//����ͼ����4����/8����
enum CharacterDirection
{
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,

    DIRECTION_MAX_FLAG
};

namespace CharacterParameter
{
    static const uint8 kWalkFrameCount = 4;         //��ɫ���߶���֡��
    static const float kWalkFrameDelay = 0.2f;      //��ɫ���߶���ÿ֡�ļ��ʱ�䣨�룩
}

//��ɫ��ʽ
struct AvatarStyle
{
    uint32 body;
    uint32 hair;
    uint32 clothes;
    uint32 weapon;
};

/*
����ʵ������
    �� player    ��ҽ�ɫ
    �� npc       npc
    �� monster   ������ˣ�
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
����������������
    �� standard  ��������
    �� attack    ��������
    �� walk      ���߶���
    �� run       �ܲ�����
    �� action    ���⶯����ͨ�����ڼ��ܶ�����
    �� die       ����
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