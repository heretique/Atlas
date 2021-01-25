add_library(DebugDraw STATIC
	PRIVATE
		Private/DebugDraw.cpp
	PUBLIC
        Public/DebugDraw/DebugDraw.h
	)
add_library(Atlas::DebugDraw ALIAS DebugDraw)

target_include_directories(DebugDraw
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(DebugDraw Atlas::Core Atlas::Assets Atlas::Materials)
