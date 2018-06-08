# Selectively disable certain CXX warnings for SWIG wrappers
# SWIG wrappers emit -Wdelete-non-virtual-dtor and -Wunused-function warnings
# Unfortunaly this quick fix for issue #518, adds a .c file to the
# java swig wrapper which then failes on CXX compiler warnings.  For
# now, turn off warnings as errors for all the swig wrappers.
set (SWIG_CXX_DISABLE_WARNINGS -Wno-error
                               -Wno-delete-non-virtual-dtor
                               -Wno-unused-function
                               -Wno-maybe-uninitialized
                               -Wno-strict-aliasing)

# If building under android, make sure swig gets an ANDROID flag
if (ANDROID)
    list (APPEND CMAKE_SWIG_FLAGS -DANDROID)
endif (ANDROID)

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
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
      COMPONENT ${CMAKE_PROJECT_NAME}
      DESTINATION ${install_location})
endmacro(upm_create_install_pkgconfig)

# Given a target name, return all in-project targets ALREADY defined (ie, this
# does not comprehend targets that have yet to be added.
function (_get_dependency_targets target return_variable)
    # Clear the variable
    set(${return_variable} "")

    # Get the LINK_LIBRARIES property of the target
    get_target_property(_dep_libs ${target} LINK_LIBRARIES)

    # Remove this target from the _dep_libs (if it exists)
    list(REMOVE_ITEM _dep_libs ${target})

    # Check if any of the LINK_LIBRARIES are targets in this project,
    # if so, add them to ${return_variable}
    foreach (deplib ${_dep_libs})
        # Only add targets that exist
        if (TARGET ${deplib})
            list(APPEND ${return_variable} ${deplib})
        # Special case - mraa is NOT in this project, but add it to the
        # reqlibname list (used in various items like pom files, .pc files, etc
        elseif (${deplib} MATCHES mraa)
            list(APPEND ${return_variable} mraa)
        endif (TARGET ${deplib})
    endforeach (deplib ${_dep_libs})
    # Uniquify
    list(REMOVE_DUPLICATES ${return_variable})
    # Make the list available outside this function
    set (${return_variable} ${${return_variable}} PARENT_SCOPE)
endfunction (_get_dependency_targets target)

# This function iterates over all provided target names and
# writes a .pc file to each build directory
function (_gen_pkg_config_per_target LABEL)
  list(LENGTH ARGN n_targets)
  message(STATUS "Generating pkg-config files for ${n_targets} ${LABEL} libraries...")
  # Iterate over the targets an build package config (.pc) files
  foreach (target ${ARGN})
    if (TARGET ${target})
      get_target_property(libname ${target} PKG_CONFIG_NAME)
      get_target_property(libdescription ${target} PKG_CONFIG_DESCRIPTION)
      get_target_property(reqlibname ${target} PKG_EXT_REQ_LIBS)
      get_target_property(BINARY_DIR ${target} TARGET_BINARY_DIR)

      # Get a list of in-project dependency targets for this target
      _get_dependency_targets(${target} DEPLIBS)

      # Check if any of the LINK_LIBRARIES are targets in this project,
      # if so, add them to reqlibname
      foreach (deplib ${DEPLIBS})
        # Get the actual library name (which equates to it's .pc filename,
        # example: target dfrph = <upm-dfrph>.pc
        if (TARGET ${deplib})
          get_target_property(deplibname ${deplib} PKG_CONFIG_NAME)
          list(APPEND reqlibname ${deplibname})
        elseif (${deplib} MATCHES mraa)
          list(APPEND reqlibname mraa)
        endif (TARGET ${deplib})
      endforeach (deplib ${DEPLIBS})
      # Make sure no dups exist
      separate_arguments(reqlibname)
      list(REMOVE_DUPLICATES reqlibname)
      # Switch to comma separated Requires: field (per pkg-config docs)
      string(REPLACE ";" "," reqlibname "${reqlibname}")

      # Create the .pc file
      upm_create_install_pkgconfig (${libname}.pc ${LIB_INSTALL_DIR}/pkgconfig)
    endif (TARGET ${target})
  endforeach (target ${ARGN})
  message(STATUS "Generating pkg-config files for ${n_targets} ${LABEL} libraries... Complete")
endfunction (_gen_pkg_config_per_target)

# This function iterates over all provided target names and
# writes a pom file to each build directory
function (_gen_pom_per_target)
  # Remove targets listed in the JAVA blacklist from pom file generation
  list(REMOVE_ITEM ARGN JAVASWIG_BLACKLIST)

  list(LENGTH ARGN n_targets)
  message(STATUS "Generating pom files for ${n_targets} CXX libraries...")
  # Iterate over the targets an build pom files
  foreach (target ${ARGN})
    if (TARGET ${target})
      get_target_property(libname ${target} PKG_CONFIG_NAME)
      get_target_property(libdescription ${target} PKG_CONFIG_DESCRIPTION)
      get_target_property(reqlibname ${target} PKG_EXT_REQ_LIBS)
      get_target_property(BINARY_DIR ${target} TARGET_BINARY_DIR)

      # Get a list of in-project dependency targets for this target
      _get_dependency_targets(${target} DEPLIBS)

      # Default the packaging for java
      set (pom_packaging jar)
      # Default the groupid for java
      set (pom_base_groupId io.mraa)
      if (ANDROID)
        # Android overrides
        set (pom_packaging aar)
        set (pom_base_groupId io.mraa.at)
      endif (ANDROID)

      # Top-level pom groupId
      set (pom_groupId ${pom_base_groupId}.upm)

      # Template for a single dependency
      set (pom_dep_template "
    <dependency>
       <groupId>\@pom_dep_groupId\@</groupId>
       <artifactId>\@deplib\@</artifactId>
       <version>[\@pom_version_minimum\@, \)</version>
       <scope>compile</scope>
    </dependency>")

      # Iterate over the dependency targets, build an XML vector of dependencies
      set (pom_dependencies "")
      foreach (deplib ${DEPLIBS})
        unset(_tmp_pom_dep)
        # mraa is a special case
        if (${deplib} MATCHES mraa)
          set (pom_dep_groupId "${pom_base_groupId}")
          set (pom_version_minimum ${MRAA_MINIMUM})
          string(CONFIGURE ${pom_dep_template} _tmp_pom_dep @ONLY)
        # Only match CXX target dependencies
        elseif ("${UPM_TARGETS_CXX}" MATCHES ";${deplib};")
          set (deplib upm_${deplib})
          set (pom_dep_groupId "${pom_base_groupId}.upm")
          set (pom_version_minimum ${upm_VERSION_STRING})
          string(CONFIGURE ${pom_dep_template} _tmp_pom_dep @ONLY)
        endif (${deplib} MATCHES mraa)

        # Add this dependency to the list of pom dependencies
        set (pom_dependencies "${pom_dependencies}${_tmp_pom_dep}")
      endforeach (deplib ${DEPLIBS})

      # Separate the dependency elements
      STRING(REGEX REPLACE ";" "\\n;" pom_dependencies "${pom_dependencies}")
      # Configure the pom file
      configure_file (${CMAKE_SOURCE_DIR}/src/pom.xml.in
          ${BINARY_DIR}/upm_${target}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.pom)
    endif (TARGET ${target})
  endforeach (target ${ARGN})
  message(STATUS "Generating pom files for ${n_targets} CXX libraries... Complete")
endfunction (_gen_pom_per_target)

# Print out a quick summary of the UPM targets
function (_print_target_summary)
  list(LENGTH UPM_TARGETS_C N_C)
  list(LENGTH UPM_TARGETS_CXX N_CXX)
  list(LENGTH UPM_TARGETS_JAVA N_JAVA)
  list(LENGTH UPM_TARGETS_NODEJS N_NODEJS)
  list(LENGTH UPM_TARGETS_PYTHON2 N_PYTHON2)
  list(LENGTH UPM_TARGETS_PYTHON3 N_PYTHON3)

  message(STATUS "UPM target summary:")
  message(STATUS "\tFound C       libraries: ${N_C}")
  message(STATUS "\tFound CXX     libraries: ${N_CXX}")
  message(STATUS "\tFound JAVA    libraries: ${N_JAVA}")
  message(STATUS "\tFound NODEJS  libraries: ${N_NODEJS}")
  message(STATUS "\tFound PYTHON2 libraries: ${N_PYTHON2}")
  message(STATUS "\tFound PYTHON3 libraries: ${N_PYTHON3}")
endfunction (_print_target_summary)

function (_get_target_dependency_interface_include_dirs target varIncludeDirs)
  set (${varIncludeDirs} "")

  # For each LINK_LIBRARIES of target
  get_property(_targets TARGET ${target} PROPERTY LINK_LIBRARIES)
  foreach (_target ${_targets})
    # If the target currently exists, use its include directories
    if (TARGET ${_target})
      get_property(_tmp_prop_val TARGET ${_target} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
      list(APPEND ${varIncludeDirs} ${_tmp_prop_val})
    # It's possible that the target does NOT exist yet, attempt to build an include directory
    else ()
        # Guess at a src dir name
        file (TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/src/${_target}" _dirname)
        if (EXISTS ${_dirname})
            list(APPEND ${varIncludeDirs} ${_dirname})
        else ()
        endif (EXISTS ${_dirname})
    endif (TARGET ${_target})
  endforeach (_target ${_targets})
  set(${varIncludeDirs} "${${varIncludeDirs}}" PARENT_SCOPE)
endfunction (_get_target_dependency_interface_include_dirs target varIncludeDirs)

# This function decides which swig interface file to use for the
# CMAKE_CURRENT_SOURCE_DIR.  If no .i file exists, create one.
#
#   if ${filePrefix}_${libname}.i exists
#       return its full path
#   elif ${libname}.i exists
#       return its full path
#   else
#       create ${libname}.i via template and return its full path
#
function (_get_current_dot_i_file filePrefix varDotIFile)
  set(${varDotIFile} "${CMAKE_CURRENT_SOURCE_DIR}/${libname}.i")
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${filePrefix}_${libname}.i")
      set(${varDotIFile} "${CMAKE_CURRENT_SOURCE_DIR}/${filePrefix}_${libname}.i")
  endif()
  # If a SWIG .i file DOES NOT exist, make a simple one from the template
  if (NOT EXISTS "${${varDotIFile}}")
      # This file will exist OUTSIDE the source tree
      set(${varDotIFile} "${CMAKE_CURRENT_BINARY_DIR}/${libname}.i")

      # Create preprocessor include string for importing
      string(REGEX REPLACE ";" "\"\n#include \"" SWIG_HASH_INCLUDES "${module_h_hpp}")
      set(SWIG_HASH_INCLUDES "#include \"${SWIG_HASH_INCLUDES}\"")
      # And the SWIG import string
      string(REPLACE "#" "%" SWIG_PERCENT_INCLUDES "${SWIG_HASH_INCLUDES}")

      # Write the interface file
      configure_file (${PROJECT_SOURCE_DIR}/src/swigme.i.in "${${varDotIFile}}" @ONLY)
  endif()

  set(${varDotIFile} "${${varDotIFile}}" PARENT_SCOPE)
endfunction()

# Create a single swig target for python
# This is a macro since the function which calls it (upm_swig_python) depends
# on values set inside this macro
macro(_upm_swig_python)
  # Transitive headers don't appear to get picked up by swig_add_module call for
  # wrapper generation.  Get them manually and add them here.
  _get_target_dependency_interface_include_dirs(${libname} DEPEND_DIRS)

  include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                       ${CMAKE_CURRENT_BINARY_DIR}/..
                       ${DEPEND_DIRS})

  # Decide between ${libname}.i or a language-specific .i
  _get_current_dot_i_file(pyupm SWIG_CURRENT_DOT_I_FILE)
  # If this module is using ${libname}.i, provide a module name for UseSWIG AND SWIG_FLAGS
  if (${SWIG_CURRENT_DOT_I_FILE} MATCHES ${libname}.i)
    set_source_files_properties(${SWIG_CURRENT_DOT_I_FILE} PROPERTIES
        SWIG_MODULE_NAME pyupm_${libname}
        SWIG_FLAGS ";-module;pyupm_${libname};")
  endif()

  # Tell UseSWIG that this is CXX
  set_property(SOURCE ${SWIG_CURRENT_DOT_I_FILE} PROPERTY CPLUSPLUS ON)

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
  if (CMAKE_VERSION VERSION_LESS "3.8")
      swig_add_module (${python_wrapper_name} python ${SWIG_CURRENT_DOT_I_FILE})
  else ()
      swig_add_library (${python_wrapper_name} LANGUAGE python SOURCES ${SWIG_CURRENT_DOT_I_FILE})
  endif ()
  # Get target library name (example _pyupm_a110x-python2)
  set (python_wrapper_target ${SWIG_MODULE_${python_wrapper_name}_REAL_NAME})
  add_dependencies(${python_wrapper_target} ${libname})

  swig_link_libraries (${python_wrapper_name} ${PYTHON_LIBRARIES} ${libname} -Wl,--unresolved-symbols=report-all)
  target_include_directories (${python_wrapper_target}
      PUBLIC
      "${PYTHON_INCLUDE_PATH}"
      "${PYTHON_INCLUDE_DIRS}"
      ${DEPEND_DIRS})

  # Turn off flags for wrapper
  foreach(flag ${SWIG_CXX_DISABLE_WARNINGS})
    compiler_flag_supported(CXX is_supported ${flag})
    if (is_supported)
      target_compile_options(${python_wrapper_target}
        PRIVATE "${flag}")
    endif(is_supported)
  endforeach(flag ${SWIG_CXX_DISABLE_WARNINGS})

  # Add C++ comments to ALL python modules (requires doc build)
  if (BUILDDOC)
    # The python documentation generated with Sphinx only uses python2
    if (PYTHON_VERSION_MAJOR VERSION_EQUAL 2)
      # Python module depends on doxy2swig .i file generated from the monolithic doxygen xml file
      add_dependencies(${python_wrapper_target} pyupm_doxy2swig)
    endif()
  endif (BUILDDOC)

  # Python collateral names will be the same for python2/3 w/different library dirs
  set_target_properties (${python_wrapper_target} PROPERTIES
      OUTPUT_NAME _pyupm_${libname}
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_PYTHON_BINARY_DIR})

  # Install .py's to python packages directory/upm
  install (FILES ${swig_extra_generated_files}
      DESTINATION ${PYTHON_PACKAGES_PATH}/upm
      COMPONENT ${CMAKE_PROJECT_NAME}-python${PYTHON_VERSION_MAJOR})

  # Install python wrapper module library
  install (TARGETS ${python_wrapper_target}
      DESTINATION ${PYTHON_PACKAGES_PATH}/upm
      COMPONENT ${CMAKE_PROJECT_NAME}-python${PYTHON_VERSION_MAJOR})

  # Restore CMAKE_CURRENT_BINARY_DIR
  set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR_SAVED})
endmacro(_upm_swig_python)

# Generate both python2 and python3 modules if possible
function(upm_swig_python)
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

      # Keep track of all Python2 targets
      set(UPM_TARGETS_PYTHON2 ${UPM_TARGETS_PYTHON2} ${python_wrapper_target}
          CACHE INTERNAL "List of Python target names")
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

      # Keep track of all Python3 targets
      set(UPM_TARGETS_PYTHON3 ${UPM_TARGETS_PYTHON3} ${python_wrapper_target}
          CACHE INTERNAL "List of Python target names")
    endif (PYTHON3LIBS_FOUND)
  endif (NOT ";${PYTHONSWIG_BLACKLIST};" MATCHES ";${libname};")
endfunction(upm_swig_python)

function(upm_swig_node)
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

    # Transitive headers don't appear to get picked up by swig_add_module call for
    # wrapper generation.  Get them manually and add them here.
    _get_target_dependency_interface_include_dirs(${libname} DEPEND_DIRS)

    include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                         ${CMAKE_CURRENT_BINARY_DIR}/..
                         ${DEPEND_DIRS})

    # Decide between ${libname}.i or a language-specific .i
    _get_current_dot_i_file(jsupm SWIG_CURRENT_DOT_I_FILE)
    # If this module is using ${libname}.i, provide a module name for UseSWIG AND SWIG_FLAGS
    if (${SWIG_CURRENT_DOT_I_FILE} MATCHES ${libname}.i)
      set_source_files_properties(${SWIG_CURRENT_DOT_I_FILE} PROPERTIES
          SWIG_MODULE_NAME jsupm_${libname}
          SWIG_FLAGS ";-module;jsupm_${libname};")
    endif()

    # Tell UseSWIG that this is CXX
    set_property(SOURCE ${SWIG_CURRENT_DOT_I_FILE} PROPERTY CPLUSPLUS ON)
    # Append additional flags for NodeJS
    set_property(SOURCE ${SWIG_CURRENT_DOT_I_FILE} APPEND PROPERTY
      SWIG_FLAGS ";-node;-DV8_VERSION=${V8_VERSION_HEX}")

    if (CMAKE_VERSION VERSION_LESS "3.8")
      swig_add_module (jsupm_${libname} javascript ${SWIG_CURRENT_DOT_I_FILE})
    else ()
      swig_add_library (jsupm_${libname} LANGUAGE javascript SOURCES ${SWIG_CURRENT_DOT_I_FILE})
    endif ()
    add_dependencies(jsupm_${libname} ${libname})
    swig_link_libraries (jsupm_${libname} ${NODE_LIBRARIES} ${libname})
    target_include_directories ( ${SWIG_MODULE_jsupm_${libname}_REAL_NAME}
      PUBLIC
      "${NODEJS_INCLUDE_DIRS}"
      ${DEPEND_DIRS}
    )

    # Turn off flags for wrapper
    foreach(flag ${SWIG_CXX_DISABLE_WARNINGS})
      compiler_flag_supported(CXX is_supported ${flag})
      if (is_supported)
        target_compile_options(${SWIG_MODULE_jsupm_${libname}_REAL_NAME}
          PRIVATE "${flag}")
      endif(is_supported)
    endforeach(flag ${SWIG_CXX_DISABLE_WARNINGS})

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

    if (NPM)
      add_custom_command (TARGET jsupm_${libname} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}JAVASCRIPT_wrap.cxx
                                                           ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname})
    endif ()

    install (TARGETS jsupm_${libname}
        DESTINATION ${NODE_MODULE_INSTALL_PATH}
      COMPONENT ${CMAKE_PROJECT_NAME}-nodejs)
  endif (NOT ";${NODESWIG_BLACKLIST};" MATCHES ";${libname};")

  # Keep track of all NODE targets
  set(UPM_TARGETS_NODEJS ${UPM_TARGETS_NODEJS} jsupm_${libname} CACHE INTERNAL "List of NodeJs target names")
endfunction(upm_swig_node)

function(upm_swig_java)
  # Skip if the libname is in the blacklist
  if (NOT ";${JAVASWIG_BLACKLIST};" MATCHES ";${libname};")
    # Transitive headers don't appear to get picked up by swig_add_module call for
    # wrapper generation.  Get them manually and add them here.
    _get_target_dependency_interface_include_dirs(${libname} DEPEND_DIRS)

    include_directories (${JAVA_INCLUDE_PATH}
                         ${JAVA_INCLUDE_PATH2}
                         ${CMAKE_CURRENT_SOURCE_DIR}
                         ${CMAKE_CURRENT_BINARY_DIR}/..
                         ${DEPEND_DIRS})

    # Decide between ${libname}.i or a language-specific .i
    _get_current_dot_i_file(javaupm SWIG_CURRENT_DOT_I_FILE)
    # If this module is using ${libname}.i, provide a module name for UseSWIG AND SWIG_FLAGS
    if (${SWIG_CURRENT_DOT_I_FILE} MATCHES ${libname}.i)
      set_source_files_properties(${SWIG_CURRENT_DOT_I_FILE} PROPERTIES
          SWIG_MODULE_NAME javaupm_${libname}
          SWIG_FLAGS ";-module;javaupm_${libname};")
    endif()

    # Tell UseSWIG that this is CXX
    set_property(SOURCE ${SWIG_CURRENT_DOT_I_FILE} PROPERTY CPLUSPLUS ON)

    # Set a -package as well (which is slightly different than the module name,
    # module: javaupm_a110x vs package: upm_a110x
    set_property(SOURCE ${SWIG_CURRENT_DOT_I_FILE} APPEND PROPERTY
      SWIG_FLAGS ";-package;upm_${libname}")

    if (CMAKE_VERSION VERSION_LESS "3.8")
      swig_add_module (javaupm_${libname} java ${SWIG_CURRENT_DOT_I_FILE})
    else ()
      swig_add_library (javaupm_${libname} LANGUAGE java SOURCES ${SWIG_CURRENT_DOT_I_FILE})
    endif ()

    # If the C++ target depends on C++ interfaces, make the JAVA target
    # depend on the JAVA interfaces
    if ("${_c_cxx_dependency_list}" MATCHES interfaces)
      add_dependencies(javaupm_${libname} javaupm_interfaces)
      # If this target depends on interfaces, include the java interfaces
      # target .jar file in the classpath, otherwise this variable will be empty
      set (INTERFACES_JAR_FILE ${CMAKE_BINARY_DIR}/src/interfaces/upm_interfaces.jar)
    endif ()
    # For linker to report unresolved symbols.  Note, there is currently no test
    # for linker flags (similar to compile files), so this is it for now.
    swig_link_libraries (javaupm_${libname} ${MRAAJAVA_LIBRARY} ${JAVA_LIBRARIES} ${libname} -Wl,--unresolved-symbols=report-all)
    target_include_directories ( ${SWIG_MODULE_javaupm_${libname}_REAL_NAME}
      PUBLIC
      "${JAVA_INCLUDE_DIRS}"
      "${JAVA_INCLUDE_PATH}"
      ${DEPEND_DIRS}
     )
    set_target_properties (javaupm_${libname} PROPERTIES
      COMPILE_FLAGS "-fpermissive"
      PREFIX "lib"
      SUFFIX ".so"
    )

    # Turn off flags for wrapper
    foreach(flag ${SWIG_CXX_DISABLE_WARNINGS})
      compiler_flag_supported(CXX is_supported ${flag})
      if (is_supported)
        target_compile_options(${SWIG_MODULE_javaupm_${libname}_REAL_NAME}
          PRIVATE "${flag}")
      endif(is_supported)
    endforeach(flag ${SWIG_CXX_DISABLE_WARNINGS})

    install (TARGETS javaupm_${libname} LIBRARY
        DESTINATION ${LIB_INSTALL_DIR}
        COMPONENT ${CMAKE_PROJECT_NAME}-java)

    # Java jar files always need to go under lib/java, regardless of
    # architecture.
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}.jar
        DESTINATION lib/java
        COMPONENT ${CMAKE_PROJECT_NAME}-java)

    if (NOT DEFINED $ENV{JAVA_HOME_NATIVE})
        set (JAVAC $ENV{JAVA_HOME}/bin/javac)
        set (JAR $ENV{JAVA_HOME}/bin/jar)
    else ()
        set (JAVAC $ENV{JAVA_HOME_NATIVE}/bin/javac)
        set (JAR $ENV{JAVA_HOME_NATIVE}/bin/jar)
    endif ()

    # Only include the upm_interfaces.jar in the classpath for targets which
    # depend on the interfaces target.  This fixes an issue where javac
    # complains about an empty upm_interfaces.jar when trying to build a target
    # which does not depend on javaupm_interfaces.  If not previously set,
    # INTERFACES_JAR_FILE will be empty, and javac *should* not complain.
    add_custom_command (TARGET javaupm_${libname}
        POST_BUILD
        COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}
        COMMAND ${JAVAC} *.java -d ${CMAKE_CURRENT_BINARY_DIR} -cp ${INTERFACES_JAR_FILE}:.
        COMMAND ${JAR} cf upm_${libname}.jar upm_${libname}
    )

    # Save the absolute path of the jar file as a property of this java target
    set_target_properties (javaupm_${libname} PROPERTIES
        JAR_FILE_ABSOLUTE ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}.jar)

    # Keep track of all JAVA targets
    set(UPM_TARGETS_JAVA ${UPM_TARGETS_JAVA} javaupm_${libname} CACHE INTERNAL "List of JAVA target names")
  endif (NOT ";${JAVASWIG_BLACKLIST};" MATCHES ";${libname};")
endfunction(upm_swig_java)

if (NPM)
add_custom_target (npmpkg)
add_custom_command (TARGET npmpkg POST_BUILD COMMAND echo "Creating NPM tarballs..."
                                             COMMAND find . -maxdepth 2 -type d -name "jsupm_*" -print0 |
                                                     xargs -0 -I {} tar czf `basename {}`.tar.gz -C {} .
)
endif (NPM)

if (BUILDSWIGNODE)
  if(SWIG_VERSION VERSION_LESS 3.0.5 AND NODE_VERSION_STRING VERSION_GREATER 0.12)
    message(WARNING "WARNING - SWIG 3.0.5+ required for building with nodejs 0.12. Current version is ${SWIG_VERSION}")
  endif()
  macro(createpackagejson)
    if (NPM)
      # Dependency to generate wrappers before packing, skip modules that depend on 3rd party libs
      if (NOT DEFINED reqlibname)
        set (reqlibname "")
      endif ()
      if (reqlibname STREQUAL "" OR reqlibname MATCHES "mraa")
        # Grab all module sources and headers, also add sources to gyp list
        file (GLOB srcfiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
                                     "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
        foreach (srcfile ${srcfiles})
          file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/${srcfile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname})
          if(${srcfile} MATCHES ".c$")
            get_filename_component(csource ${srcfile} NAME_WE)
            file (RENAME ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}/${srcfile} ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}/${csource}_c.c)
            set (upm_LIB_SRCS_GYP "'${csource}_c.c',\n${upm_LIB_SRCS_GYP}")
          elseif (${srcfile} MATCHES ".cxx$" OR ${srcfile} MATCHES ".cpp$")
            set (upm_LIB_SRCS_GYP "'${srcfile}',\n${upm_LIB_SRCS_GYP}")
          endif ()
        endforeach ()
        set (upm_LIB_SRCS_GYP "'jsupm_${libname}JAVASCRIPT_wrap.cxx',\n${upm_LIB_SRCS_GYP}")

        # Create list of include directories and copy them
        foreach (includedir ${UPM_COMMON_HEADER_DIRS})
          file (RELATIVE_PATH rel ${CMAKE_SOURCE_DIR} ${includedir})
          set (upm_LIB_INCLUDE_DIRS_GYP "'${rel}',\n${upm_LIB_INCLUDE_DIRS_GYP}")
        endforeach (includedir)
        file (COPY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname})
        set (upm_LIB_INCLUDE_DIRS_GYP "'.',\n${upm_LIB_INCLUDE_DIRS_GYP}")

        # Utilities and interfaces
        file (COPY ${CMAKE_SOURCE_DIR}/src/utilities DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname})
        file (COPY ${CMAKE_SOURCE_DIR}/src/interfaces DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname})
        set (upm_LIB_SRCS_GYP "'utilities/upm_utilities.c',\n${upm_LIB_SRCS_GYP}")
        set (upm_LIB_INCLUDE_DIRS_GYP "'utilities',\n${upm_LIB_INCLUDE_DIRS_GYP}")
        set (upm_LIB_INCLUDE_DIRS_GYP "'interfaces',\n${upm_LIB_INCLUDE_DIRS_GYP}")

        # Add readme, package.json for NPM and node-gyp config file
        configure_file (${PROJECT_SOURCE_DIR}/src/binding.gyp.in ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}/binding.gyp @ONLY)
        configure_file (${PROJECT_SOURCE_DIR}/src/package.json.npm.in ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}/package.json @ONLY)
        configure_file (${PROJECT_SOURCE_DIR}/doxy/README.npm.md ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}/README.md)

        add_dependencies (npmpkg jsupm_${libname})
      else ()
          message ("Skipping jsupm_${libname}, needs ${reqlibname}")
      endif ()
    endif (NPM)

    # package.json for regular install
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
        DESTINATION ${NODE_MODULE_INSTALL_PATH}
        COMPONENT ${CMAKE_PROJECT_NAME}-nodejs)
  endmacro(createpackagejson)
endif(BUILDSWIGNODE)

# Cordova binding
if (BUILDCORDOVA)
  add_custom_target(cordova_binding ALL
                    COMMAND upm-cordova-binding ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/cordova ${VERSION}
                    COMMENT "Building cordova bindings based on swig java modules")

  install(DIRECTORY ${CMAKE_BINARY_DIR}/cordova
      DESTINATION ${CMAKE_INSTALL_PREFIX}
      COMPONENT ${CMAKE_PROJECT_NAME}-cordova)
endif (BUILDCORDOVA)

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
  # Exports to PARENT scope:
  #     libname -> CXX target name
  #     libnamec -> C target name (if used)
  set (options CPP_WRAPS_C)
  set (oneValueArgs NAME DESCRIPTION)
  set (multiValueArgs CPP_HDR CPP_SRC C_HDR C_SRC FTI_SRC FTI_HDR REQUIRES)
  # Parse function parameters
  cmake_parse_arguments(UPM_MIXED_MODULE_INIT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  # Set the description
  set (libdescription ${UPM_MIXED_MODULE_INIT_DESCRIPTION})

  # Make sure REQUIRES is a list
  separate_arguments(UPM_MIXED_MODULE_INIT_REQUIRES)

  # Always build C libs first
  if (UPM_MIXED_MODULE_INIT_C_SRC)
    set (libname ${UPM_MIXED_MODULE_INIT_NAME})
    # Set the src and hpp variables for upm_module_init
    set (module_src ${UPM_MIXED_MODULE_INIT_C_SRC})
    set (module_hpp ${UPM_MIXED_MODULE_INIT_C_HDR})

    # If building FTI, and FTI src exists, add it in
    if (BUILDFTI AND UPM_MIXED_MODULE_INIT_FTI_SRC)
      #set (module_src ${UPM_MIXED_MODULE_INIT_C_SRC} ${UPM_MIXED_MODULE_INIT_FTI_SRC})
      list (APPEND module_src ${UPM_MIXED_MODULE_INIT_FTI_SRC})
    endif (BUILDFTI AND UPM_MIXED_MODULE_INIT_FTI_SRC)

    # Set a flag to tell upm_module_init that it's building a C library
    set (IS_C_LIBRARY TRUE)
    upm_module_init(${UPM_MIXED_MODULE_INIT_REQUIRES})

    # If a c target has a dependency on utilities-c, then swap the corresponding
    # C++ target's dependency on utilities-c to utilities.  This solves a niche
    # problem for Android  Things (C++ which depends on C which depends on
    # utilities-c is not getting the utilities-c library).  Since only C++
    # packages get swig'ed  to Java, the only way to get the utilities-c library
    # is to packages with a utilites C++ library and add a dependency.
    # TODO: Find a better way to do this.
    if ("${UPM_MIXED_MODULE_INIT_REQUIRES}" MATCHES utilities-c)
      list (REMOVE_ITEM UPM_MIXED_MODULE_INIT_REQUIRES utilities-c)
      list (APPEND UPM_MIXED_MODULE_INIT_REQUIRES utilities)
    endif ()

    ## "export" the logical C lib target for the calling module's
    ## CMakeLists.txt
    set (libnamec ${libname} PARENT_SCOPE)

  endif (UPM_MIXED_MODULE_INIT_C_SRC)

  # Build C++ if enabled AND C++ headers exist
  if (BUILDCPP AND UPM_MIXED_MODULE_INIT_CPP_HDR)
    # Set the src and hpp variables for upm_module_init
    set (module_src ${UPM_MIXED_MODULE_INIT_CPP_SRC})
    set (module_hpp ${UPM_MIXED_MODULE_INIT_CPP_HDR})

    # Reset the libname (upm_module_init can change it)
    set (libname ${UPM_MIXED_MODULE_INIT_NAME})
    unset (IS_C_LIBRARY)
    upm_module_init(${UPM_MIXED_MODULE_INIT_REQUIRES})

    # If the C++ wraps the C target, add the C target as a dependency
    if (UPM_MIXED_MODULE_INIT_CPP_WRAPS_C)
      target_link_libraries(${libname} ${libname}-c)
    endif (UPM_MIXED_MODULE_INIT_CPP_WRAPS_C)

    ## "export" the logical C++ lib target for the calling module's
    ## CMakeLists.txt
    set (libname ${libname} PARENT_SCOPE)

  endif (BUILDCPP AND UPM_MIXED_MODULE_INIT_CPP_HDR)

endfunction (UPM_MIXED_MODULE_INIT)

function(upm_module_init)
  set (basename ${libname})

  # Save off ARGN as target dependency list for use in any macros below
  set (_c_cxx_dependency_list ${ARGN})

  # If this is a C library, handle different collateral naming
  if (IS_C_LIBRARY)
    set (libname ${libname}-c)
    # If this is a C library, export C library target name to parent's libname
    set (libname ${libname} PARENT_SCOPE)
    set (libprefix upmc-)
    # Keep track of all C targets
    set(UPM_TARGETS_C ${UPM_TARGETS_C} ${libname} CACHE INTERNAL "List of C target names")
  else ()
    # Keep track of all CXX targets
    set(UPM_TARGETS_CXX ${UPM_TARGETS_CXX} ${libname} CACHE INTERNAL "List of CXX target names")
    set (libprefix upm-)
  endif (IS_C_LIBRARY)

  # Create the target library from src/hdrs
  add_library (${libname} SHARED ${module_src} ${module_hpp})

  # Specify the current source directory as an INTERFACE include dir.
  # This allows for transitive header dependencies via target_link_libraries
  target_include_directories(${libname} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

  # Also add common header directories to this target
  target_include_directories (${libname} PUBLIC ${UPM_COMMON_HEADER_DIRS})

  # Iterate over all arguments passed to upm_module_init.
  #     1. Add them via target_link_libraries
  #     2. If mraa, then add a link and include dependency
  foreach (linkflag ${ARGN})
    # Add necessary MRAA components IF the target requires it
    if (${linkflag} MATCHES mraa)
      target_link_libraries (${libname} ${MRAA_LIBRARY})
      # Always add a PUBLIC dependency to MRAA include dirs
      target_include_directories (${libname} PUBLIC ${MRAA_INCLUDE_DIRS})
    else ()
      # Else, add the linkflag directly
      target_link_libraries (${libname} ${linkflag})
    endif (${linkflag} MATCHES mraa)
  endforeach ()

  set_target_properties(
    ${libname}
    PROPERTIES PREFIX lib${libprefix}
    OUTPUT_NAME ${basename}
    SOVERSION ${upm_VERSION_MAJOR}
    VERSION ${upm_VERSION_STRING}
  )

  # Don't SWIG C
  if (NOT IS_C_LIBRARY)
    # When auto-generating a SWIG .i file, first include any C headers which
    # end in *_regs.h.  Then, include the C++ headers.
    if (UPM_MIXED_MODULE_INIT_C_HDR AND UPM_MIXED_MODULE_INIT_CPP_WRAPS_C)
        string(REGEX MATCHALL "[^;]*_regs.h" _regs "${UPM_MIXED_MODULE_INIT_C_HDR}")
        if(_regs)
            list(APPEND module_h_hpp "${_regs}")
        endif()
    endif ()
    list(APPEND module_h_hpp "${module_hpp}")
    separate_arguments (module_h_hpp)
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

  # Skip if the libname is in the blacklist
  if (BUILDCORDOVA)
    if (NOT IS_C_LIBRARY AND NOT ";${JAVASWIG_BLACKLIST};" MATCHES ";${libname};")
      add_dependencies(cordova_binding javaupm_${libname})
    endif()
  endif (BUILDCORDOVA)

  # Add additional properties to this target (used later for .pc/pom file creation)
  set_target_properties(${libname} PROPERTIES PKG_CONFIG_NAME "${libprefix}${basename}")
  set_target_properties(${libname} PROPERTIES PKG_CONFIG_DESCRIPTION "${libdescription}")
  set_target_properties(${libname} PROPERTIES PKG_EXT_REQ_LIBS "${reqlibname}")
  set_target_properties(${libname} PROPERTIES TARGET_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

  # Install target library to lib dir
  install (TARGETS ${libname}
      DESTINATION ${LIB_INSTALL_DIR}
      COMPONENT ${CMAKE_PROJECT_NAME})

  # Install header files to include/upm/
  install (FILES ${module_hpp}
      DESTINATION include/upm
      COMPONENT ${CMAKE_PROJECT_NAME}-dev)

  # Install JSON library descriptor files into datadir (if they exist)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${basename}.json")
    install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/${basename}.json"
      DESTINATION ${CMAKE_INSTALL_DATADIR}/upm/${basename}
      COMPONENT ${CMAKE_PROJECT_NAME})
  endif ()

  if (IPK)
    cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
    set(CPACK_COMPONENT_${libname}_DESCRIPTION "${libdescription}")
  endif()
endfunction(upm_module_init)

# Top-level module init
if (BUILDSWIGPYTHON)
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "# UPM python modules")
  if(PYTHON2LIBS_FOUND)
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON2_PACKAGES_PATH}/upm
        COMPONENT ${CMAKE_PROJECT_NAME}-python2)
  endif(PYTHON2LIBS_FOUND)
  if(PYTHON3LIBS_FOUND)
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON3_PACKAGES_PATH}/upm
        COMPONENT ${CMAKE_PROJECT_NAME}-python3)
  endif(PYTHON3LIBS_FOUND)
endif (BUILDSWIGPYTHON)

# Provide a cached variable to groups of targets
set(UPM_TARGETS_C "" CACHE INTERNAL "List of C target names")
set(UPM_TARGETS_CXX "" CACHE INTERNAL "List of CXX target names")
set(UPM_TARGETS_JAVA "" CACHE INTERNAL "List of JAVA target names")
set(UPM_TARGETS_NODEJS "" CACHE INTERNAL "List of NodeJs target names")
set(UPM_TARGETS_PYTHON2 "" CACHE INTERNAL "List of Python2 target names")
set(UPM_TARGETS_PYTHON3 "" CACHE INTERNAL "List of Python3 target names")

# MODULE_LIST can be specified on the CMake command line to specify
# a list of directories under src to include in the build.
# MODULE_LIST is optional.
#   If MODULE_LIST is provided, only add the directories specified
#   If MODULE_LIST is NOT provided, add all subdirectories under src
# Example (-DMODULE_LIST="dfrph;rotaryencoder")
if (NOT MODULE_LIST)
  # If no module list, add all subdirectories
  subdirlist(MODULE_LIST ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# If the module list does NOT include the utilities directory, prepend it since
# most sensor library targets depend on utilities
if (NOT "${MODULE_LIST}" MATCHES ";utilities;")
  set(MODULE_LIST "utilities;${MODULE_LIST}")
endif()

# If the module list does NOT include the interfaces directory, prepend it since
# some sensor library targets depend on interfaces
if (NOT "${MODULE_LIST}" MATCHES ";interfaces;")
  set(MODULE_LIST "interfaces;${MODULE_LIST}")
endif()

# Iterate over each directory in MODULE_LIST
foreach(subdir ${MODULE_LIST})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
    add_subdirectory(${subdir})
  endif()
endforeach()

# Print stats for all targets
_print_target_summary()

# Generate a pkg-config file (.pc) per target
_gen_pkg_config_per_target(C ${UPM_TARGETS_C})
_gen_pkg_config_per_target(CXX ${UPM_TARGETS_CXX})

# Generate a pom file per CXX target
if (BUILDSWIGJAVA)
  _gen_pom_per_target(${UPM_TARGETS_CXX})
endif (BUILDSWIGJAVA)

# Available to the top-level CMakeLists.txt
set (UPM_TARGETS_CXX ${UPM_TARGETS_CXX} PARENT_SCOPE)
set (UPM_TARGETS_PYTHON2 ${UPM_TARGETS_PYTHON2} PARENT_SCOPE)
