add_subdirectory(ysbitmap/cutout)
add_subdirectory(ysbitmap/pngEncoder)
add_subdirectory(ysbitmap/pngDecoder)

add_subdirectory(ysclass/YsVec2YsVec3YsAtt3)
add_subdirectory(ysclass/YsRelDir)
add_subdirectory(ysclass/YsStringParser)
add_subdirectory(ysclass/YsSegmentedArray)
add_subdirectory(ysclass/YsKdTreeNearestPoint)
add_subdirectory(ysclass/YsPointFinderLattice3d)
add_subdirectory(ysclass/YsCheckInsidePolygon2)
add_subdirectory(ysclass/YsMatrix4x4)
add_subdirectory(ysclass/YsMatrix)
add_subdirectory(ysclass/YsSword)
add_subdirectory(ysclass/ScreenPolygon)
add_subdirectory(ysclass/DDA)
add_subdirectory(ysclass/YsShell_Boolean)
add_subdirectory(ysclass/YsSplitDegeneratePolygon)

add_subdirectory(ysgebl/kernel/fileio)
add_subdirectory(ysgebl/kernel/search)


set(YS_ALL_BATCH_TEST ${YS_ALL_BATCH_TEST} PARENT_SCOPE)
