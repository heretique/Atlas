#pragma once

#include "Math/Transform.h"

namespace atlas
{
class TransformComponent
{
public:
    const math::Matrix& world() const;
    math::Matrix&       world();
    const math::Matrix& local() const;
    math::Matrix&       local();

private:
    math::Matrix _worldTransform;
    math::Matrix _localTransform;
};

}  // atlas namespace
