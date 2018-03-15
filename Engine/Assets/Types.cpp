#include "Types.h"
#include "Core/Engine.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
namespace wren
{
    void invalid(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetHandle>(vm, 0, AssetHandle::invalid);
    }

    void bindAssetTypes()
    {
        Engine::vm()
                .beginModule("scripts/Assets")
                .bindClass<AssetHandle, uint32_t, uint32_t>("AssetHandle")
                .bindCFunction(true, "invalid", &wren::invalid)
                .bindMethod<decltype(&AssetHandle::index), &AssetHandle::index>(false, "index")
                .bindMethod<decltype(&AssetHandle::generation), &AssetHandle::generation>(false, "generation")
                .bindMethod < decltype(&AssetHandle::operator!=),
            &AssetHandle::operator!=>(false, "!=(_)").bindMethod < decltype(&AssetHandle::operator==),
            &AssetHandle::operator==>(false, "==(_)").bindMethod < decltype(&AssetHandle::operator<),
            &AssetHandle::operator<>(false, "<(_)").endClass().endModule();
    }
}  // wren namespace
}  // atlas namespace
