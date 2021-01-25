add_library(Utils STATIC
	PRIVATE
		Private/BgfxUtils.cpp
	PUBLIC
        Public/Utils/BgfxUtils.h
        Public/Utils/TypeTraits.h
	)
add_library(Atlas::Utils ALIAS Utils)

target_include_directories(Utils
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Utils Atlas::Core)
