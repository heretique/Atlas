#pragma once

#include "Component.h"
#include "Math/Transform.h"

namespace atlas
{
class TransformComponent : public Component
{
public:
    static ComponentPtr factoryFunc()
    {
        return std::make_unique<TransformComponent>(StringHash(ComponentNames::Transform));
    }

    TransformComponent(ComponentType type);

    const math::Matrix& world() const;
    math::Matrix&       world();
    const math::Matrix& local() const;
    math::Matrix&       local();

private:
    math::Matrix _worldTransform;
    math::Matrix _localTransform;
};

namespace wren
{
    void bindTransformComponent();
}

}  // atlas namespace
