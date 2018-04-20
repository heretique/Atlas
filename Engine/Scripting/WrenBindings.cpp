#include "WrenBindings.h"

#include "Assets/Types.h"
#include "Core/Engine.h"
#include "Core/StringHash.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
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

    void bindImgui()
    {
        Engine::vm()
            .beginModule("scripts/ImGui")
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
            .bindCFunction(true, "getWindowSize()", &wren::getWindowSize)
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

    void bindMath()
    {
        Engine::vm()
                .beginModule("scripts/Math")                                                             // Module Math
                .bindClass<math::Vector2, float, float>("Vec2")                                          // Class Vec2
                .bindGetter<decltype(math::Vector2::x), &math::Vector2::x>("x")                          //
                .bindSetter<decltype(math::Vector2::x), &math::Vector2::x>("x=(_)")                      //
                .bindGetter<decltype(math::Vector2::y), &math::Vector2::y>("y")                          //
                .bindSetter<decltype(math::Vector2::y), &math::Vector2::y>("y=(_)")                      //
                .bindGetter<decltype(math::Vector2::x), &math::Vector2::x>("i")                          //
                .bindSetter<decltype(math::Vector2::x), &math::Vector2::x>("i=(_)")                      //
                .bindGetter<decltype(math::Vector2::y), &math::Vector2::y>("j")                          //
                .bindSetter<decltype(math::Vector2::y), &math::Vector2::y>("j=(_)")                      //
                .bindMethod<decltype(&math::Vector2::angle), &math::Vector2::angle>(true, "angle(_,_)")  //
                .bindMethod<void (math::Vector2::*)(const math::Vector2&, const math::Vector2&), &math::Vector2::clamp>(
                    false, "clamp(_,_)")  //
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
            &math::Vector2::operator*>(false, "*(_)")                       //
                    .endClass()                                             //
                    .bindClass<math::Vector3, float, float, float>("Vec3")  // Class Vec3
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
            &math::Vector3::operator*>(false, "*(_)")
                    .endClass()                                                    //
                    .bindClass<math::Vector4, float, float, float, float>("Vec4")  // Class Vec4
                    .bindGetter<decltype(math::Vector4::x), &math::Vector4::x>("x")
                    .bindSetter<decltype(math::Vector4::x), &math::Vector4::x>("x=(_)")
                    .bindGetter<decltype(math::Vector4::y), &math::Vector4::y>("y")
                    .bindSetter<decltype(math::Vector4::y), &math::Vector4::y>("y=(_)")
                    .bindGetter<decltype(math::Vector4::z), &math::Vector4::z>("z")
                    .bindSetter<decltype(math::Vector4::z), &math::Vector4::z>("z=(_)")
                    .bindGetter<decltype(math::Vector4::z), &math::Vector4::z>("w")
                    .bindSetter<decltype(math::Vector4::z), &math::Vector4::z>("w=(_)")
                    .bindGetter<decltype(math::Vector4::x), &math::Vector4::x>("r")
                    .bindSetter<decltype(math::Vector4::x), &math::Vector4::x>("r=(_)")
                    .bindGetter<decltype(math::Vector4::y), &math::Vector4::y>("g")
                    .bindSetter<decltype(math::Vector4::y), &math::Vector4::y>("g=(_)")
                    .bindGetter<decltype(math::Vector4::z), &math::Vector4::z>("b")
                    .bindSetter<decltype(math::Vector4::z), &math::Vector4::z>("b=(_)")
                    .bindGetter<decltype(math::Vector4::z), &math::Vector4::z>("a")
                    .bindSetter<decltype(math::Vector4::z), &math::Vector4::z>("a=(_)")
                    .bindMethod<decltype(&math::Vector4::angle), &math::Vector4::angle>(true, "angle(_,_)")
                    .bindMethod<void (math::Vector4::*)(const math::Vector4&, const math::Vector4&),
                                &math::Vector4::clamp>(false, "clamp(_,_)")
                    .bindMethod<decltype(&math::Vector4::distance), &math::Vector4::distance>(false, "distance(_)")
                    .bindMethod<decltype(&math::Vector4::distanceSquared), &math::Vector4::distanceSquared>(
                        false, "distanceSquared(_)")
                    .bindMethod<float (math::Vector4::*)(const math::Vector4&) const, &math::Vector4::dot>(false,
                                                                                                           "dot(_)")
                    .bindMethod<float (*)(const math::Vector4&, const math::Vector4&), &math::Vector4::dot>(true,
                                                                                                            "dot(_,_)")
                    .bindMethod<decltype(&math::Vector4::length), &math::Vector4::length>(false, "length()")
                    .bindMethod<decltype(&math::Vector4::lengthSquared), &math::Vector4::lengthSquared>(
                        false, "lengthSquared()")
                    .bindMethod<decltype(&math::Vector4::negate), &math::Vector4::negate>(false, "negate()")
                    .bindMethod<math::Vector4& (math::Vector4::*)(), &math::Vector4::normalize>(false, "normalize()")
                    .bindMethod<void (math::Vector4::*)(float), &math::Vector4::scale>(false, "scale(_)")
                    .bindMethod < const math::Vector4 (math::Vector4::*)(const math::Vector4&) const,
            &math::Vector4::operator+>(false, "+(_)")
                    .bindMethod<const math::Vector4 (math::Vector4::*)(const math::Vector4&) const,
                                (&math::Vector4::operator-)>(false, "-(_)")
                    .bindMethod < const math::Vector4 (math::Vector4::*)(float) const,
            &math::Vector4::operator*>(false, "*(_)")                                          //
                    .endClass()                                                                //
                    .bindClass<math::Quaternion>("Quaternion")                                 // Class Quaternion
                    .bindGetter<decltype(math::Quaternion::x), &math::Quaternion::x>("x")      //
                    .bindSetter<decltype(math::Quaternion::x), &math::Quaternion::x>("x=(_)")  //
                    .bindGetter<decltype(math::Quaternion::y), &math::Quaternion::y>("y")      //
                    .bindSetter<decltype(math::Quaternion::y), &math::Quaternion::y>("y=(_)")  //
                    .bindGetter<decltype(math::Quaternion::z), &math::Quaternion::z>("z")      //
                    .bindSetter<decltype(math::Quaternion::z), &math::Quaternion::z>("z=(_)")  //
                    .bindGetter<decltype(math::Quaternion::z), &math::Quaternion::z>("w")      //
                    .bindSetter<decltype(math::Quaternion::z), &math::Quaternion::z>("w=(_)")  //
                    .bindMethod<decltype(&math::Quaternion::identity), &math::Quaternion::identity>(true,
                                                                                                    "identity")  //
                    .bindMethod<decltype(&math::Quaternion::zero), &math::Quaternion::zero>(true, "zero")        //
                    .bindMethod<decltype(&math::Quaternion::setIdentity), &math::Quaternion::setIdentity>(
                        false, "setIdentity()")  //
                    .bindMethod<void (math::Quaternion::*)(float, float, float, float), &math::Quaternion::set>(
                        false, "setFromValues(_,_,_,_)")  //
                    .bindMethod<void (math::Quaternion::*)(const math::Matrix&), &math::Quaternion::set>(
                        false, "setFromMatrix(_)")  //
                    .bindMethod<void (math::Quaternion::*)(const math::Vector3&, float), &math::Quaternion::set>(
                        false, "setFromAxisAngle(_,_)")  //
                    .bindMethod<void (math::Quaternion::*)(const math::Quaternion&), &math::Quaternion::set>(
                        false, "setFromQuaternion(_)")  //                                            //
                    .bindMethod<decltype(&math::Quaternion::lerp), &math::Quaternion::lerp>(true, "lerp(_,_,_,_)")  //
                    .bindMethod<void (*)(const math::Quaternion&, const math::Quaternion&, float, math::Quaternion*),
                                &math::Quaternion::slerp>(true, "slerp(_,_,_,_)")  //
                    .bindMethod < decltype(&math::Quaternion::operator*),
            &math::Quaternion::operator*>(false, "*(_)")  //
                 .endClass()                              //
                 .bindClass<math::Matrix>("Matrix")       // class Matrix
                 .bindMethod<decltype(&math::Matrix::identity), &math::Matrix::identity>(true,
                                                                                         "identity")  //
                 .bindMethod<decltype(&math::Matrix::zero), &math::Matrix::zero>(true, "zero")        //
                 .bindMethod<decltype(&math::Matrix::setIdentity), &math::Matrix::setIdentity>(false,
                                                                                               "setIdentity()")  //
                 .bindMethod<decltype(&math::Matrix::setZero), &math::Matrix::setZero>(false, "setZero()")       //
                 .endClass()                                                                                     //
                 .bindClass<math::Transform>("Transform")  // class Transform
                 .bindMethod<decltype(&math::Transform::getMatrix), &math::Transform::getMatrix>(false, "matrix")  //
                 .bindMethod<const math::Vector3& (math::Transform::*)() const, &math::Transform::getScale>(false,
                                                                                                            "scale")  //
                 .bindMethod<decltype(&math::Transform::getScaleX), &math::Transform::getScaleX>(false, "scaleX")     //
                 .bindMethod<decltype(&math::Transform::getScaleY), &math::Transform::getScaleY>(false, "scaleY")     //
                 .bindMethod<decltype(&math::Transform::getScaleZ), &math::Transform::getScaleZ>(false, "scaleZ")     //
                 .bindMethod<const math::Quaternion& (math::Transform::*)() const, &math::Transform::getRotation>(
                     false, "rotation")  //
                 .bindMethod<const math::Vector3& (math::Transform::*)() const, &math::Transform::getTranslation>(
                     false, "translation")  //
                 .bindMethod<decltype(&math::Transform::getTranslationX), &math::Transform::getTranslationX>(
                     false, "translationX")  //
                 .bindMethod<decltype(&math::Transform::getTranslationY), &math::Transform::getTranslationY>(
                     false, "translationY")  //
                 .bindMethod<decltype(&math::Transform::getTranslationZ), &math::Transform::getTranslationZ>(
                     false, "translationZ")  //
                 .bindMethod<math::Vector3 (math::Transform::*)() const, &math::Transform::getForwardVector>(
                     false, "forward")                                                                                //
                 .bindMethod<math::Vector3 (math::Transform::*)() const, &math::Transform::getUpVector>(false, "up")  //
                 .bindMethod<math::Vector3 (math::Transform::*)() const, &math::Transform::getRightVector>(false,
                                                                                                           "right")  //
                 .bindMethod<void (math::Transform::*)(float, float, float, float), &math::Transform::rotate>(
                     false, "rotateWithQuaternionFloats(_,_,_,_)")  //
                 .bindMethod<void (math::Transform::*)(const math::Quaternion&), &math::Transform::rotate>(
                     false, "rotateWithQuaternion(_)")  //
                 .bindMethod<void (math::Transform::*)(const math::Vector3&, float), &math::Transform::rotate>(
                     false, "rotateWithAxisAngle(_,_)")  //
                 .bindMethod<void (math::Transform::*)(const math::Matrix&), &math::Transform::rotate>(
                     false, "rotateWithMatrix(_)")                                                                 //
                 .bindMethod<decltype(&math::Transform::rotateX), &math::Transform::rotateX>(false, "rotateX(_)")  //
                 .bindMethod<decltype(&math::Transform::rotateY), &math::Transform::rotateY>(false, "rotateY(_)")  //
                 .bindMethod<decltype(&math::Transform::rotateZ), &math::Transform::rotateZ>(false, "rotateZ(_)")  //
                 .bindMethod<void (math::Transform::*)(float), &math::Transform::scale>(false,
                                                                                        "uniformScale(_)")  //
                 .bindMethod<void (math::Transform::*)(float, float, float), &math::Transform::scale>(
                     false,
                     "scaleWithFloats(_,_,_)")  //
                 .bindMethod<void (math::Transform::*)(const math::Vector3&), &math::Transform::scale>(
                     false,
                     "scaleWithVector(_)")                                                                      //
                 .bindMethod<decltype(&math::Transform::scaleX), &math::Transform::scaleX>(false, "scaleX(_)")  //
                 .bindMethod<decltype(&math::Transform::scaleY), &math::Transform::scaleY>(false, "scaleY(_)")  //
                 .bindMethod<decltype(&math::Transform::scaleZ), &math::Transform::scaleZ>(false, "scaleZ(_)")  //
                 .endClass()                                                                                    //
                 .endModule();
    }

    void bindUtils()
    {
        Engine::vm()
            .beginModule("scripts/Utils")
            .bindClass<StringHash, const std::string&>("StringHash")
            .bindMethod<decltype(&StringHash::hash), &StringHash::hash>(false, "hash()")
            .endClass()
            .endModule();
    }

    void bindAssets()
    {
        bindAssetTypes();
    }

}  // wren namespace
}  // atlas namespace
