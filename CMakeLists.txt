cmake_minimum_required (VERSION 2.8)
project (upm)

find_package (SWIG)
if (SWIG_FOUND)
  include (${SWIG_USE_FILE})
endif ()

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
# force the libmaa version to be the required version
pkg_check_modules (MAA REQUIRED maa>=0.2.6)
message (INFO " found libmaa version: ${MAA_VERSION}")

set (CMAKE_SWIG_FLAGS "")

# add a target to generate API documentation with Doxygen
find_package (Doxygen)
if (DOXYGEN_FOUND)
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target (doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif (DOXYGEN_FOUND)

include(CPack)
add_subdirectory (src)
add_subdirectory (examples)
