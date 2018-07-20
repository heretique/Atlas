#include "TransformComponent.h"
#include "Core/Engine.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
TransformComponent::TransformComponent(ComponentType type)
    : Component(type)
{
}

const math::Matrix& TransformComponent::world() const
{
    return _worldTransform;
}

math::Matrix& TransformComponent::world()
{
    return _worldTransform;
}

const math::Matrix& TransformComponent::local() const
{
    return _localTransform;
}

math::Matrix& TransformComponent::local()
{
    return _localTransform;
}

void wren::bindTransformComponent()
{
    Engine::vm()
        .beginModule("main")                                                                                    //
        .bindClass<TransformComponent, ComponentType>("Transform")                                              //
        .bindMethod<math::Matrix& (TransformComponent::*)(), &TransformComponent::world>(false, "worldMatrix")  //
        .bindMethod<math::Matrix& (TransformComponent::*)(), &TransformComponent::local>(false, "localMatrix")  //
        .endClass()                                                                                             //
        .endModule();

    Engine::wrenModule() +=
        "foreign class Transform {\n"
        "    foreign worldMatrix\n"
        "    foreign localMatrix\n"
        "}\n"
        "\n";
}

}  // atlas namespace
