#pragma once

#include "Core/FreeList.h"
#include "Core/Handle.h"

#include <Core/Types.h>
#include <memory>
#include <string>

namespace atlas {
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
  Template,   // this can be a combination of any other resource types
  UserDefined // This is last always
};

// do not change bitfields size without changing the API handle bitfield size
// these need to be the same
class Asset;
typedef std::shared_ptr<Asset> AssetPtr;
typedef Handle<20, 12> AssetHandle;
typedef PackedFreeList<AssetPtr, AssetHandle, 4096> AssetStorage;
typedef AssetStorage::PackedStorage AssetPackedArray;
static const AssetHandle kInvalidAssetHandle{0, 0};

class Asset {
public:
  Asset(int type, const std::string &name, u32 flags);
  virtual ~Asset();

  virtual bool load(const std::istream &data);
  virtual AssetPtr clone() const;

  AssetHandle handle() const;
  int type() const;
  u32 flags() const;
  const std::string &name() const;
  bool isLoaded() const;

protected:
  int _type;
  std::string _name;
  u32 _flags;
  bool _loaded;
  AssetHandle _handle;

  friend class AssetManager;
};

inline AssetHandle Asset::handle() const { return _handle; }

inline int Asset::type() const { return _type; }

inline u32 Asset::flags() const { return _flags; }

inline const std::string &Asset::name() const { return _name; }

inline bool Asset::isLoaded() const { return _loaded; }

} // namespace atlas
