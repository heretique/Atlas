#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Base.h"
#include "AtlasCore.h"

namespace atlas {

class Asset;

// do not change bitfields size without changing the API handle bitfield size
// these need to be the same
typedef HandledPackedStorage<Asset*, 20, 12> AssetStorage;
typedef AssetStorage::Handle AssetHandle;
typedef AssetStorage::PackedStorage AssetPackedArray;
static AssetHandle INVALID_ASSET_HANDLE{0,0};

enum class AssetTypes {
    Undefined = 0,
    Scene,
    Geometry,
    Animation,
    Material,
    Code,
    Shader,
    Texture,
    ParticleEffect,
    Pipeline,
    Template, // this can be a combination of any other resource types
    UserDefined // This is last always
};

class Asset
{
public:
    Asset(int type, const string& name, int flags);
    virtual ~Asset();
    virtual Asset *clone();

    virtual void initDefault();
    virtual void release();
    virtual bool load( const istream& data);
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
    AssetHandle getHandle() { return _handle; }
    bool isLoaded() { return _loaded; }
    void addRef() { ++_refCount; }
    void subRef() { --_refCount; }

protected:
    int                 _type;
    std::string         _name;
    AssetHandle         _handle;
    int                 _flags;
    u32                 _refCount;  // Number of handles created by user
    bool                _loaded;

    friend class AssetManager;
};

//
// Resource Manager
//

typedef std::function<void (Asset*)> AssetTypeReleaseFunc;
typedef std::function<Asset* (const std::string &name, int flags)> AssetTypeFactoryFunc;

struct AssetRegEntry
{
    std::string             typeString;
    AssetTypeFactoryFunc    factoryFunc;  // Factory to create asset object
    AssetTypeReleaseFunc    releaseFunc;  // Called to destroy asset
};

// ========================

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void registerAssetType(int type, const std::string &typeString, AssetTypeFactoryFunc ff,
                           AssetTypeReleaseFunc rf);

    AssetHandle addAsset(int type, const std::string &name, int flags);
    uint removeAsset(AssetHandle handle);
    Asset *assetFromHandle(AssetHandle handle);
    Asset *getNextAsset(int type, AssetHandle start);
    Asset *findAsset(int type, const std::string &name);
    bool loadAssets();
    void loadAssetsAsync();
    void setAssetsDir(const string& path);
    void releaseUnusedAssets();
    int unusedAssets();

protected:
    AssetHandle addAsset(Asset *resource);
private:
    AssetStorage _assets;
    map<int, AssetRegEntry> _registry;
    string _assetDir;
};

} // atlas



#endif // ASSETMANAGER_H
