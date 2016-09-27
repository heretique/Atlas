#include "WrenBindings.h"
#include "imgui.h"
#include "Wren++.h"


namespace wren {

namespace ig = ImGui;

struct ImGuiWindowFlag {
    std::uint32_t flag{ 0u };
};

void begin(WrenVM* vm)
{
    ig::Begin((const char*)wrenGetSlotString(vm, 1), NULL, 0);
}

void beginWithFlags(WrenVM* vm)
{
    ig::Begin(
                (const char*)wrenGetSlotString(vm, 1),
                NULL,
                *(std::uint32_t*)wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 2)
                );
}

void setWindowPos(WrenVM* vm) {
    ig::SetNextWindowPos(*(const ImVec2*)wrenpp::getSlotForeign<ImVec2>(vm, 1));
}

void setWindowSize(WrenVM* vm) {
    ig::SetNextWindowSize(*(const ImVec2*)wrenpp::getSlotForeign<ImVec2>(vm, 1));
}

void setTitleBar(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits &= ~ImGuiWindowFlags_NoTitleBar;
}

void unsetTitleBar(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    (*bits) |= ImGuiWindowFlags_NoTitleBar;
}

void setResize(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits &= ~ImGuiWindowFlags_NoResize;
}

void setMove(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits &= ~ImGuiWindowFlags_NoMove;
}

void unsetMove(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits |= ImGuiWindowFlags_NoMove;
}

void unsetResize(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits |= ImGuiWindowFlags_NoResize;
}

void setShowBorders(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits |= ImGuiWindowFlags_ShowBorders;
}

void unsetShowBorders(WrenVM* vm) {
    std::uint32_t* bits = reinterpret_cast<std::uint32_t*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
    *bits &= ~ImGuiWindowFlags_ShowBorders;
}

void bindImguiModule()
{

    wrenpp::beginModule("atlas/imgui")
            .bindClass< wren::ImGuiWindowFlag >("ImguiFlag")
            .bindCFunction(false, "setTitleBar()", &wren::setTitleBar)
            .bindCFunction(false, "unsetTitleBar()", &wren::unsetTitleBar)
            .bindCFunction(false, "setResize()", &wren::setResize)
            .bindCFunction(false, "unsetResize()", &wren::unsetResize)
            .bindCFunction(false, "setMove()", &wren::setMove)
            .bindCFunction(false, "unsetMove()", &wren::unsetMove)
            .bindCFunction(false, "setShowBorders()", &wren::setShowBorders)
            .bindCFunction(false, "unsetShowBorders()", &wren::unsetShowBorders)
            .endClass()
            .beginClass("ImGui")
            .bindCFunction(true, "begin(_)", &wren::begin)
            .bindCFunction(true, "begin(_,_)", &wren::beginWithFlags)
            .bindFunction< decltype(&ig::End), &ig::End>(true, "end()")
            .bindCFunction(true, "setNextWindowPos(_)", &wren::setWindowPos)
            .bindCFunction(true, "setNextWindowSize(_)", &wren::setWindowSize)
            .endClass();
}

} // wren
