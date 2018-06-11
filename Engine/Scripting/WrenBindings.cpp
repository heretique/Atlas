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
            .beginModule("main")
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

        Engine::wrenModule() +=
            "foreign class ImguiFlag {\n"
            "    construct new() {}\n"
            "    foreign setTitleBar()   // on by default\n"
            "    foreign unsetTitleBar()\n"
            "    foreign setResize()     // on by default\n"
            "    foreign unsetResize()\n"
            "    foreign setMove()       // on by default\n"
            "    foreign unsetMove()\n"
            "    foreign setShowBorders()    // off by default\n"
            "    foreign unsetShowBorders()\n"
            "}\n"
            "\n"
            "class Imgui {\n"
            "\n"
            "    // windows\n"
            "    foreign static begin( name )    // begin a window scope\n"
            "    foreign static begin( name, flags ) // begin a window scope with window flags\n"
            "    foreign static end()            // end a window scope\n"
            "    foreign static treeNode( name )\n"
            "    foreign static treePop()\n"
            "    foreign static text( str )\n"
            "    foreign static textColored( color, str )\n"
            "\n"
            "    // window control\n"
            "    foreign static setNextWindowPos( size )\n"
            "    foreign static setNextWindowSize( size )    // call before begin()\n"
            "\n"
            "    foreign static getWindowSize()\n"
            "\n"
            "    // formatting\n"
            "    foreign static spacing()\n"
            "    foreign static dummy( size )    // add a dummy of a given size, as Vec2\n"
            "    foreign static indent()\n"
            "    foreign static unindent()\n"
            "\n"
            "    // elements\n"
            "    foreign static bullet()             // a bullet point without text\n"
            "    foreign static bulletText( label )  // a bullet point with text\n"
            "    foreign static button( label )\n"
            "    foreign static button( label, size) // size is of type Vec2\n"
            "    foreign static radioButton( label, active ) // WTF, how do I use this?\n"
            "    foreign static checkBox( label, active )\n"
            "    foreign static plotArray( label, array, count )\n"
            "    foreign static plotArray( label, array, count, offset )\n"
            "    foreign static plotArray( label, array, count, offset, size )\n"
            "    foreign static sliderFloat( label, value, min, max )    // returns the new value as a number\n"
            "\n"
            "    static treeNode( name, body ) {\n"
            "        if ( treeNode( name ) ) {\n"
            "            body.call()\n"
            "            treePop()\n"
            "        }\n"
            "    }\n"
            "}\n";
    }

    void bindMath()
    {
        Engine::vm()
                .beginModule("main")                                                                     // Module Math
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

        Engine::wrenModule() +=
            "foreign class Vec2 {\n"
            "    construct new(x, y) {}\n"
            "\n"
            "    foreign x\n"
            "    foreign x=(rhs)\n"
            "    foreign y\n"
            "    foreign y=(rhs)\n"
            "    foreign i\n"
            "    foreign i=(rhs)\n"
            "    foreign j\n"
            "    foreign j=(rhs)\n"
            "\n"
            "    foreign static angle(v1, v2)\n"
            "    foreign clamp(min, max)\n"
            "    foreign distance(v)\n"
            "    foreign distanceSquared(v)\n"
            "    foreign dot(rhs)\n"
            "    foreign static dot(v1, v2)\n"
            "    foreign length()\n"
            "    foreign lengthSquared()\n"
            "    foreign negate()\n"
            "    foreign normalize() // normalizes the vector in place\n"
            "    foreign scale(s)\n"
            "    foreign smooth(target, elapsedTime, responseTime)\n"
            "\n"
            "    foreign +(rhs)\n"
            "    foreign -(rhs)\n"
            "    foreign *(rhs)\n"
            "    \n"
            "\n"
            "    static one { Vec2.new(1, 1) }\n"
            "    static zero { Vec2.new(0, 0) }\n"
            "    isOne() { x == 1 && y == 1 }\n"
            "    isZero() { x == 0 && y == 0  }\n"
            "    toString() {\"(%(x), %(y))\"}\n"
            "}\n"
            "\n"
            "foreign class Vec3 {\n"
            "    construct new( x, y, z ) {}\n"
            "\n"
            "    foreign x\n"
            "    foreign x=( rhs )\n"
            "    foreign y\n"
            "    foreign y=( rhs )\n"
            "    foreign z\n"
            "    foreign z=( rhs )\n"
            "    foreign r\n"
            "    foreign r=( rhs )\n"
            "    foreign g\n"
            "    foreign g=( rhs )\n"
            "    foreign b\n"
            "    foreign b=( rhs )\n"
            "\n"
            "    foreign static angle(v1, v2)\n"
            "    foreign clamp(min, max)\n"
            "    foreign cross( rhs )    // returns the result\n"
            "    foreign distance(v)\n"
            "    foreign distanceSquared(v)\n"
            "    foreign dot(rhs)\n"
            "    foreign static dot(v1, v2)\n"
            "    foreign length()\n"
            "    foreign lengthSquared()\n"
            "    foreign negate()\n"
            "    foreign normalize() // normalizes the vector in place\n"
            "    foreign scale(s)\n"
            "    foreign smooth(target, elapsedTime, responseTime)\n"
            "\n"
            "    foreign *( rhs )    // returns the result\n"
            "    foreign +( rhs )     // returns the result\n"
            "    foreign -( rhs )    // returns the result\n"
            "\n"
            "\n"
            "    static one { Vec3.new(1, 1, 1) }\n"
            "    static zero { Vec3.new(0, 0, 0) }\n"
            "    isOne() { x == 1 && y == 1 && z == 1 }\n"
            "    isZero() { x == 0 && y == 0 && z == 0 }\n"
            "    toString() {\"(%(x), %(y), %(z))\"}\n"
            "}\n"
            "\n"
            "foreign class Vec4 {\n"
            "    construct new( x, y, z, w ) {}\n"
            "\n"
            "    foreign x\n"
            "    foreign x=( rhs )\n"
            "    foreign y\n"
            "    foreign y=( rhs )\n"
            "    foreign z\n"
            "    foreign z=( rhs )\n"
            "    foreign w\n"
            "    foreign w=( rhs )\n"
            "    foreign r\n"
            "    foreign r=( rhs )\n"
            "    foreign g\n"
            "    foreign g=( rhs )\n"
            "    foreign b\n"
            "    foreign b=( rhs )\n"
            "    foreign a\n"
            "    foreign a=( rhs )\n"
            "\n"
            "    foreign static angle(v1, v2)\n"
            "    foreign clamp(min, max)\n"
            "    foreign distance(v)\n"
            "    foreign distanceSquared(v)\n"
            "    foreign dot(rhs)\n"
            "    foreign static dot(v1, v2)\n"
            "    foreign length()\n"
            "    foreign lengthSquared()\n"
            "    foreign negate()\n"
            "    foreign normalize() // normalizes the vector in place\n"
            "    foreign scale(s)\n"
            "\n"
            "    foreign *( rhs )    // returns the result\n"
            "    foreign +( rhs )     // returns the result\n"
            "    foreign -( rhs )    // returns the result\n"
            "\n"
            "\n"
            "    static one { Vec3.new(1, 1, 1, 1) }\n"
            "    static zero { Vec3.new(0, 0, 0, 0) }\n"
            "    isOne() { x == 1 && y == 1 && z == 1 && w == 1 }\n"
            "    isZero() { x == 0 && y == 0 && z == 0 && w == 0 }\n"
            "    toString() {\"(%(x), %(y), %(z), %(w))\"}\n"
            "}\n"
            "\n"
            "foreign class Quaternion {\n"
            "    construct new() {}\n"
            "\n"
            "    foreign x\n"
            "    foreign x=( rhs )\n"
            "    foreign y\n"
            "    foreign y=( rhs )\n"
            "    foreign z\n"
            "    foreign z=( rhs )\n"
            "    foreign w\n"
            "    foreign w=( rhs )\n"
            "\n"
            "    foreign static identity\n"
            "    foreign static zero\n"
            "\n"
            "    foreign setIdentity()\n"
            "    foreign setFromValues(x, y, z, w)\n"
            "    foreign setFromMatrix(m)\n"
            "    foreign setFromAxisAngle(axis, angle) // vec3, float\n"
            "    foreign setFromQuaternion(quaternion)\n"
            "\n"
            "    foreign static lerp(q1, q2, t, qdest)\n"
            "    foreign static slerp(q1, q2, t, qdest)\n"
            "\n"
            "    foreign *( rhs )\n"
            "}\n"
            "\n"
            "foreign class Matrix {\n"
            "    construct new() {}\n"
            "\n"
            "    foreign static identity\n"
            "    foreign static zero\n"
            "\n"
            "    foreign setIdentity()\n"
            "    foreign setZero()\n"
            "}\n"
            "\n"
            "foreign class Transform {\n"
            "    construct new() {}\n"
            "\n"
            "    foreign matrix\n"
            "    foreign scale\n"
            "    foreign scaleX\n"
            "    foreign scaleY\n"
            "    foreign scaleZ\n"
            "    foreign rotation\n"
            "    foreign translation\n"
            "    foreign translationX\n"
            "    foreign translationY\n"
            "    foreign translationZ\n"
            "    foreign forward\n"
            "    foreign up\n"
            "    foreign right\n"
            "    foreign rotateWithQuaternionFloats(x, y, z, w)\n"
            "    foreign rotateWithQuaternion(quaternion)\n"
            "    foreign rotateWithAxisAngle(axis, angle)\n"
            "    foreign rotateWithMatrix(matrix)\n"
            "    foreign rotateX(angle)\n"
            "    foreign rotateY(angle)\n"
            "    foreign rotateZ(angle)\n"
            "    foreign uniformScale(scale)\n"
            "    foreign scaleWithFloats(x, y, z)\n"
            "    foreign scaleWithVector(vector)\n"
            "    foreign scaleX(scale)\n"
            "    foreign scaleY(scale)\n"
            "    foreign scaleZ(scale)\n"
            "}\n";
    }

    void bindUtils()
    {
        Engine::vm()
            .beginModule("main")
            .bindClass<StringHash, const std::string&>("StringHash")
            .bindMethod<decltype(&StringHash::hash), &StringHash::hash>(false, "hash()")
            .endClass()
            .endModule();

        Engine::wrenModule() +=
            "foreign class StringHash {\n"
            "   construct new (string) {}\n"
            "   foreign hash()\n"
            "   toString() {\n"
            "       var hash = hash()\n"
            "       return \"%(hash)\"\n"
            "   }\n"
            "}\n";
    }
}  // wren namespace
}  // atlas namespace
