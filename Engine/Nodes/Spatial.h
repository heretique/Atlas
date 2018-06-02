#pragma once

#include "Node.h"

namespace atlas
{
class SpatialNode : public Node
{
public:
    static NodePtr factoryFunc(const std::string& name)
    {
        return std::make_shared<SpatialNode>(NodeTypes::Spatial, name);
    }

    SpatialNode(NodeType nodeType, const std::string& name);

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(                                  //
            cereal::base_class<Node>(this),  //
            CEREAL_NVP(_bbox),               //
            CEREAL_NVP(_bsphere)             //
            );
    }

private:
    math::BoundingBox    _bbox;
    math::BoundingSphere _bsphere;
};
}
