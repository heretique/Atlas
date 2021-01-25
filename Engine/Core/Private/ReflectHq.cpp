#include <rttr/registration>
#include "Hq/Math/MathTypes.h"
#include "Hq/Math/Mat4x4.h"

namespace atlas
{
using namespace rttr;
using namespace hq::math;
RTTR_REGISTRATION
{
    // Math Types
    registration::class_<Vec2>("Vec2")
        .constructor<>()
        .constructor<float>()
        .constructor<float, float>()
        .constructor<const float*>()
        .property("data", &Vec2::data)
        .property("x", &Vec2::x)
        .property("y", &Vec2::y)(policy::ctor::as_object);

    registration::class_<Vec2i>("Vec2i")
        .constructor<>()
        .constructor<int>()
        .constructor<int, int>()
        .constructor<const int*>()
        .property("data", &Vec2i::data)
        .property("x", &Vec2i::x)
        .property("y", &Vec2i::y)(policy::ctor::as_object);

    registration::class_<Vec3>("Vec3")
        .constructor<>()
        .constructor<float>()
        .constructor<const Vec2&, float>()
        .constructor<float, float, float>()
        .constructor<const float*>()
        .property("data", &Vec3::data)
        .property("x", &Vec3::x)
        .property("y", &Vec3::y)
        .property("z", &Vec3::z)(policy::ctor::as_object);

    registration::class_<Vec4>("Vec4")
        .constructor<>()
        .constructor<float>()
        .constructor<const Vec2&, float, float>()
        .constructor<const Vec3&, float>()
        .constructor<float, float, float, float>()
        .constructor<const float*>()
        .property("data", &Vec4::data)
        .property("x", &Vec4::x)
        .property("y", &Vec4::y)
        .property("z", &Vec4::z)
        .property("w", &Vec4::w)(policy::ctor::as_object);

    registration::class_<Triangle>("Triangle")
        .constructor<const Vec3&, const Vec3&, const Vec3&>()
        .constructor<const float*>()
        .property("points", &Triangle::points)(policy::ctor::as_object);

    registration::class_<Rect>("Rect")
        .constructor<>()
        .constructor<float, float>()
        .constructor<float, float, float, float>()
        .constructor<const Vec2&>()
        .constructor<const Vec2&, const Vec2&>()
        .property("x", &Rect::x)
        .property("y", &Rect::y)
        .property("width", &Rect::width)
        .property("heigh", &Rect::height)(policy::ctor::as_object);

    registration::class_<Plane>("Plane")
        .constructor<>()
        .constructor<const Vec3&, float>()
        .constructor<float, float, float, float>()
        .property("normal", &Plane::normal)
        .property("distance", &Plane::distance)(policy::ctor::as_object);

    registration::class_<Mat3x3>("Mat3x3")
        .constructor<>()
        .constructor<const float*>()
        .constructor<float, float, float, float, float, float, float, float, float>()
        .property("data", &Mat3x3::data)
        .property("m11", &Mat3x3::m11)
        .property("m12", &Mat3x3::m12)
        .property("m13", &Mat3x3::m13)
        .property("m21", &Mat3x3::m21)
        .property("m22", &Mat3x3::m22)
        .property("m23", &Mat3x3::m23)
        .property("m31", &Mat3x3::m31)
        .property("m32", &Mat3x3::m32)
        .property("m33", &Mat3x3::m33)(policy::ctor::as_object);

    registration::class_<Mat4x4>("Mat4x4")
        .constructor<>()
        .constructor<const float*>()
        .property("data", &Mat4x4::data)
        .property("m11", &Mat4x4::m11)
        .property("m12", &Mat4x4::m12)
        .property("m13", &Mat4x4::m13)
        .property("m14", &Mat4x4::m14)
        .property("m21", &Mat4x4::m21)
        .property("m22", &Mat4x4::m22)
        .property("m23", &Mat4x4::m23)
        .property("m24", &Mat4x4::m24)
        .property("m31", &Mat4x4::m31)
        .property("m32", &Mat4x4::m32)
        .property("m33", &Mat4x4::m33)
        .property("m34", &Mat4x4::m34)
        .property("m41", &Mat4x4::m41)
        .property("m42", &Mat4x4::m42)
        .property("m43", &Mat4x4::m43)
        .property("m44", &Mat4x4::m44)(policy::ctor::as_object);

    registration::class_<Quat>("Quat")
        .constructor<>()
        .constructor<float, float, float, float>()
        .constructor<const Vec3&, float>()
        .constructor<const Mat3x3&>()
        .constructor<const Mat4x4&>()
        .property("data", &Quat::data)
        .property("x", &Quat::x)
        .property("y", &Quat::y)
        .property("z", &Quat::z)
        .property("w", &Quat::w)(policy::ctor::as_object);

    registration::class_<Line2>("Line2")
        .constructor<>()
        .constructor<const Vec2&, const Vec2&>()
        .property("p1", &Line2::p1)
        .property("p2", &Line2::p2)(policy::ctor::as_object);

    registration::class_<Line3>("Line3")
        .constructor<>()
        .constructor<const Vec3&, const Vec3&>()
        .property("p1", &Line3::p1)
        .property("p2", &Line3::p2)(policy::ctor::as_object);

    registration::class_<Ray2>("Ray2")
        .constructor<>()
        .constructor<const Vec2&, const Vec2&>()
        .property("origin", &Ray2::origin)
        .property("direction", &Ray2::direction)(policy::ctor::as_object);

    registration::class_<Ray3>("Ray3")
        .constructor<>()
        .constructor<const Vec3&, const Vec3&>()
        .property("origin", &Ray3::origin)
        .property("direction", &Ray3::direction)(policy::ctor::as_object);

    registration::class_<Box2>("Box2")
        .constructor<>()
        .constructor<const Vec2&, const Vec2&>()
        .property("min", &Box2::min)
        .property("max", &Box2::max)
        .method("center", &Box2::center)
        .method("extent", &Box2::extent)(policy::ctor::as_object);

    registration::class_<Box2i>("Box2i")
        .constructor<>()
        .constructor<const Vec2i&, const Vec2i&>()
        .property("min", &Box2i::min)
        .property("max", &Box2i::max)
        .method("center", &Box2i::center)
        .method("extent", &Box2i::extent)(policy::ctor::as_object);

    registration::class_<Box3>("Box3")
        .constructor<>()
        .constructor<const Vec3&, const Vec3&>()
        .property("min", &Box3::min)
        .property("max", &Box3::max)
        .method("center", &Box3::center)
        .method("extent", &Box3::extent)(policy::ctor::as_object);

    registration::class_<Circle>("Circle")
        .constructor<>()
        .constructor<const Vec2&, float>()
        .property("center", &Circle::center)
        .property("radius", &Circle::radius)(policy::ctor::as_object);

    registration::class_<Sphere>("Sphere")
        .constructor<>()
        .constructor<const Vec3&, float>()
        .property("center", &Sphere::center)
        .property("radius", &Sphere::radius)(policy::ctor::as_object);


}
}  // atlas namespace
