#include "ObjCharacter.h"

ObjCharacter::ObjCharacter(const GUID_t& guid, const ObjType& objType)
    : guid_(guid), type_(objType)
{
}

ObjCharacter::~ObjCharacter()
{

}