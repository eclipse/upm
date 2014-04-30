cmake_minimum_required (VERSION 2.8)
project (upm)

set (SWIG_EXECUTABLE /usr/bin/swig)
find_package (SWIG REQUIRED)
include (${SWIG_USE_FILE})

find_package (PkgConfig REQUIRED)
pkg_check_modules (MAA maa>=0.2.1)
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

add_subdirectory (src)
add_subdirectory (examples)
