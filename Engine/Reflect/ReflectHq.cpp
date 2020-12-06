#include <rttr/registration>
#include "Hq/Math/MathTypes.h"
#include "Hq/Math/Mat4x4.h"

namespace atlas
{
using namespace rttr;
using namespace hq::math;
RTTR_REGISTRATION
{
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
        .property("m44", &Mat4x4::m44);
}
}  // atlas namespace
