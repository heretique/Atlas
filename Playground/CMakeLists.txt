
option (ATLAS_PLAYGROUND_TESTPLAYER "Build Engine Test Player" ON)
option (ATLAS_PLAYGROUND_SDFCOMPUTE "Build SDF Compute Playground" ON)

find_package(imgui CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)


if (ATLAS_PLAYGROUND_TESTPLAYER)
    add_subdirectory(TestPlayer)
endif()

if (ATLAS_PLAYGROUND_SDFCOMPUTE)
	add_subdirectory(SDFCompute)
endif()
