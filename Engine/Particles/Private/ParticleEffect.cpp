#include "Particles/ParticleEffect.h"

namespace atlas
{

AssetPtr ParticleEffectAsset::factoryFunc(const std::string &filename, u32 flags)
{
    return std::make_shared<ParticleEffectAsset>(filename, flags);
}

ParticleEffectAsset::ParticleEffectAsset(const std::string &filename, u32 flags)
    : Asset(AssetTypes::ParticleEffect, filename, flags)
{

}

bool ParticleEffectAsset::loadImpl(std::istream &data)
{
    return  false;
}

} // atlas namespace
