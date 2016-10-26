cmake_minimum_required (VERSION 2.8.11)
project (upm)

# Before going any further, define build options
option (BUILDDOC "Build all doc" OFF)
option (BUILDCPP "Build CPP sensor libraries" ON)
option (BUILDFTI "Build Funtion Table Interface (FTI) in C sensor libraries" OFF)
option (BUILDSWIGPYTHON "Build swig python modules" ON)
option (BUILDSWIGNODE "Build swig node modules" ON)
option (BUILDSWIGJAVA "Build swig java modules" OFF)
option (BUILDEXAMPLES "Build C/C++ example binaries" OFF)
option (BUILDJAVAEXAMPLES "Build java example jars" OFF)
option (IPK "Generate IPK using CPack" OFF)
option (RPM "Generate RPM using CPack" OFF)
option (BUILDTESTS "Generate check-ups for upm" OFF)
option (WERROR "Make all warnings into errors." OFF)

# Warn if building in source root
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message (WARNING "Building into sources dir can be risky, prefer other directory")
endif ()

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Check if provided compiler supports target flag
# usage:
#       compiler_flag_supported(C/CXX is_supported flag)
#
# The result of output_variable is set to the sanitized flag name if supported
# or cleared if the flag is not supported.
function (compiler_flag_supported compiler output_variable flag)
  # Currently only C and CXX compile flags, clear and return if not supported
  if (NOT ${compiler} MATCHES "C|CXX")
    message (WARNING "Unknown compiler: ${compiler}")
    set ("${output_variable}" "" PARENT_SCOPE)
    return ()
  endif (NOT ${compiler} MATCHES "C|CXX")

  string (REPLACE "-" "_" SANITIZED_FLAG_NAME "${flag}")
  string (REPLACE "/" "_" SANITIZED_FLAG_NAME "${SANITIZED_FLAG_NAME}")
  string (REPLACE "=" "_" SANITIZED_FLAG_NAME "${SANITIZED_FLAG_NAME}")
  string (REPLACE " " "_" SANITIZED_FLAG_NAME "${SANITIZED_FLAG_NAME}")

  # Disable messages from CHECK_C/XX_COMPILER_FLAGS macros
  set (CMAKE_REQUIRED_QUIET_SAVED ${CMAKE_REQUIRED_QUIET})
  set (CMAKE_REQUIRED_QUIET ON)

  # C or CXX?
  if (${compiler} STREQUAL C)
    CHECK_C_COMPILER_FLAG("${flag}" HAS_${SANITIZED_FLAG_NAME})
  elseif (${compiler} STREQUAL CXX)
    CHECK_CXX_COMPILER_FLAG("${flag}" HAS_${SANITIZED_FLAG_NAME})
  endif ()

  # Restore previous CMAKE_REQUIRED_QUIET state
  set (CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_SAVED})

  # Does the compiler support this flag?
  if (HAS_${SANITIZED_FLAG_NAME})
    set ("${output_variable}" "${SANITIZED_FLAG_NAME}" PARENT_SCOPE)
  else ()
    set ("${output_variable}" "" PARENT_SCOPE)
  endif()
endfunction ()

include (CheckCCompilerFlag)
include (CheckCXXCompilerFlag)
# Add scoped compile flag/s using add_compile_options.
# This function checks to see if each flag is supported
# by the compiler before setting the compile option.
function (upm_add_compile_flags compiler)
  # Iterate the flags, check if supported
  foreach (flag ${ARGN})
    # Check if this compile flag is supported
    compiler_flag_supported(${compiler} is_supported ${flag})

    # Add if supported, warn and skip if not supported
    if (is_supported)
      set (_TMP_COMPILER_FLAGS "${_TMP_COMPILER_FLAGS} ${flag}")
    else ()
      message (WARNING "${compiler} compiler does not support flag \"${flag}\"")
    endif ()
  endforeach (flag ${ARGN})

  # Set the variable in the parent scope
  set (CMAKE_${compiler}_FLAGS ${_TMP_COMPILER_FLAGS} PARENT_SCOPE)
endfunction ()

# Compiler flags common to both C and CXX
set (C_CXX_WARNING_FLAGS -Wall)

# Errors as warnings?
if (WERROR)
  list (APPEND C_CXX_WARNING_FLAGS -Werror)
  message (STATUS "Warnings as errors enabled (-Werror), disable with -DWERROR=off")
endif (WERROR)

# Set C compiler warning flags at top-level scope and emit a warning about
# unsupported flags
upm_add_compile_flags(C ${C_CXX_WARNING_FLAGS}
  -Winit-self
  -Wimplicit
  -Wmissing-parameter-type)

# Set CXX compiler warning flags at top-level scope and emit a warning about
# unsupported flags
upm_add_compile_flags(CXX ${C_CXX_WARNING_FLAGS}
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wreorder)

find_package (Threads REQUIRED)
find_package (PkgConfig REQUIRED)

# Force a libmraa search and minimum required version every time a config is generated
unset(MRAA_FOUND CACHE)
set(MRAA_MINIMUM 1.1.1)
pkg_check_modules (MRAA REQUIRED mraa>=${MRAA_MINIMUM})

# Check for BACNET
pkg_check_modules (BACNET libbacnet)

# Check for MODBUS
pkg_check_modules (MODBUS libmodbus>=3.1.2)

# Check for OPENZWAVE
pkg_check_modules (OPENZWAVE libopenzwave)

# Find JPEG
find_package (JPEG)

# Find nodejs
if (BUILDSWIGNODE)
  find_package (Node REQUIRED)
endif (BUILDSWIGNODE)

# Find swig if any wrapper is enabled
if (BUILDSWIGPYTHON OR BUILDSWIGNODE OR BUILDSWIGJAVA)
  find_package (SWIG 3.0.5 REQUIRED)
  include (${SWIG_USE_FILE})
endif ()

# Python is required for swig generated python and for UPM tests.
# The UPM build can generated modules for both python2 AND python3
# with the corresponding PYTHONLIBS.  Currently, BUILDTESTS has a
# hard dependency on the PYTHON2INTERP.
# OpenCV python detect will attempt to find python2/3
if (BUILDSWIGPYTHON OR BUILDTESTS)
  include (cmake/modules/OpenCVDetectPython.cmake)

  # Fail if building tests but no python interpreter was found
  if (BUILDTESTS AND NOT PYTHON2INTERP_FOUND)
      message(FATAL_ERROR "BUILDTESTS=ON requires the python2 interpreter")
  endif (BUILDTESTS AND NOT PYTHON2INTERP_FOUND)

  # Fail if no LIBS were found
  if (NOT PYTHON2LIBS_FOUND AND NOT PYTHON3LIBS_FOUND)
      message(FATAL_ERROR "At least one python lib is required")
  endif (NOT PYTHON2LIBS_FOUND AND NOT PYTHON3LIBS_FOUND)
endif (BUILDSWIGPYTHON OR BUILDTESTS)

# Which versions of python were found?
if (PYTHON2LIBS_FOUND AND BUILDSWIGPYTHON)
  message(STATUS "Building python2 modules with python-${PYTHON2LIBS_VERSION_STRING}")
endif (PYTHON2LIBS_FOUND AND BUILDSWIGPYTHON)
if (PYTHON3LIBS_FOUND AND BUILDSWIGPYTHON)
  message(STATUS "Building python3 modules with python-${PYTHON3LIBS_VERSION_STRING}")
endif (PYTHON3LIBS_FOUND AND BUILDSWIGPYTHON)

# Python2 is currently required for python documentation
if (BUILDSWIGPYTHON AND BUILDDOC AND NOT PYTHON2INTERP_FOUND)
  message(FATAL_ERROR "Failed to find python2 interpreter which is required "
      "to build python documentation.")
endif (BUILDSWIGPYTHON AND BUILDDOC AND NOT PYTHON2INTERP_FOUND)

# Set CMAKE_INSTALL_LIBDIR if not defined
include(GNUInstallDirs)
set (LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Installation path for libraries")

# Make a version file containing the current version from git.
include (GetGitRevisionDescription)
git_describe (VERSION "--tags")
# If git_describe fails, use a dirty version
if (${VERSION} MATCHES -NOTFOUND)
  set (VERSION "v0.8.0-dirty")
  message (WARNING "Failed to retrieve UPM version with 'git describe' (using "
      "${VERSION}). Check that git is installed and this is a valid git repo.")
endif ()

message (STATUS "UPM Version ${VERSION}")

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

# Detect arch
include (TargetArch)
target_architecture (DETECTED_ARCH)
message (STATUS "Target arch is ${DETECTED_ARCH}")

#-march=native for ARM when not defined/forced
if (DETECTED_ARCH MATCHES "arm.*" AND NOT CMAKE_CXX_FLAGS MATCHES "-march")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# enable c++11 standards support unconditionally
include(CheckCXXCompilerFlag)
if (CMAKE_VERSION VERSION_LESS "3.1")
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if (COMPILER_SUPPORTS_CXX11)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif (COMPILER_SUPPORTS_CXX0X)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(FATAL_ERROR "A C++11 compliant compiler is required to build UPM.")
  endif()
else()
  # 3.1+ uses this generic method to enable c++11
  set (CMAKE_CXX_STANDARD 11)
  set (CXX_STANDARD_REQUIRED ON)
  set (CXX_EXTENSIONS OFF)
endif()

include(CheckCCompilerFlag)
if (CMAKE_VERSION VERSION_LESS "3.1")
  CHECK_C_COMPILER_FLAG("-std=c11" COMPILER_SUPPORTS_C11)
  if (COMPILER_SUPPORTS_C11)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
  else()
    message(FATAL_ERROR "A C11 compliant C compiler is required to build UPM.")
  endif()
else()
  # 3.1+ uses this generic method to enable c11
  set (CMAKE_C_STANDARD 11)
  set (C_STANDARD_REQUIRED ON)
  set (C_EXTENSIONS OFF)
endif()

# The doc target depends on each sensor target
#
# doc
#   ├──> libupm_sensor0
#   ├──> libupm_sensor1
#   ├──> libupm_sensor2
#   └──> libupm_sensor_n
#
# The pydoc target builds documentation with sphinx via inspection by loading
# each python module.  Those modules must include the CXX documentation via
# a monolithic swig file generated by doxy2swig
#
# pydoc
#  └──> _pyupm_sensor0_python2
#        ├──────> libupm_sensor0
#        └──────> doxy2swig
#
# The doxy2swig target is dependent upon the doc target IF BUILDDOC=ON,
# otherwise doxy2swig uses an empty file.  Doxy2swig also depends on each
# sensor target
#
# doxy2swig
#  ├──> BUILDDOC=ON───> doc
#  └──> libupm_sensor0
#
# The jsdoc target builds js documentation via yuidoc and only requires
# the doc target
#
# jsdoc ─> doc
#
if (BUILDDOC)
  # Add a target to generate API documentation with Doxygen
  find_package (Doxygen REQUIRED)
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

  # Check if Sphinx is installed and add target to generate API documentationa
  # Currently, the per-module documentation for python is generated from the
  # python2 modules.
  if(BUILDSWIGPYTHON)
    find_package (Sphinx REQUIRED)
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
  endif(BUILDSWIGPYTHON)

  # Check if Yuidoc is installed and add target for API documentation
  if(BUILDSWIGNODE)
    find_package(Yuidoc REQUIRED)
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
  endif(BUILDSWIGNODE)
endif (BUILDDOC)

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
  message (STATUS "Package arch is ${TARGET_ARCH}")

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
  message (STATUS "RPM packaging enabled for ${DETECTED_ARCH}")
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

# Python interp is previously found if BUILDTESTS=ON
if (BUILDTESTS)
  enable_testing ()
  add_subdirectory (tests)
endif()

# Install C headers
install(DIRECTORY include/ DESTINATION include/upm
    FILES_MATCHING PATTERN "*.h")
