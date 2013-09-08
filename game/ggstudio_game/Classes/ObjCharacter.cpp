#include "ObjCharacter.h"

ObjCharacter::ObjCharacter(const GUID_t& guid, const ObjType& objType/* = ObjType::OBJ_TYPE_INVALID*/)
    : guid_(guid), type_(objType)
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