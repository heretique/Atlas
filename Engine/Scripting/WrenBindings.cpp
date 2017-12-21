#include "WrenBindings.h"
#include "Core/Engine.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
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
            .endClass()
            .endModule();
    }

    void bindVectorModule()
    {
        Engine::vm()
                .beginModule("atlas/vector")
                .bindClass<math::Vector2, float, float>("Vec2")
                .bindGetter<decltype(math::Vector2::x), &math::Vector2::x>("x")
                .bindSetter<decltype(math::Vector2::x), &math::Vector2::x>("x=(_)")
                .bindGetter<decltype(math::Vector2::y), &math::Vector2::y>("y")
                .bindSetter<decltype(math::Vector2::y), &math::Vector2::y>("y=(_)")
                .bindGetter<decltype(math::Vector2::x), &math::Vector2::x>("i")
                .bindSetter<decltype(math::Vector2::x), &math::Vector2::x>("i=(_)")
                .bindGetter<decltype(math::Vector2::y), &math::Vector2::y>("j")
                .bindSetter<decltype(math::Vector2::y), &math::Vector2::y>("j=(_)")
                .bindMethod<decltype(&math::Vector2::angle), &math::Vector2::angle>(true, "angle(_,_)")
                .bindMethod<void (math::Vector2::*)(const math::Vector2&, const math::Vector2&), &math::Vector2::clamp>(
                    false, "clamp(_,_)")
                .bindMethod<decltype(&math::Vector2::distance), &math::Vector2::distance>(false, "distance(_)")
                .bindMethod<decltype(&math::Vector2::distanceSquared), &math::Vector2::distanceSquared>(
                    false, "distanceSquared(_)")
                .bindMethod<float (math::Vector2::*)(const math::Vector2&) const, &math::Vector2::dot>(false, "dot(_)")
                .bindMethod<float (*)(const math::Vector2&, const math::Vector2&), &math::Vector2::dot>(true,
                                                                                                        "dot(_,_)")
                .bindMethod<decltype(&math::Vector2::length), &math::Vector2::length>(false, "length()")
                .bindMethod<decltype(&math::Vector2::lengthSquared), &math::Vector2::lengthSquared>(false,
                                                                                                    "lengthSquared()")
                .bindMethod<decltype(&math::Vector2::negate), &math::Vector2::negate>(false, "negate()")
                .bindMethod<math::Vector2& (math::Vector2::*)(), &math::Vector2::normalize>(false, "normalize()")
                .bindMethod<void (math::Vector2::*)(float), &math::Vector2::scale>(false, "scale(_)")
                .bindMethod<decltype(&math::Vector2::rotate), &math::Vector2::rotate>(false, "rotate(_,_)")
                .bindMethod<decltype(&math::Vector2::smooth), &math::Vector2::smooth>(false, "smooth(_,_,_)")
                .bindMethod < const math::Vector2 (math::Vector2::*)(const math::Vector2&) const,
            &math::Vector2::operator+>(false, "+(_)")
                    .bindMethod<const math::Vector2 (math::Vector2::*)(const math::Vector2&) const,
                                (&math::Vector2::operator-)>(false, "-(_)")
                    .bindMethod < const math::Vector2 (math::Vector2::*)(float) const,
            &math::Vector2::operator*>(false, "*(_)")
                    .endClass()
                    .bindClass<math::Vector3, float, float, float>("Vec3")
                    .bindGetter<decltype(math::Vector3::x), &math::Vector3::x>("x")
                    .bindSetter<decltype(math::Vector3::x), &math::Vector3::x>("x=(_)")
                    .bindGetter<decltype(math::Vector3::y), &math::Vector3::y>("y")
                    .bindSetter<decltype(math::Vector3::y), &math::Vector3::y>("y=(_)")
                    .bindGetter<decltype(math::Vector3::z), &math::Vector3::z>("z")
                    .bindSetter<decltype(math::Vector3::z), &math::Vector3::z>("z=(_)")
                    .bindGetter<decltype(math::Vector3::x), &math::Vector3::x>("r")
                    .bindSetter<decltype(math::Vector3::x), &math::Vector3::x>("r=(_)")
                    .bindGetter<decltype(math::Vector3::y), &math::Vector3::y>("g")
                    .bindSetter<decltype(math::Vector3::y), &math::Vector3::y>("g=(_)")
                    .bindGetter<decltype(math::Vector3::z), &math::Vector3::z>("b")
                    .bindSetter<decltype(math::Vector3::z), &math::Vector3::z>("b=(_)")
                    .bindMethod<decltype(&math::Vector3::angle), &math::Vector3::angle>(true, "angle(_,_)")
                    .bindMethod<void (math::Vector3::*)(const math::Vector3&, const math::Vector3&),
                                &math::Vector3::clamp>(false, "clamp(_,_)")
                    .bindMethod<void (math::Vector3::*)(const math::Vector3&), &math::Vector3::cross>(false, "cross(_)")
                    .bindMethod<decltype(&math::Vector3::distance), &math::Vector3::distance>(false, "distance(_)")
                    .bindMethod<decltype(&math::Vector3::distanceSquared), &math::Vector3::distanceSquared>(
                        false, "distanceSquared(_)")
                    .bindMethod<float (math::Vector3::*)(const math::Vector3&) const, &math::Vector3::dot>(false,
                                                                                                           "dot(_)")
                    .bindMethod<float (*)(const math::Vector3&, const math::Vector3&), &math::Vector3::dot>(true,
                                                                                                            "dot(_,_)")
                    .bindMethod<decltype(&math::Vector3::length), &math::Vector3::length>(false, "length()")
                    .bindMethod<decltype(&math::Vector3::lengthSquared), &math::Vector3::lengthSquared>(
                        false, "lengthSquared()")
                    .bindMethod<decltype(&math::Vector3::negate), &math::Vector3::negate>(false, "negate()")
                    .bindMethod<math::Vector3& (math::Vector3::*)(), &math::Vector3::normalize>(false, "normalize()")
                    .bindMethod<void (math::Vector3::*)(float), &math::Vector3::scale>(false, "scale(_)")
                    .bindMethod<decltype(&math::Vector3::smooth), &math::Vector3::smooth>(false, "smooth(_,_,_)")
                    .bindMethod < const math::Vector3 (math::Vector3::*)(const math::Vector3&) const,
            &math::Vector3::operator+>(false, "+(_)")
                    .bindMethod<const math::Vector3 (math::Vector3::*)(const math::Vector3&) const,
                                (&math::Vector3::operator-)>(false, "-(_)")
                    .bindMethod < const math::Vector3 (math::Vector3::*)(float) const,
            &math::Vector3::operator*>(false, "*(_)").endClass().endModule();
    }
}
}
