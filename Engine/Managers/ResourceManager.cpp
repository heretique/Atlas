#include "Base.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "LogManager.h"

namespace atlas {


//
// Resource
//
Resource::Resource( int type, const std::string &name, int flags )
{
    _type = type;
    _name = name;
    _handle = {0, 0};
    _loaded = false;
    _refCount = 0;
    _userRefCount = 0;
    _flags = flags;
}

Resource::~Resource()
{
    // Remove all references
    // Nothing to do here
}


Resource *Resource::clone()
{
    Engine::log().writeInfo( "Resource cloning not implemented for type %i", _type );
    return 0x0;
}


void Resource::initDefault()
{
}


void Resource::release()
{
}


bool Resource::load( const char *data, int size )
{
    // Resources can only be loaded once
    if( _loaded ) return false;

    // A NULL pointer can be used if the file could not be loaded
    if( data == 0x0 || size <= 0 )
    {
        Engine::log().writeWarning( "Resource '%s' of type %i: No data loaded (file not found?)", _name.c_str(), _type );
        return false;
    }

    _loaded = true;

    return true;
}


void Resource::unload()
{
    release();
    initDefault();
    _loaded = false;
}

float Resource::getElemParamF( int elem, int elemIdx, int param, int compIdx )
{
    Engine::log().writeError( "Invalid elem, param or component in atlasGetResParamF" );
    return std::numeric_limits<float>::quiet_NaN();
}

void Resource::setElemParamF( int elem, int elemIdx, int param, int compIdx, float value )
{
    Engine::log().writeError( "Invalid elem, param or component in atlasSetResParamF" );
}

const char *Resource::getElemParamStr( int elem, int elemIdx, int param )
{
    Engine::log().writeError( "Invalid elem or param in atlasGetResParamStr" );
    return "";
}

void Resource::setElemParamStr( int elem, int elemIdx, int param, const char *value )
{
    Engine::log().writeError( "Invalid elem or param in atlasSetResParamStr" );
}

void *Resource::mapStream( int elem, int elemIdx, int stream, bool read, bool write )
{
    Engine::log().writeError( "Invalid operation in atlasMapResStream" );
    return 0x0;
}

void Resource::unmapStream()
{
    Engine::log().writeError( "Invalid operation by atlasUnmapResStream" );
}


//
// ResourceManager
//



ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}


void ResourceManager::registerResType(int resType, const std::string &typeString, ResTypeInitializationFunc inf,
                                       ResTypeReleaseFunc rf, ResTypeFactoryFunc ff)
{
    ResourceRegEntry entry;
    entry.typeString = typeString;
    entry.initializationFunc = inf;
    entry.releaseFunc = rf;
    entry.factoryFunc = ff;
    _registry[resType] = entry;

    // Initialize resource type
    if(inf) inf();
}

ResHandle ResourceManager::addResource(Resource *resource)
{
    return _resources.add(resource);
}

ResHandle ResourceManager::addResource(int type, const std::string &name, int flags)
{
    if( name == "" )
    {
        Engine::log().writeInfo("Invalid name for added resource of type %i", type);
        return {0,0};
    }


    // Create resource
    Resource *resource = 0x0;
    auto it = _registry.find(type);
    if (it != _registry.end()) resource = it->second.factoryFunc(name, flags);
    if( resource == 0x0 ) return {0,0};

    Engine::log().writeInfo("Adding %s resource '%s'", it->second.typeString.c_str(), name.c_str());

    return addResource(resource);
}

Resource *ResourceManager::resFromHandle(ResHandle handle)
{
    return *_resources.get(handle);
}

Resource *ResourceManager::getNextResource(int type, ResHandle start)
{
    Resource *res = nullptr;
    const ResPackedArray &packedRes = _resources.storage();
    u32 startIndex = _resources.getPackedIndex(start);
    if (startIndex == INVALID_PACKED_INDEX)
    {
        for (auto i = startIndex + 1; i < packedRes.count; ++i)
        {
            res = packedRes.array[i];
            if (res != nullptr && (type == (int)ResourceTypes::Undefined || res->_type == type))
            {
                return res;
            }
        }
    }

    return res;
}

} // atlas

