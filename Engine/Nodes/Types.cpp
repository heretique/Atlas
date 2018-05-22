#include "Types.h"

#include "Core/Engine.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
// TODOCM: refactor this abomination
const std::string& NodeTypes::toName(const NodeType type)
{
    if (type == Undefined)
        return NodeTypeNames::Undefined;
    else if (type == Root)
        return NodeTypeNames::Root;
    else if (type == Spatial)
        return NodeTypeNames::Spatial;

    return NodeTypeNames::Undefined;
}

namespace wren
{
    void nodeType_Undefined(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<NodeType>(vm, 0, NodeTypes::Undefined);
    }

    void nodeType_Root(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<NodeType>(vm, 0, NodeTypes::Root);
    }

    void nodeType_Spatial(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<NodeType>(vm, 0, NodeTypes::Spatial);
    }

    void bindNodeTypes()
    {
        Engine::vm()
            .beginModule("scripts/Scene")                                                       //
            .bindClass<NodePtr>("NodePtr")                                                      //
            .bindMethod<decltype(&NodePtr::get), &NodePtr::get>(false, "get")                   //
            .bindMethod<decltype(&NodePtr::use_count), &NodePtr::use_count>(false, "useCount")  //
            .endClass()                                                                         //
            .beginClass("NodeTypes")                                                            //
            .bindCFunction(true, "Undefined", &wren::nodeType_Undefined)                        //
            .bindCFunction(true, "Root", &wren::nodeType_Root)                                  //
            .bindCFunction(true, "Spatial", &wren::nodeType_Spatial)                            //
            .endClass()                                                                         //
            .endModule();
    }
}
}
