#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include "Common.h"

//Game Type
typedef uint16 MapID_t;
typedef uint64 GUID_t;

//����ͼ����4����/8����
#define CHARACTER_MODE_EIGHT_DIRECTION
enum CharacterDirection
{
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,

    //�Ƿ�ʹ�ð˷���
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
    static const uint8 kWalkFrameCount = 8;         //��ɫ���߶���֡��
    static const float kWalkFrameDelay = 0.1f;      //��ɫ���߶���ÿ֡�ļ��ʱ�䣨�룩
}

//��ɫ��ʽ
struct AvatarStyle
{
    uint16 body;
    uint16 hair;
    uint16 clothes;
    uint16 weapon;
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
    �� move      �ƶ�����
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
    ACTION_TYPE_MOVING   = 3,
    ACTION_TYPE_WALKING  = 4,
    ACTION_TYPE_RUNNING  = 5,
    ACTION_TYPE_ACTION   = 6,
    ACTION_TYPE_DIE      = 7,

    ACTION_TYPE_MAX_FLAG
};

/*
ͼ����������
    �� acrossable �ɴ�͸�ĵ���
    �� block      �ϰ�����
    �� cover      �ڵ�����
*/
enum TileFlag
{
    TILE_FLAG_ACROSSABLE,   //��ͨ�е�
    TILE_FLAG_BLOCK,        //�ϰ���
    TILE_FLAG_COVER         //�ڵ���
};


#endif