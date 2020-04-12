#pragma once

#include "Common.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{
class TransformComponent : public Component
{
public:
    const hq::math::Mat4x4& world() const;
    hq::math::Mat4x4&       world();
    const hq::math::Mat4x4& local() const;
    hq::math::Mat4x4&       local();
    const hq::math::Box3&   bounds() const;
    hq::math::Box3&         bounds();

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
        SERIALIZE(_worldTransform);
        SERIALIZE(_localTransform);
        SERIALIZE(_bbox);
    }

private:
    hq::math::Mat4x4 _worldTransform {hq::math::Mat4x4::Identity};
    hq::math::Mat4x4 _localTransform {hq::math::Mat4x4::Identity};
    hq::math::Box3 _bbox;
};

}  // atlas namespace
