#pragma once

#include "Component.h"
#include "Math/Transform.h"
#include <cereal/cereal.hpp>

namespace atlas
{
class TransformComponent : public Component
{
public:
    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(                                       //
            cereal::base_class<Component>(this),  //
            CEREAL_NVP(_worldTransform),          //
            CEREAL_NVP(_localTransform)           //
            );
    }

    const math::Transform& world() const;
    math::Transform&       world();
    const math::Transform& local() const;
    math::Transform&       local();

private:
    math::Transform _worldTransform;
    math::Transform _localTransform;
};
}  // atlas namespace
