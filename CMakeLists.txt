cmake_minimum_required (VERSION 2.8)
project (upm)

set (SWIG_EXECUTABLE /usr/bin/swig)
find_package (SWIG REQUIRED)
include (${SWIG_USE_FILE})

find_package (PkgConfig REQUIRED)
pkg_check_modules (MAA maa>=0.2.1)

set (CMAKE_SWIG_FLAGS "")

add_subdirectory (src)
add_subdirectory (examples)
