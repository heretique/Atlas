#pragma once

#include "Assets/Types.h"
#include "Component.h"
#include <cereal/cereal.hpp>

namespace atlas
{
class MaterialComponent : public Component
{
public:
    static ComponentPtr factoryFunc()
    {
        return std::make_unique<MaterialComponent>(StringHash(ComponentNames::Material));
    }

    MaterialComponent(ComponentType type);

    void setMaterial(AssetPtr material);
    void clearMaterial();

private:
    AssetPtr _material;
};

}  // namespace Atlas
