#pragma once
#include "Core/Types.h"

struct WrenVM;

namespace atlas
{
namespace wren
{
    // Imgui
    struct ImGuiWindowFlag
    {
        u32 flag{0u};
    };

    // ImGui functions
    void begin(WrenVM*);
    void beginWithFlags(WrenVM*);
    void setWindowPos(WrenVM*);
    void setWindowSize(WrenVM*);
    void text(WrenVM*);
    void dummy(WrenVM*);
    void textColored(WrenVM*);
    void bulletText(WrenVM*);
    void button(WrenVM*);
    void buttonSized(WrenVM*);
    void checkBox(WrenVM*);
    void plotNumberArray(WrenVM*);
    void plotNumberArrayWithOffset(WrenVM*);
    void plotNumberArrayWithOffsetAndSize(WrenVM*);
    void sliderFloat(WrenVM*);

    // ImGuiWindowFlags functions
    void setTitleBar(WrenVM*);
    void unsetTitleBar(WrenVM*);
    void setResize(WrenVM*);
    void unsetResize(WrenVM*);
    void setMove(WrenVM*);
    void unsetMove(WrenVM*);
    void setShowBorders(WrenVM*);
    void unsetShowBorders(WrenVM*);
}
}
