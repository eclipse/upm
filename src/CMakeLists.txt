macro (file_to_list readfile outlist)
  FILE(READ "${readfile}" contents)
  STRING(REGEX REPLACE ";" "\\\\;" contents "${contents}")
  STRING(REGEX REPLACE "\n" ";" contents "${contents}")
  set("${outlist}" "${contents}" )
endmacro(file_to_list)

file_to_list ("javaswig_blacklist" JAVASWIG_BLACKLIST)
file_to_list ("pythonswig_blacklist" PYTHONSWIG_BLACKLIST)
file_to_list ("nodeswig_blacklist" NODESWIG_BLACKLIST)

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro(subdirlist)

macro (upm_create_install_pkgconfig generated_file install_location)
  configure_file (${PROJECT_SOURCE_DIR}/src/pkgconfig.in
    ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} @ONLY)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} DESTINATION ${install_location})
endmacro(upm_create_install_pkgconfig)

# Provides a means to call target_link_libraries AND the necessary
# swig_link_libaries.  This iterates over all additional parameters AFTER
# the target name calling target_link_libraries/swig_link_libraries
# as necessary accordingly
macro (upm_target_link_libraries target_name)
  # For each additional argument to upm_target_link_libraries...
  foreach(_library ${ARGN})
    # Add the dependency
    target_link_libraries(${target_name} ${_library})

    # Call the swig link library method for each library
    if (BUILDSWIGNODE)
      swig_link_libraries (jsupm_${libname} ${_library})
    endif()
    # Build python2 AND/OR python3 modules
    if (BUILDSWIGPYTHON)
      if (PYTHON2LIBS_FOUND)
        swig_link_libraries (pyupm_${libname}-python2 ${_library})
      endif (PYTHON2LIBS_FOUND)
      if (PYTHON3LIBS_FOUND)
        swig_link_libraries (pyupm_${libname}-python3 ${_library})
      endif (PYTHON3LIBS_FOUND)
    endif()
    if (BUILDSWIGJAVA)
      swig_link_libraries (javaupm_${libname} ${_library})
    endif()
  endforeach(_library ${ARGN})
endmacro (upm_target_link_libraries target_name)

# Selectively do not emit warnings from the SWIG-generated wrappers
set (disabled_flags -Wno-delete-non-virtual-dtor -Wno-unused-function)

# Create a single swig target for python
macro(_upm_swig_python)
  include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

  set_source_files_properties (pyupm_${libname}.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (pyupm_${libname}.i PROPERTIES SWIG_FLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/..")

  # Set current python binary dir
  set (CMAKE_CURRENT_PYTHON_BINARY_DIR
      ${CMAKE_CURRENT_BINARY_DIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

  # Make sure swig places wrapper/.py collateral under python2/3 directory
  # Save the original BINARY_DIR, set a new BINARY_DIR, then restore at
  # the end of this macro. This works for cmake > 3 and works AROUND a bug in
  # cmake < 3 (#13173,#13772,#14459).
  set(CMAKE_CURRENT_BINARY_DIR_SAVED ${CMAKE_CURRENT_BINARY_DIR})
  set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_PYTHON_BINARY_DIR})

  # Swig module name (example: pyupm_a110x-python2)
  set (python_wrapper_name pyupm_${libname}-python${PYTHON_VERSION_MAJOR})
  swig_add_module (${python_wrapper_name} python pyupm_${libname}.i)
  # Get target library name (example _pyupm_a110x-python2)
  set (python_wrapper_target ${SWIG_MODULE_${python_wrapper_name}_REAL_NAME})

  swig_link_libraries (${python_wrapper_name} ${PYTHON_LIBRARIES} ${MRAA_LIBRARIES} ${libname})
  target_include_directories (${python_wrapper_target}
      PUBLIC
      "${PYTHON_INCLUDE_PATH}"
      "${PYTHON_INCLUDE_DIRS}")

  # Turn off flags for wrapper
  foreach(flag ${disabled_flags})
    compiler_flag_supported(CXX is_supported ${flag})
    if (is_supported)
      target_compile_options(${python_wrapper_target}
        PRIVATE "${flag}")
    endif(is_supported)
  endforeach(flag ${disabled_flags})

  # Add C++ comments to ALL python modules (requires doc build)
  if (BUILDDOC)
    # Python module depends on doxy2swig .i file generated from the monolithic doxygen xml file
    add_dependencies(${python_wrapper_target} pyupm_doxy2swig )

    # The pydoc target depends on EACH python module
    add_dependencies(pydoc ${python_wrapper_target})
  endif (BUILDDOC)

  # Python collateral names will be the same for python2/3 w/different library dirs
  set_target_properties (${python_wrapper_target} PROPERTIES
      OUTPUT_NAME _pyupm_${libname}
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_PYTHON_BINARY_DIR})

  # Install .py's to python packages directory/upm
  install (FILES ${swig_extra_generated_files}
      DESTINATION ${PYTHON_PACKAGES_PATH}/upm
      COMPONENT ${libname})

  # Install python wrapper module library
  install (TARGETS ${python_wrapper_target}
      DESTINATION ${PYTHON_PACKAGES_PATH}/upm)

  # Restore CMAKE_CURRENT_BINARY_DIR
  set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR_SAVED})
endmacro(_upm_swig_python)

# Generate both python2 and python3 modules if possible
macro(upm_swig_python)
  # Skip if the libname is in the blacklist
  if (NOT ";${PYTHONSWIG_BLACKLIST};" MATCHES ";${libname};")
    # Generate python2 module if python2 libs are available
    if (PYTHON2LIBS_FOUND)
      set(PYTHON_INCLUDE_DIRS ${PYTHON2_INCLUDE_DIRS})
      set(PYTHON_INCLUDE_PATH ${PYTHON2_INCLUDE_PATH})
      set(PYTHON_LIBRARIES ${PYTHON2_LIBRARIES})
      set(PYTHON_VERSION_MAJOR ${PYTHON2_VERSION_MAJOR})
      set(PYTHON_VERSION_MINOR ${PYTHON2_VERSION_MINOR})
      set(PYTHON_PACKAGES_PATH ${PYTHON2_PACKAGES_PATH})
      _upm_swig_python()
    endif (PYTHON2LIBS_FOUND)

    # Generate python3 module if python3 libs are available
    if (PYTHON3LIBS_FOUND)
      set(PYTHON_INCLUDE_DIRS ${PYTHON3_INCLUDE_DIRS})
      set(PYTHON_INCLUDE_PATH ${PYTHON3_INCLUDE_PATH})
      set(PYTHON_LIBRARIES ${PYTHON3_LIBRARIES})
      set(PYTHON_VERSION_MAJOR ${PYTHON3_VERSION_MAJOR})
      set(PYTHON_VERSION_MINOR ${PYTHON3_VERSION_MINOR})
      set(PYTHON_PACKAGES_PATH ${PYTHON3_PACKAGES_PATH})
      _upm_swig_python()
    endif (PYTHON3LIBS_FOUND)
  endif (NOT ";${PYTHONSWIG_BLACKLIST};" MATCHES ";${libname};")
endmacro(upm_swig_python)

macro(upm_swig_node)
  # Skip if the libname is in the blacklist
  if (NOT ";${NODESWIG_BLACKLIST};" MATCHES ";${libname};")
    # SWIG treats SWIG_FLAGS as a list and not a string so semicolon seperation is
    # required. This hardcodes V8_VERSION to be <10 but I assume that's not going
    # to be a problem for a little while! SWIG uses a padded SWIG_V8 version which
    # we hack together from our findnode module.
    set (V8_VERSION_HEX 0x0${V8_VERSION_MAJOR}${V8_VERSION_MINOR}${V8_VERSION_PATCH})
    string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
    while (V8_VERSION_HEX_length LESS 8)
      set (V8_VERSION_HEX "${V8_VERSION_HEX}0")
      string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
    endwhile ()

    include_directories (
      ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    set_property (SOURCE jsupm_${libname}.i PROPERTY SWIG_FLAGS "-node" "-DV8_VERSION=${V8_VERSION_HEX}")
    set_source_files_properties (jsupm_${libname}.i PROPERTIES CPLUSPLUS ON)
    swig_add_module (jsupm_${libname} javascript jsupm_${libname}.i)
    swig_link_libraries (jsupm_${libname} ${MRAA_LIBRARIES} ${NODE_LIBRARIES} ${libname})
    target_include_directories ( ${SWIG_MODULE_jsupm_${libname}_REAL_NAME}
      PUBLIC
      "${NODE_INCLUDE_DIRS}"
    )

    # Turn off flags for wrapper
    foreach(flag ${disabled_flags})
      compiler_flag_supported(CXX is_supported ${flag})
      if (is_supported)
        target_compile_options(${SWIG_MODULE_jsupm_${libname}_REAL_NAME}
          PRIVATE "${flag}")
      endif(is_supported)
    endforeach(flag ${disabled_flags})

    set_target_properties (jsupm_${libname} PROPERTIES
      COMPILE_FLAGS "-DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=${V8_VERSION_HEX}"
      PREFIX ""
      SUFFIX ".node"
    )
    if (${V8_VERSION_MAJOR} GREATER 3)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7")
          message(FATAL_ERROR "FATAL_ERROR: GCC 4.7 or above is required to compile jsupm_${libname} ")
        endif()
      endif()

    endif()

    createpackagejson(${libname})

    install (TARGETS jsupm_${libname} DESTINATION ${NODE_MODULE_INSTALL_PATH})
  endif (NOT ";${NODESWIG_BLACKLIST};" MATCHES ";${libname};")
endmacro(upm_swig_node)

macro(upm_swig_java)
  # Skip if the libname is in the blacklist
  if (NOT ";${JAVASWIG_BLACKLIST};" MATCHES ";${libname};")
    FIND_PACKAGE (JNI REQUIRED)
    pkg_check_modules (MRAAJAVA REQUIRED mraajava>=0.8.0)

    include_directories (
      ${JAVA_INCLUDE_PATH}
      ${JAVA_INCLUDE_PATH2}
      ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    set_source_files_properties (javaupm_${libname}.i PROPERTIES CPLUSPLUS ON)
    set_source_files_properties (javaupm_${libname}.i PROPERTIES SWIG_FLAGS ";-package;upm_${libname};-I${CMAKE_BINARY_DIR}/src")
    swig_add_module (javaupm_${libname} java javaupm_${libname}.i)
    swig_link_libraries (javaupm_${libname} ${MRAAJAVA_LIBRARIES} ${MRAA_LIBRARIES} ${JAVA_LIBRARIES} ${libname})
    target_include_directories ( ${SWIG_MODULE_javaupm_${libname}_REAL_NAME}
      PUBLIC
      "${JAVA_INCLUDE_DIRS}"
      "${JAVA_INCLUDE_PATH}"
     )
    set_target_properties (javaupm_${libname} PROPERTIES
      COMPILE_FLAGS "-fpermissive -DJAVACALLBACK"
      PREFIX "lib"
      SUFFIX ".so"
    )

    # Turn off flags for wrapper
    foreach(flag ${disabled_flags})
      compiler_flag_supported(CXX is_supported ${flag})
      if (is_supported)
        target_compile_options(${SWIG_MODULE_javaupm_${libname}_REAL_NAME}
          PRIVATE "${flag}")
      endif(is_supported)
    endforeach(flag ${disabled_flags})

    install (TARGETS javaupm_${libname} LIBRARY DESTINATION ${LIB_INSTALL_DIR})
    # Java jar files always need to go under lib/java, regardless of
    # architecture.
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}.jar
        DESTINATION lib/java)

    if (NOT DEFINED $ENV{JAVA_HOME_NATIVE})
        set (JAVAC $ENV{JAVA_HOME}/bin/javac)
        set (JAR $ENV{JAVA_HOME}/bin/jar)
    else ()
        set (JAVAC $ENV{JAVA_HOME_NATIVE}/bin/javac)
        set (JAR $ENV{JAVA_HOME_NATIVE}/bin/jar)
    endif ()

    add_custom_command (TARGET javaupm_${libname}
        POST_BUILD
        COMMAND cmake -E echo "Compiling java.."
        COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}
        COMMAND ${JAVAC} *.java -d ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND cmake -E echo "Creating jar"
        COMMAND ${JAR} cvf upm_${libname}.jar upm_${libname}
    )

    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/../pom.xml.in
        ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.pom)
  endif (NOT ";${JAVASWIG_BLACKLIST};" MATCHES ";${libname};")
endmacro(upm_swig_java)

macro(upm_doxygen)
  if (DOXYGEN_FOUND)
    set (CMAKE_SWIG_FLAGS -DDOXYGEN=${DOXYGEN_FOUND})
    add_dependencies (doc ${libname})
  endif ()
endmacro(upm_doxygen)

if (BUILDSWIGNODE)
  if(SWIG_VERSION VERSION_LESS 3.0.5 AND NODE_VERSION_STRING VERSION_GREATER 0.12)
    message(WARNING "WARNING - SWIG 3.0.5+ required for building with nodejs 0.12. Current version is ${SWIG_VERSION}")
  endif()
  find_path (NODE_ROOT_DIR "include/node/node.h")
  set (NODE_INCLUDE_DIRS
    ${NODE_ROOT_DIR}/include/src
    ${NODE_ROOT_DIR}/include/node
    ${NODE_ROOT_DIR}/include/deps/v8/include
    ${NODE_ROOT_DIR}/include/deps/uv/include
  )
  macro(createpackagejson)
    configure_file (${PROJECT_SOURCE_DIR}/src/package.json.in ${CMAKE_CURRENT_BINARY_DIR}/package.json @ONLY)
    # If a CMAKE_INSTALL_PREFIX has NOT been provided, set NODE_MODULE_INSTALL_PATH
    # base on the NODE_ROOT_DIR.
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set (NODE_MODULE_INSTALL_PATH ${NODE_ROOT_DIR}/lib/node_modules/jsupm_${libname}/)
    # If a CMAKE_INSTALL_PREFIX has been provided, set NODE_MODULE_INSTALL_PATH
    # relative to the provided install directory.
    else ()
      set (NODE_MODULE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib/node_modules/jsupm_${libname}/)
    endif ()
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/package.json
        DESTINATION ${NODE_MODULE_INSTALL_PATH} COMPONENT ${libname})
  endmacro(createpackagejson)
endif(BUILDSWIGNODE)

# Process C/C++ sensor modules
# This function pre-processes sensor library input and hands off the
# necessary global variables to upm_module_init for library creation,
# documenation, swigging, etc...
function (UPM_MIXED_MODULE_INIT)
  # CPP_WRAPS_C -> Set to have CPP library link to C library
  # DESCRIPTION -> Library description string
  # CPP_HDR -> List of CPP header files
  # CPP_SRC -> List of CPP source files
  # C_HDR -> List of C header files
  # C_SRC -> List of C source files
  # FTI_SRC -> List of C FTI source files
  # REQUIRES -> List requires libraries for pkg-config
  set (options CPP_WRAPS_C)
  set (oneValueArgs NAME DESCRIPTION)
  set (multiValueArgs CPP_HDR CPP_SRC C_HDR C_SRC FTI_SRC FTI_HDR REQUIRES)
  # Parse function parameters
  cmake_parse_arguments(UPM_MIXED_MODULE_INIT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  # Set the description
  set (libdescription ${UPM_MIXED_MODULE_INIT_DESCRIPTION})

  # Always build C libs first
  if (UPM_MIXED_MODULE_INIT_C_SRC)
    set (libname ${UPM_MIXED_MODULE_INIT_NAME})
    # Set the src and hpp variables for upm_module_init
    set (module_src ${UPM_MIXED_MODULE_INIT_C_SRC})
    set (module_hpp ${UPM_MIXED_MODULE_INIT_C_HDR})

    # Create the reqlibname list
    string(REPLACE ";" " " reqlibname "${UPM_MIXED_MODULE_INIT_REQUIRES}")
    # Append upmc-utilities to the reqlibs (but not on upm-utilities itself)
    if (NOT ${libname} MATCHES "utilities")
      set (reqlibname "${reqlibname} upmc-utilities")
    endif()

    # If building FTI, and FTI src exists, add it in
    if (BUILDFTI AND UPM_MIXED_MODULE_INIT_FTI_SRC)
      #set (module_src ${UPM_MIXED_MODULE_INIT_C_SRC} ${UPM_MIXED_MODULE_INIT_FTI_SRC})
      list (APPEND module_src ${UPM_MIXED_MODULE_INIT_FTI_SRC})
    endif (BUILDFTI AND UPM_MIXED_MODULE_INIT_FTI_SRC)

    # Add include directories for C
    include_directories (${UPM_COMMON_HEADER_DIRS}
                         ${CMAKE_SOURCE_DIR}/src/utilities)

    # Set a flag to tell upm_module_init that it's building a C library
    set (IS_C_LIBRARY TRUE)
    upm_module_init()

    # add upmc-utilities as a dependancy to all C libs (but NOT to the
    # utilities lib itself)
    if (NOT ${libname} MATCHES "utilities-c")
      target_link_libraries (${libname} utilities-c)
    endif()

    ## "export" the logical C lib target for the calling module's
    ## CMakeLists.txt
    set (libnamec ${libname} PARENT_SCOPE)

  endif (UPM_MIXED_MODULE_INIT_C_SRC)

  # Build C++ if enabled AND C++ headers exist
  if (BUILDCPP AND UPM_MIXED_MODULE_INIT_CPP_HDR)
    # Set the src and hpp variables for upm_module_init
    set (module_src ${UPM_MIXED_MODULE_INIT_CPP_SRC})
    set (module_hpp ${UPM_MIXED_MODULE_INIT_CPP_HDR})

    # Create the reqlibname list
    string(REPLACE ";" " " reqlibname "${UPM_MIXED_MODULE_INIT_REQUIRES}")

    # Reset the libname (upm_module_init can change it)
    set (libname ${UPM_MIXED_MODULE_INIT_NAME})
    unset (IS_C_LIBRARY)
    upm_module_init()

    # If the C++ wraps the C target, add the C target as a dependency
    if (UPM_MIXED_MODULE_INIT_CPP_WRAPS_C)
      target_link_libraries(${libname} ${libname}-c)
    endif (UPM_MIXED_MODULE_INIT_CPP_WRAPS_C)

    ## "export" the logical C++ lib target for the calling module's
    ## CMakeLists.txt
    set (libnamecxx ${libname} PARENT_SCOPE)

  endif (BUILDCPP AND UPM_MIXED_MODULE_INIT_CPP_HDR)

endfunction (UPM_MIXED_MODULE_INIT)

macro(upm_module_init)
  set (basename ${libname})

  # If this is a C library, handle different collateral naming
  if (IS_C_LIBRARY)
    set (libname ${libname}-c)
    set (libprefix upmc-)
    set (pcname upmc-${basename}.pc)
  else ()
    set (libprefix upm-)
    set (pcname upm-${basename}.pc)
  endif (IS_C_LIBRARY)

  link_directories (${MRAA_LIBDIR})
  add_library (${libname} SHARED ${module_src} ${module_hpp})
  foreach (linkflag ${ARGN})
    target_link_libraries (${libname} ${linkflag})
  endforeach ()
  include_directories (${MRAA_INCLUDE_DIRS} . ..)
  target_link_libraries (${libname} ${MRAA_LIBRARIES})
  set_target_properties(
    ${libname}
    PROPERTIES PREFIX lib${libprefix}
    OUTPUT_NAME ${basename}
    SOVERSION ${upm_VERSION_MAJOR}
    VERSION ${upm_VERSION_STRING}
  )
  upm_create_install_pkgconfig (${pcname} ${LIB_INSTALL_DIR}/pkgconfig)

  # Don't SWIG C
  if (NOT IS_C_LIBRARY)
    if (BUILDSWIGPYTHON)
      upm_swig_python()
    endif (BUILDSWIGPYTHON)
    if (BUILDSWIGNODE)
      upm_swig_node()
    endif (BUILDSWIGNODE)
    if (BUILDSWIGJAVA)
      upm_swig_java()
    endif (BUILDSWIGJAVA)
  endif (NOT IS_C_LIBRARY)

  # Skip doxygen run on C (for now)
  if (BUILDDOC AND NOT IS_C_LIBRARY)
    upm_doxygen()
  endif()

  install (TARGETS ${libname} DESTINATION ${LIB_INSTALL_DIR})
  install (FILES ${module_hpp} DESTINATION include/upm COMPONENT ${libname})

  if (IPK)
    cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
    set(CPACK_COMPONENT_${libname}_DESCRIPTION "${libdescription}")
  endif()
endmacro(upm_module_init)

# Top-level module init
if (BUILDSWIGPYTHON)
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "# UPM python modules")
  if(PYTHON2LIBS_FOUND)
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON2_PACKAGES_PATH}/upm
        COMPONENT python2)
  endif(PYTHON2LIBS_FOUND)
  if(PYTHON3LIBS_FOUND)
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON3_PACKAGES_PATH}/upm
        COMPONENT python3)
  endif(PYTHON3LIBS_FOUND)
endif (BUILDSWIGPYTHON)

# Generate python module documentation from doxygen collateral
if (BUILDDOC AND BUILDSWIGPYTHON)
  # doxy2swig the doxygen output
  add_custom_target (pyupm_doxy2swig ALL
    COMMAND ${PYTHON2_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doxy2swig.py
      ${CMAKE_BINARY_DIR}/xml/index.xml
      ${CMAKE_CURRENT_BINARY_DIR}/pyupm_doxy2swig.i
      DEPENDS doc)
# BUILDDOC not set but still building python modules, generate an empty
# pyupm_doxy2swig.i file (overwriting if necessary)
elseif (BUILDSWIGPYTHON)
  message (STATUS "BUILDDOC=${BUILDDOC}, python modules will not contain cxx documentation")
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/pyupm_doxy2swig.i "// Empty doxy2swig stub")
endif (BUILDDOC AND BUILDSWIGPYTHON)

if (MODULE_LIST)
  set(SUBDIRS ${MODULE_LIST})
  set(SUBDIRS ${SUBDIRS} upm)
else()
  subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
endif()
foreach(subdir ${SUBDIRS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory(${subdir})
    endif()
endforeach()
