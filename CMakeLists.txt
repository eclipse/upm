cmake_minimum_required (VERSION 2.8.11)
project (upm)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
message("WARNING: building into sources dir can be risky, prefer other directory")
endif ()

find_package (SWIG)
if (SWIG_FOUND)
  include (${SWIG_USE_FILE})
endif ()

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)

# force a libmraa search and minimum required version every time a config is generated
unset(MRAA_FOUND CACHE)
pkg_check_modules (MRAA REQUIRED mraa>=0.6.0)
message (INFO " found libmraa version: ${MRAA_VERSION}")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
set (LIB_INSTALL_DIR "lib${LIB_SUFFIX}" CACHE PATH "Installation path for libraries")

# Set CMAKE_LIB_INSTALL_DIR if not defined
include(GNUInstallDirs)

# Make a version file containing the current version from git.
include (GetGitRevisionDescription)
git_describe (VERSION "--tags")
if ("x_${VERSION}" STREQUAL "x_GIT-NOTFOUND")
  message (WARNING " - Install git to compile a production libmraa!")
  set (VERSION "v0.3.0-dirty")
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
option (BUILDEXAMPLES "Build C++ example binaries" OFF)

if (BUILDDOC)
  # add a target to generate API documentation with Doxygen
  find_package (Doxygen)
  if (DOXYGEN_FOUND)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target (doc
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
  endif (DOXYGEN_FOUND)
  # check if Sphinx is installed and add target to generate API documentation
  find_package (Sphinx)
  if(SPHINX_FOUND AND BUILDSWIG AND BUILDSWIGPYTHON)
    # python required for Sphinx build
    find_package (PythonLibs)
    string (REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
    list (GET PYTHON_VERSION_LIST 0 PYTHON_VERSION_MAJOR)
    list (GET PYTHON_VERSION_LIST 1 PYTHON_VERSION_MINOR)
    set (SITE_PACKAGES ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/pydoc/conf.py @ONLY)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/index.rst ${CMAKE_CURRENT_BINARY_DIR}/pydoc/index.rst COPYONLY)
    add_custom_target (pydoc ALL
      COMMAND rm -r -f ${CMAKE_BINARY_DIR}/pyupm && mkdir -p ${CMAKE_BINARY_DIR}/pyupm
      COMMAND find ${CMAKE_BINARY_DIR}/src -name "_pyupm_*.so" -exec cp {} ${CMAKE_BINARY_DIR}/pyupm \;
      COMMAND find ${CMAKE_BINARY_DIR}/src -name "pyupm_*.py" -exec cp {} ${CMAKE_BINARY_DIR}/pyupm \;
      COMMAND ${SPHINX_API_EXECUTABLE} -f -o pydoc  ${CMAKE_BINARY_DIR}/pyupm
      # todo: use a separate cmake FILE module for string replacement instead
      COMMAND ${SPHINX_EXECUTABLE} -b html pydoc html/python
      COMMAND sed -i.bak s|\">pyupm_|\">|g html/python/index.html html/python/modules.html
      COMMAND sed -i.bak s|[[:space:]]module</a>|</a>|g html/python/index.html html/python/modules.html
      DEPENDS doc
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Sphinx" VERBATIM
    )
  endif ()
  # check if Yuidoc is installed and add target for API documentation
  find_package(Yuidoc)
  if(YUIDOC_FOUND AND BUILDSWIGNODE)
    # node required for Yuidoc documentation
    find_package(Node)
    file(GLOB_RECURSE JSDOC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/doxy/node doxy/node/*)
    foreach(JSDOC_FILE ${JSDOC_FILES})
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxy/node/${JSDOC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${JSDOC_FILE} COPYONLY)
    endforeach()
    add_custom_target(jsdoc ALL
      COMMAND ${NODE_EXECUTABLE} docgen -m upm -i xml -t ${CMAKE_CURRENT_SOURCE_DIR}/src -g ../../../../docs/images/
      COMMAND ${YUIDOC_EXECUTABLE} -C --no-sort --helpers generators/yuidoc/helper.js --themedir generators/yuidoc/tmpl -o html/node jsdoc/yuidoc/upm
      COMMAND ${NODE_EXECUTABLE} tolower -i html/node
      DEPENDS doc
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Yuidoc" VERBATIM
    )
  endif()
endif()

if (IPK)
  include (TargetArch)
  target_architecture (DETECTED_ARCH)
  message( INFO " - Target arch is ${DETECTED_ARCH}")

  set(CPACK_GENERATOR "DEB" "TGZ")
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
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libmraa0 (>= ${MRAA_VERSION})")
  set(CPACK_DEBIAN_PACKAGE_PROVIDES "upm-dev, upm-dbg, upm-doc")
  set(CPACK_DEBIAN_PACKAGE_REPLACES ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(CPACK_DEBIAN_PACKAGE_CONFLICTS ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(WDIR "${CPACK_TOPLEVEL_DIRECTORY}/${CPACK_PACKAGE_FILE_NAME}")
  include (CPack)
endif()

add_subdirectory (src)

if(BUILDEXAMPLES)
  add_subdirectory (examples/c++)
endif()
