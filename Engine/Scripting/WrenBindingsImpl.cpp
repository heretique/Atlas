#include "WrenBindingsImpl.h"
#include "math/Vector2.h"
#include <imgui/imgui.h>
#include <wrenpp/Wren++.h>

namespace atlas
{
namespace wren
{
    void begin(WrenVM* vm)
    {
        ImGui::Begin(static_cast<const char*>(wrenGetSlotString(vm, 1)), nullptr, 0);
    }

    void beginWithFlags(WrenVM* vm)
    {
        ImGui::Begin(static_cast<const char*>(wrenGetSlotString(vm, 1)), NULL,
                     *reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 2)));
    }

    void setWindowPos(WrenVM* vm)
    {
        ImGui::SetNextWindowPos(*reinterpret_cast<const ImVec2*>(wrenpp::getSlotForeign<math::Vector2>(vm, 1)));
    }

    void setWindowSize(WrenVM* vm)
    {
        ImGui::SetNextWindowSize(*reinterpret_cast<const ImVec2*>(wrenpp::getSlotForeign<math::Vector2>(vm, 1)));
    }

    void text(WrenVM* vm)
    {
        const char* str = wrenGetSlotString(vm, 1);
        ImGui::Text(str);
    }

    void dummy(WrenVM* vm)
    {
        const ImVec2* v = wrenpp::getSlotForeign<ImVec2>(vm, 1);
        ImGui::Dummy(*v);
    }

    void textColored(WrenVM*)
    {
        // do nothing for now
        // const math::Vector2f* v = (const math::Vector2f*)wrenGetSlotForeign( vm, 1 );
        // const char* t = wrenGetSlotString( vm, 2 );
        // ImGui::TextColored();
    }

    void bulletText(WrenVM* vm)
    {
        ImGui::BulletText(static_cast<const char*>(wrenGetSlotString(vm, 1)));
    }

    void button(WrenVM* vm)
    {
        bool pressed = ImGui::Button(static_cast<const char*>(wrenGetSlotString(vm, 1)));
        wrenSetSlotBool(vm, 0, pressed);
    }

    void checkBox(WrenVM* vm)
    {
        const char* label       = wrenGetSlotString(vm, 1);
        bool        conditional = wrenGetSlotBool(vm, 2);
        ImGui::Checkbox(label, &conditional);
        wrenSetSlotBool(vm, 0, conditional);
    }

    void buttonSized(WrenVM* vm)
    {
        ImGui::Button(static_cast<const char*>(wrenGetSlotString(vm, 1)), *wrenpp::getSlotForeign<ImVec2>(vm, 2));
    }

    void plotNumberArray(WrenVM* vm)
    {
        const std::vector<float>* array = wrenpp::getSlotForeign<std::vector<float> >(vm, 2);
        ImGui::PlotLines(wrenGetSlotString(vm, 1), array->data(), int(wrenGetSlotDouble(vm, 3)), 0, NULL, FLT_MAX,
                         FLT_MAX, ImVec2(140, 80), sizeof(float));
    }

    void plotNumberArrayWithOffset(WrenVM* vm)
    {
        const std::vector<float>* array = wrenpp::getSlotForeign<std::vector<float> >(vm, 2);
        ImGui::PlotLines(wrenGetSlotString(vm, 1), array->data(), int(wrenGetSlotDouble(vm, 3)),
                         int(wrenGetSlotDouble(vm, 4)), NULL, FLT_MAX, FLT_MAX, ImVec2(140, 80), sizeof(float));
    }

    void plotNumberArrayWithOffsetAndSize(WrenVM* vm)
    {
        const std::vector<float>* array = wrenpp::getSlotForeign<std::vector<float> >(vm, 2);
        ImGui::PlotLines(wrenGetSlotString(vm, 1), array->data(), int(wrenGetSlotDouble(vm, 3)),
                         int(wrenGetSlotDouble(vm, 4)), NULL, FLT_MAX, FLT_MAX, *wrenpp::getSlotForeign<ImVec2>(vm, 5),
                         sizeof(float));
    }

    void sliderFloat(WrenVM* vm)
    {
        const char* label = wrenGetSlotString(vm, 1);
        float       value = float(wrenGetSlotDouble(vm, 2));
        float       min   = float(wrenGetSlotDouble(vm, 3));
        float       max   = float(wrenGetSlotDouble(vm, 4));
        ImGui::SliderFloat(label, &value, min, max);
        wrenSetSlotDouble(vm, 0, value);
    }

    void setTitleBar(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        *bits &= ~ImGuiWindowFlags_NoTitleBar;
    }

    void unsetTitleBar(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        (*bits) |= ImGuiWindowFlags_NoTitleBar;
    }

    void setResize(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        *bits &= ~ImGuiWindowFlags_NoResize;
    }

    void setMove(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        *bits &= ~ImGuiWindowFlags_NoMove;
    }

    void unsetMove(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        *bits |= ImGuiWindowFlags_NoMove;
    }

    void unsetResize(WrenVM* vm)
    {
        u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        *bits |= ImGuiWindowFlags_NoResize;
    }

    void setShowBorders(WrenVM*)
    {
        //    u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        //    *bits |= ImGuiWindowFlags_ShowBorders;
    }

    void unsetShowBorders(WrenVM*)
    {
        //    u32* bits = reinterpret_cast<u32*>(wrenpp::getSlotForeign<ImGuiWindowFlag>(vm, 0));
        //    *bits &= ~ImGuiWindowFlags_ShowBorders;
    }
}
}
