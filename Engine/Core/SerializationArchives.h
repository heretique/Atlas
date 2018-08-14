#pragma once
// These must be included before any other header that contains CEREAL_REGISTER_TYPE macro

// Include any archives you plan on using with your type before you register it.
// This could occur in multiple source files for the same type, if desired.
// Any archives included from other header files included here will also be
// registered.
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

// Include the polymorphic serialization and registration mechanisms
#include <cereal/types/polymorphic.hpp>
