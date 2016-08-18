cmake_minimum_required (VERSION 2.8.11)
project (upm)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
message("WARNING: building into sources dir can be risky, prefer other directory")
endif ()

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)

# Force a libmraa search and minimum required version every time a config is generated
unset(MRAA_FOUND CACHE)
pkg_check_modules (MRAA REQUIRED mraa>=1.1.1)
message (INFO " found mraa version: ${MRAA_VERSION}")

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Set CMAKE_INSTALL_LIBDIR if not defined
include(GNUInstallDirs)
set (LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Installation path for libraries")

# Make a version file containing the current version from git.
include (GetGitRevisionDescription)
git_describe (VERSION "--tags")
if ("x_${VERSION}" STREQUAL "x_GIT-NOTFOUND" OR "x_${VERSION}" STREQUAL "x_-128-NOTFOUND")
  message (WARNING " - Install git to compile a production UPM!")
  set (VERSION "v0.7.3-dirty")
endif ()

message (INFO " - UPM Version ${VERSION}")

# Parse the version information into pieces.
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

option (BUILDDOC "Build all doc" OFF)
option (BUILDCPP "Build CPP sensor libraries" ON)
option (BUILDFTI "Build Funtion Table Interface (FTI) in C sensor libraries" OFF)
option (BUILDSWIGPYTHON "Build swig python modules" ON)
option (BUILDSWIGNODE "Build swig node modules" ON)
option (BUILDSWIGJAVA "Build swig java modules" OFF)
option (BUILDPYTHON3 "Use python3 for building/installing/testing" OFF)
option (BUILDEXAMPLES "Build C/C++ example binaries" OFF)
option (BUILDJAVAEXAMPLES "Build java example jars" OFF)
option (IPK "Generate IPK using CPack" OFF)
option (RPM "Generate RPM using CPack" OFF)
option (BUILDTESTS "Generate check-ups for upm" ON)
option (ENABLECXX11 "Enable C++11 standards support" ON)

# Find swig
if (BUILDSWIGPYTHON OR BUILDSWIGNODE OR BUILDSWIGJAVA)
    if (BUILDSWIGNODE)
        find_package (SWIG 3.0.5 REQUIRED)
    else ()
        find_package (SWIG REQUIRED)
    endif ()
    if (SWIG_FOUND)
        include (${SWIG_USE_FILE})
    endif ()
endif ()

# Detect arch
include (TargetArch)
target_architecture (DETECTED_ARCH)
message( INFO " - Target arch is ${DETECTED_ARCH}")

#-march=native for ARM when not defined/forced
if (DETECTED_ARCH MATCHES "arm.*" AND NOT CMAKE_CXX_FLAGS MATCHES "-march")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# enable c++11 standards support
if (ENABLECXX11)
  include(CheckCXXCompilerFlag)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if (COMPILER_SUPPORTS_CXX11)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif (COMPILER_SUPPORTS_CXX0X)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
      message(WARNING "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please update your C++ compiler.")
    endif()
  else()
    # 3.1+ uses this generic method to enable c++11
    set (CMAKE_CXX_STANDARD 11)
  endif()
else()
  message(WARNING "Some modules require C++11 support, and may not build without it.")
endif()

if (BUILDDOC)
  # Add a target to generate API documentation with Doxygen
  find_package (Doxygen)
  if (DOXYGEN_FOUND)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    if (BUILDSWIGJAVA)
        configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/Doxyfile.java.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile-java @ONLY)
    endif()
    file(GLOB PNG_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/docs docs/icons/*.png)
    foreach(PNG_FILE ${PNG_FILES})
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/${PNG_FILE} ${CMAKE_CURRENT_BINARY_DIR}/html/docs/${PNG_FILE} COPYONLY)
    endforeach()
    add_custom_target (doc
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      COMMAND tar -czf html/xml.tar.gz -C xml .
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
  endif (DOXYGEN_FOUND)
  # Check if Sphinx is installed and add target to generate API documentation
  find_package (Sphinx)
  if(SPHINX_FOUND AND BUILDSWIGPYTHON)
    # Python required for Sphinx build
    find_package (PythonLibs ${PYTHONBUILD_VERSION} REQUIRED)
    string (REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
    list (GET PYTHON_VERSION_LIST 0 PYTHON_VERSION_MAJOR)
    list (GET PYTHON_VERSION_LIST 1 PYTHON_VERSION_MINOR)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/pydoc/conf.py @ONLY)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxy/index.rst ${CMAKE_CURRENT_BINARY_DIR}/pydoc/index.rst COPYONLY)
    add_custom_target (pydoc ALL
      COMMAND rm -r -f ${CMAKE_BINARY_DIR}/pyupm && mkdir -p ${CMAKE_BINARY_DIR}/pyupm
      COMMAND find ${CMAKE_BINARY_DIR}/src -name "_pyupm_*.so" -exec cp {} ${CMAKE_BINARY_DIR}/pyupm \;
      COMMAND find ${CMAKE_BINARY_DIR}/src -name "pyupm_*.py" -exec cp {} ${CMAKE_BINARY_DIR}/pyupm \;
      COMMAND ${SPHINX_API_EXECUTABLE} -f -o pydoc  ${CMAKE_BINARY_DIR}/pyupm
      # TODO: use a separate cmake FILE module for string replacement instead
      COMMAND ${SPHINX_EXECUTABLE} -b html pydoc html/python
      COMMAND sed -i.bak s|\">pyupm_|\">|g html/python/index.html html/python/modules.html
      COMMAND sed -i.bak s|[[:space:]][mM]odule</a>|</a>|g html/python/index.html html/python/modules.html
      DEPENDS doc
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Sphinx" VERBATIM
    )
  endif ()
  # Check if Yuidoc is installed and add target for API documentation
  find_package(Yuidoc)
  if(YUIDOC_FOUND AND BUILDSWIGNODE)
    # Node required for Yuidoc documentation
    find_package(Node)
    file(GLOB_RECURSE JSDOC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/doxy/node doxy/node/*)
    foreach(JSDOC_FILE ${JSDOC_FILES})
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxy/node/${JSDOC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${JSDOC_FILE} COPYONLY)
    endforeach()
    add_custom_target(jsdoc ALL
      COMMAND ${NODEJS_EXECUTABLE} docgen -m upm -i xml -t ${CMAKE_CURRENT_SOURCE_DIR}/src -g ../../
      COMMAND ${YUIDOC_EXECUTABLE} -C --no-sort --helpers generators/yuidoc/helper.js --themedir generators/yuidoc/tmpl -o html/node jsdoc/yuidoc/upm
      COMMAND ${NODEJS_EXECUTABLE} tolower -i html/node
      DEPENDS doc
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Yuidoc" VERBATIM
    )
  endif()
endif()

if (IPK)

 # Get target package arch from Yocto ADT sysroot if set or host OS, mapping to Ubuntu name if necessary
  if (DEFINED ENV{OECORE_TARGET_SYSROOT})
    GET_FILENAME_COMPONENT (DETECTED_SYSROOT $ENV{OECORE_TARGET_SYSROOT} NAME)
    string (REGEX REPLACE "-poky-linux" "" TARGET_ARCH "${DETECTED_SYSROOT}")
  else ()
    # Debian uses amd64 to denote x86_64
    if (DETECTED_ARCH STREQUAL "x86_64")
      set (TARGET_ARCH "amd64")
    else ()
      set (TARGET_ARCH ${DETECTED_ARCH})
    endif ()
  endif ()
  message (INFO " - Package arch is ${TARGET_ARCH}")

  set(CPACK_GENERATOR "DEB")
  set(OPKG_ARCH ${TARGET_ARCH})
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
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${TARGET_ARCH})
  set(CPACK_SYSTEM_NAME ${TARGET_ARCH})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "mraa (>= ${MRAA_VERSION})")
  set(CPACK_DEBIAN_PACKAGE_PROVIDES "upm-dev, upm-dbg, upm-doc")
  set(CPACK_DEBIAN_PACKAGE_REPLACES ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(CPACK_DEBIAN_PACKAGE_CONFLICTS ${CPACK_DEBIAN_PACKAGE_PROVIDES})
  set(WDIR "${CPACK_TOPLEVEL_DIRECTORY}/${CPACK_PACKAGE_FILE_NAME}")
  include (CPack)
endif()

if (RPM)
  message (INFO " - RPM packaging enabled for ${DETECTED_ARCH}")
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "RPM")
  set(CPACK_PACKAGE_NAME "upm")
  set(upm_PACKAGE_ON_TAG ".")
  if ("${VERSION_COMMIT}" STREQUAL "")
    set(upm_PACKAGE_ON_TAG "")
  endif()
  set(CPACK_PACKAGE_VERSION
      "${upm_VERSION_MAJOR}.${upm_VERSION_MINOR}.${upm_VERSION_PATCH}${upm_PACKAGE_ON_TAG}${VERSION_COMMIT}")
  set(CPACK_PACKAGE_CONTACT "Intel IoT-Devkit")
  set(CPACK_PACKAGE_VENDOR "Intel IoT-Devkit")
  set(CPACK_RPM_PACKAGE_REQUIRES "mraa >= ${MRAA_VERSION}")
  set(CPACK_RPM_PACKAGE_PROVIDES "${CPACK_PACKAGE_NAME}-devel")
  set(CPACK_RPM_PACKAGE_LICENSE "MIT")
  EXECUTE_PROCESS(COMMAND rpm --showrc
    COMMAND grep -E "dist[[:space:]]*\\."
    COMMAND sed -e "s/^.*dist\\s*\\.//"
    COMMAND tr \\n \\t
    COMMAND sed  -e s/\\t//
    OUTPUT_VARIABLE DIST_TAG)
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.${DIST_TAG}.${DETECTED_ARCH}")
  include(CPack)
endif()

if (BUILDSWIGPYTHON OR BUILDTESTS)
  if (BUILDPYTHON3)
    set (PYTHONBUILD_VERSION 3)
  else ()
    set (PYTHONBUILD_VERSION 2.7)
  endif ()
endif ()

# UPM common headers
set (UPM_COMMON_HEADER_DIRS
    ${CMAKE_HOME_DIRECTORY}/include
    ${CMAKE_HOME_DIRECTORY}/include/fti)

add_subdirectory (src)
if(BUILDEXAMPLES)
  add_subdirectory (examples/c)
  if(BUILDCPP)
    add_subdirectory (examples/c++)
  endif(BUILDCPP)
endif()

if(BUILDJAVAEXAMPLES)
  add_subdirectory (examples/java)
endif()

if (BUILDTESTS)
  find_package (PythonInterp ${PYTHONBUILD_VERSION} REQUIRED)
  enable_testing ()
  add_subdirectory (tests)
endif()

# Install C headers
install(DIRECTORY include/ DESTINATION include/upm
    FILES_MATCHING PATTERN "*.h")
