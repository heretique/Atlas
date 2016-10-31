#include "Base.h"
#include "ResourceManager.h"
#include "Script.h"

namespace atlas {

void ScriptResource::initializationFunc()
{

}


void ScriptResource::releaseFunc()
{

}


ScriptResource::ScriptResource( const string &name, int flags ) :
    Resource( (int)ResourceTypes::Geometry, name, flags )
{
    initDefault();
}


ScriptResource::~ScriptResource()
{
    release();
}

bool ScriptResource::load(const char *data, int size)
{
    return false;
}


Resource *ScriptResource::clone()
{
    return nullptr;
}


void ScriptResource::initDefault()
{

}


void ScriptResource::release()
{

}

} // atlas
