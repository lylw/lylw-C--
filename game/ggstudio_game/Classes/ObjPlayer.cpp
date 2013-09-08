#include "ObjPlayer.h"

ObjPlayer::ObjPlayer(const GUID_t& guid)
    : ObjCharacter(guid, ObjType::OBJ_TYPE_PLAYER)
{
    
}

ObjPlayer::~ObjPlayer()
{

}