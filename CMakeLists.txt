cmake_minimum_required (VERSION 2.8.11)
project (upm)

find_package (SWIG)
if (SWIG_FOUND)
  include (${SWIG_USE_FILE})
endif ()

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)
# force the libmaa version to be the required version
pkg_check_modules (MAA REQUIRED maa>=0.2.9)
message (INFO " found libmaa version: ${MAA_VERSION}")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Make a version file containing the current version from git.
include (GetGitRevisionDescription)
git_describe (VERSION "--tags")
if ("x_${VERSION}" STREQUAL "x_GIT-NOTFOUND")
  message (WARNING " - Install git to compile a production libmaa!")
  set (VERSION "v0.1.4-dirty")
endif ()

message (INFO " - UPM Version ${VERSION}")

#parse the version information into pieces.
string (REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string (REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string (REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string (REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+\\-([0-9]+).*" "\\1" VERSION_COMMIT "${VERSION}")
string (REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+-[0-9]+\\-(.*)" "\\1" VERSION_SHA1 "${VERSION}")

if ("${VERSION_COMMIT}" MATCHES "^v.*")
  set (VERSION_COMMIT "")
endif()

set (upm_VERSION_MAJOR ${VERSION_MAJOR})
set (upm_VERSION_MINOR ${VERSION_MINOR})
set (upm_VERSION_PATCH ${VERSION_PATCH})
set (upm_VERSION_STRING ${upm_VERSION_MAJOR}.${upm_VERSION_MINOR}.${upm_VERSION_PATCH})

set (CMAKE_SWIG_FLAGS "")

option (IPK "Generate IPK using CPack" OFF)
option (BUILDDOC "Build all doc." OFF)
option (BUILDSWIG "Build swig modules." ON)
option (BUILDSWIGPYTHON "Build swig python modules." ON)
option (BUILDSWIGNODE "Build swig node modules." ON)

if (BUILDDOC)
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
endif()

if (IPK)
  include (TargetArch)
  target_architecture (DETECTED_ARCH)
  message( INFO " - Target arch is ${DETECTED_ARCH}")

  set(CPACK_GENERATOR "DEB")
  set(OPKG_ARCH ${DETECTED_ARCH})
  set(CPACK_BINARY_DIR ${CMAKE_BINARY_DIR})
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Intel IoT-Devkit") #required
  set(upm_PACKAGE_ON_TAG ".")
  if ("${VERSION_COMMIT}" STREQUAL "")
    set(upm_PACKAGE_ON_TAG "")
  endif()
  set(CPACK_PACKAGE_VERSION
      "${upm_VERSION_MAJOR}.${upm_VERSION_MINOR}.${upm_VERSION_PATCH}${upm_PACKAGE_ON_TAG}${VERSION_COMMIT}")
  set(CPACK_PACKAGE_NAME "upm")
  set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DETECTED_ARCH})
  set(CPACK_SYSTEM_NAME ${DETECTED_ARCH})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libmaa0 (>= ${MAA_VERSION})")
  set(CPACK_DEBIAN_PACKAGE_PROVIDES "upm-dev, upm-dbg, upm-doc")
  set(CPACK_DEBIAN_PACKAGE_REPLACES ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(CPACK_DEBIAN_PACKAGE_CONFLICTS ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(WDIR "${CPACK_TOPLEVEL_DIRECTORY}/${CPACK_PACKAGE_FILE_NAME}")
  include (CPack)
endif()

add_subdirectory (src)
add_subdirectory (examples)
