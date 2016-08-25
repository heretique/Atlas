#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <functional>
#include <vector>
#include <map>

#include "Types.h"
#include "AtlasCore.h"

namespace atlas {

class Resource;

// do not change bitfields size without changing the API handle bitfield size
// these need to be the same
typedef HandledPackedStorage<Resource*, 20, 12> ResStorage;
typedef ResStorage::Handle ResHandle;
typedef ResStorage::PackedStorage ResPackedArray;

enum class ResourceTypes {
    Undefined = 0,
    SceneGraph,
    Geometry,
    Animation,
    Material,
    Code,
    Shader,
    Texture,
    ParticleEffect,
    Pipeline
};

class Resource
{
public:
    Resource(int type, const std::string &name, int flags);
    virtual ~Resource();
    virtual Resource *clone();

    virtual void initDefault();
    virtual void release();
    virtual bool load( const char *data, int size );
    void unload();

    int findElem(int elem, int param, const char *value);
    virtual int getElemCount( int elem );
    virtual int getElemParamI( int elem, int elemIdx, int param );
    virtual void setElemParamI( int elem, int elemIdx, int param, int value );
    virtual float getElemParamF( int elem, int elemIdx, int param, int compIdx );
    virtual void setElemParamF( int elem, int elemIdx, int param, int compIdx, float value );
    virtual const char *getElemParamStr( int elem, int elemIdx, int param );
    virtual void setElemParamStr( int elem, int elemIdx, int param, const char *value );
    virtual void *mapStream( int elem, int elemIdx, int stream, bool read, bool write );
    virtual void unmapStream();

    int &getType() { return _type; }
    int getFlags() { return _flags; }
    const std::string &getName() { return _name; }
    ResHandle getHandle() { return _handle; }
    bool isLoaded() { return _loaded; }

protected:
    int                 _type;
    std::string         _name;
    ResHandle           _handle;
    int                 _flags;

    u32                 _refCount;  // Number of other objects referencing this resource
    u32                 _userRefCount;  // Number of handles created by user

    bool                _loaded;

    friend class ResourceManager;
};

//
// Resource Manager
//

typedef std::function<void ()> ResTypeInitializationFunc;
typedef std::function<void ()> ResTypeReleaseFunc;
typedef std::function<Resource* (const std::string &name, int flags)> ResTypeFactoryFunc;

struct ResourceRegEntry
{
    std::string                typeString;
    ResTypeInitializationFunc  initializationFunc;  // Called when type is registered
    ResTypeReleaseFunc         releaseFunc;  // Called when type is unregistered
    ResTypeFactoryFunc         factoryFunc;  // Factory to create resource object
};

// ========================

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void registerResType(int type, const std::string &typeString, ResTypeInitializationFunc inf,
                         ResTypeReleaseFunc rf, ResTypeFactoryFunc ff);

    ResHandle addResource(int type, const std::string &name, int flags);
    Resource *resFromHandle(ResHandle handle);
    Resource *getNextResource(int type, ResHandle start);
    Resource *findResource(int type, const std::string &name);

protected:
    ResHandle addResource(Resource *resource);
private:
    ResStorage _resources;
    std::map<int, ResourceRegEntry> _registry;
};

} // atlas



#endif // RESOURCEMANAGER_H
