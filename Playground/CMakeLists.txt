
option (ATLAS_PLAYGROUND_TESTPLAYER "Build Engine Test Player" ON)
option (ATLAS_PLAYGROUND_SDFCOMPUTE "Build SDF Compute Playground" ON)

if (ATLAS_PLAYGROUND_TESTPLAYER)
    add_subdirectory(TestPlayer)
endif()

if (ATLAS_PLAYGROUND_SDFCOMPUTE)
	add_subdirectory(SDFCompute)
endif()
