# Add an example executable target for the provided source file which depends
# on a UPM library target.  Strings after the first '-' are ignored.
#
# Usage:
#   add_example(source_file TARGETS <target0> <target1> <targetn> SUFFIX <target_suffix>)
#
# Parameters:
#   source_file - C/C++ source file. Filename must follow a specific format:
#       <library>-[additional].c(xx)
#       library: Name of UPM library required by example
#       -additional: Any other string used to identify the example
#   TARGETS - Additional dependency target/s required to BUILD/RUN this example.  If TARGETS is
#       set, these are in addition to the dependency library provided by the example filename.
#   SUFFIX - Provide a means to differentiate between C/C++ dependency targets and
#       example names.  Leave empty for CXX examples. Use '-c' for c targets.
#           SUFFIX  Example file  Dependency target  Example binary
#           ------  ------------  -----------------  --------------
#           "-c"    foo.c         foo-c              foo-example-c
#                   foo.cxx       foo                foo-example-cxx
#
# Parent scope variable:
#   example_src_list - Any example target which is successfully created by
#       add_example will be removed from the parent scope variable
#       example_src_list (if it exists).  This provides a means to handle
#       special case examples BEFORE handling all files in example_src_list.
#
# Examples:
#   # Creates sensorfoo-example-cxx, depends on target: sensorfoo
#   add_example(sensorfoo.cxx)
#
#   # Creates sensorfoo-example-c, depends on target: sensorfoo-c
#   add_example(sensorfoo.c SUFFIX "-c")
#
#   # Creates sensorfoo-bar-example-c, depends on target: sensorfoo-c
#   add_example(sensorfoo-bar.c SUFFIX "-c")
#
#   # Creates testfoobar-cxx, depends on targets: blib, blah
#   add_example(testfoobar.c TARGETS blib blah)
#
#   Examples:
#       a110x.c             Requires libupmc-a110x
#       lcm1602-i2c.c       Requires libupmc-lcm1602
#       bmp280-bme280.c     Requires libupmc-bmp280
#       lcd-eboled.cxx   Requires libupm-lcd
#
#
function (add_example example_src)
    set (oneValueArgs SUFFIX)
    set (multiValueArgs TARGETS)
    # Parse function parameters
    cmake_parse_arguments(add_example "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Attempt to remove this example source file from a list of example
    # sources.  This allows for handling special case examples.
    list (REMOVE_ITEM example_src_list ${example_src})
    set (example_src_list ${example_src_list} PARENT_SCOPE)

    # Get the base filename from the full filename
    # For file /some/path/to/sensorfoo-bar.c, example_name = sensorfoo-bar
    get_filename_component(example_name ${example_src} NAME_WE)

    # Parse out <library>
    # Example: sensorfoo-bar.c, library = sensorfoo
    string (REPLACE "-" ";" split_name ${example_name})
    list (GET split_name 0 library_name)

    # Parse dependency target name from example_src filename (add on suffix)
    # For sensorfoo, lib_target_names = sensorfoo-c
    set (lib_target_names "${library_name}${add_example_SUFFIX}")

    # If TARGETS is provided, add additional dependency targets
    if (add_example_TARGETS)
        list (APPEND lib_target_names "${add_example_TARGETS}")
    endif ()

    # Unfortunately, c++ and c library targets are not named the same. If
    # no suffix is provided, assume C++ and add a suffix of -cxx to the
    # example target name
    #           library         example target
    #       c:  sensorfoo-c     sensorfoo-bar-example-c
    #     cxx:  sensorfoo       sensorfoo-bar-example-cxx
    if (NOT add_example_SUFFIX)
        set(add_example_SUFFIX "-cxx")
    endif ()

    # Create the target name for this example: mylibrary-bar-example-c
    set (this_target_name "${example_name}-example${add_example_SUFFIX}")

    # If a dependency target does NOT exist, print a warning and skip
    foreach(_dep_target ${lib_target_names})
        if (NOT TARGET ${_dep_target})
            message(STATUS "Example ${example_src} is missing a required CMake target (${_dep_target}), skipping...")
            return()
        endif ()
    endforeach ()

    # Create an executable target for this example
    add_executable (${this_target_name} ${example_src})

    # Add each dependency to the library target
    foreach(_dep_target ${lib_target_names})
        target_link_libraries(${this_target_name} ${_dep_target} ${CMAKE_THREAD_LIBS_INIT} utilities-c)
    endforeach ()
endfunction (add_example example_src)

# Add subdirectories if BUILDEXAMPLES=on
if(BUILDEXAMPLES)
    # Add C examples
    add_subdirectory (c)

    # Add C++ examples?
    if (BUILDCPP)
        add_subdirectory (c++)
    endif ()

    # Add java examples?
    if(BUILDSWIGJAVA)
        add_subdirectory (java)
    endif()

    # Add all examples as an install component (if building examples)
    install (DIRECTORY ${PROJECT_SOURCE_DIR}/examples
        DESTINATION ${CMAKE_INSTALL_DATADIR}/upm
        COMPONENT ${CMAKE_PROJECT_NAME}-examples
        FILES_MATCHING
            PATTERN "*.c"
            PATTERN "*.cxx"
            PATTERN "*.hpp"
            PATTERN "*.java"
            PATTERN "*.js"
            PATTERN "*.py")
endif()
