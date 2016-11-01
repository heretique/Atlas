#include "Base.h"
#include "AssetManager.h"
#include "Script.h"

namespace atlas {

ScriptAsset::ScriptAsset( const string &name, int flags ) :
    Asset( (int)AssetTypes::Code, name, flags )
{
    initDefault();
}


ScriptAsset::~ScriptAsset()
{
    release();
}

bool ScriptAsset::load(const char *data, int size)
{
    return false;
}


Asset *ScriptAsset::clone()
{
    return nullptr;
}


void ScriptAsset::initDefault()
{

}


void ScriptAsset::release()
{

}

} // atlas
