#pragma once

namespace atlas
{

// -------------- Type Name
template<typename T>
struct TypeName;

#define REGISTER_TYPE_NAME(X) template <> struct TypeName<X> \
    { static const char* name; } ; const char* TypeName<X>::name = #X
//-------------------------



} // atlas namespace
