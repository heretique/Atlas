add_library(Reflection INTERFACE
    PUBLIC
        Public/Reflection/Reflection.h
        Public/Reflection/Registration.h
    )
add_library(Atlas::Reflection ALIAS Reflection)

target_include_directories(Reflection
    INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>)

target_link_libraries(Reflection INTERFACE RTTR::Core_Lib)
