#include "WrenBindings.h"
#include "Core/Engine.h"
#include "WrenBindingsImpl.h"

#include <imgui/imgui.h>
#include <wrenpp/Wren++.h>

namespace atlas
{
namespace wren
{
    namespace ig = ImGui;

    void bindImguiModule()
    {
        Engine::vm()
            .beginModule("atlas/imgui")
            .bindClass<wren::ImGuiWindowFlag>("ImguiFlag")
            .bindCFunction(false, "setTitleBar()", &wren::setTitleBar)
            .bindCFunction(false, "unsetTitleBar()", &wren::unsetTitleBar)
            .bindCFunction(false, "setResize()", &wren::setResize)
            .bindCFunction(false, "unsetResize()", &wren::unsetResize)
            .bindCFunction(false, "setMove()", &wren::setMove)
            .bindCFunction(false, "unsetMove()", &wren::unsetMove)
            .bindCFunction(false, "setShowBorders()", &wren::setShowBorders)
            .bindCFunction(false, "unsetShowBorders()", &wren::unsetShowBorders)
            .endClass()
            .beginClass("Imgui")
            // windows & their formatting
            .bindCFunction(true, "begin(_)", &wren::begin)
            .bindCFunction(true, "begin(_,_)", &wren::beginWithFlags)
            .bindFunction<decltype(&ig::End), &ig::End>(true, "end()")
            .bindCFunction(true, "setNextWindowPos(_)", &wren::setWindowPos)
            .bindCFunction(true, "setNextWindowSize(_)", &wren::setWindowSize)
            .bindFunction<decltype(static_cast<bool (*)(const char*)>(&ig::TreeNode)), &ig::TreeNode>(true,
                                                                                                      "treeNode(_)")
            .bindFunction<decltype(&ig::Spacing), &ig::Spacing>(true, "spacing()")
            .bindFunction<decltype(&ig::Indent), &ig::Indent>(true, "indent()")
            .bindFunction<decltype(&ig::Unindent), &ig::Unindent>(true, "unindent()")
            .bindFunction<decltype(&ig::TreePop), &ig::TreePop>(true, "treePop()")
            .bindCFunction(true, "text(_)", &wren::text)
            .bindCFunction(true, "textColored(_,_)", &wren::textColored)
            .bindCFunction(true, "dummy(_)", &wren::dummy)
            // elements
            .bindCFunction(true, "bulletText(_)", &wren::bulletText)
            .bindFunction<decltype(&ig::Bullet), &ig::Bullet>(true, "bullet()")
            .bindCFunction(true, "button(_,_)", &wren::buttonSized)
            .bindCFunction(true, "button(_)", &wren::button)
            .bindFunction<decltype(static_cast<bool (*)(const char*, bool)>(&ig::RadioButton)), &ig::RadioButton>(
                true, "radioButton(_,_)")
            .bindCFunction(true, "checkBox(_,_)", &wren::checkBox)
            .bindCFunction(true, "plotArray(_,_,_)", &wren::plotNumberArray)
            .bindCFunction(true, "plotArray(_,_,_,_)", &wren::plotNumberArrayWithOffset)
            .bindCFunction(true, "plotArray(_,_,_,_,_)", &wren::plotNumberArrayWithOffsetAndSize)
            .bindCFunction(true, "sliderFloat(_,_,_,_)", &wren::sliderFloat)
            .endClass();
    }
}
}
