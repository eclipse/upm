# FindNpm
# --------
#
# Find npm
#
# This module finds an installed npm.  It sets the following variables:
#
#   NPM_FOUND - Set to true if npm is found
#   NPM_DIR - The directory where npm is installed
#   NPM_GLOBAL_NODE_MODULE_DIR - The global node_modules directory
#   NPM_EXECUTABLE - The path to the npm executable
#   NPM_VERSION  - The version number of the npm executable

find_program(NPM_EXECUTABLE NAMES npm HINTS /usr)

# If npm was found, fill in the rest
if (NPM_EXECUTABLE)
    # Set the global node_modules location
    execute_process(COMMAND ${NPM_EXECUTABLE} root -g
        OUTPUT_VARIABLE NPM_GLOBAL_NODE_MODULE_DIR
        ERROR_VARIABLE NPM_root_g_error
        RESULT_VARIABLE NPM_root_g_result_code)
    # Remove and newlines
    string (STRIP ${NPM_GLOBAL_NODE_MODULE_DIR} NPM_GLOBAL_NODE_MODULE_DIR)
    if(NPM_root_g_result_code)
        if(NPM_FIND_REQUIRED)
            message(SEND_ERROR "Command \"${NPM_EXECUTABLE} root -g\" failed with output:\n${NPM_root_g_error}")
        else ()
            message(STATUS "Command \"${NPM_EXECUTABLE} root -g\" failed with output:\n${NPM_root_g_error}")
        endif ()
    endif()
    unset(NPM_root_g_error)
    unset(NPM_root_g_result_code)

    # Set the NPM dir
    if (EXISTS "${NPM_GLOBAL_NODE_MODULE_DIR}/npm")
        set(NPM_DIR "${NPM_GLOBAL_NODE_MODULE_DIR}/npm")
    endif()

    # Set the VERSION
    execute_process(COMMAND ${NPM_EXECUTABLE} -v
        OUTPUT_VARIABLE NPM_VERSION
        ERROR_VARIABLE NPM_version_error
        RESULT_VARIABLE NPM_version_result_code)

    if(NPM_version_result_code)
        if(NPM_FIND_REQUIRED)
            message(SEND_ERROR "Command \"${NPM_EXECUTABLE} -v\" failed with output:\n${NPM_version_error}")
        else()
            message(STATUS "Command \"${NPM_EXECUTABLE} -v\" failed with output:\n${NPM_version_error}")
        endif ()
    endif ()
    unset(NPM_version_error)
    unset(NPM_version_result_code)

    # Remove and newlines
    string (STRIP ${NPM_VERSION} NPM_VERSION)

    set (NPM_FOUND TRUE)
else()
    # Fail on REQUIRED
    if (Npm_FIND_REQUIRED)
        message(SEND_ERROR "Failed to find npm executable")
    endif()
endif ()

find_package_handle_standard_args(NPM
    REQUIRED_VARS NPM_EXECUTABLE NPM_DIR
    VERSION_VAR NPM_VERSION )

mark_as_advanced(NPM_DIR NPM_GLOBAL_NODE_MODULE_DIR NPM_EXECUTABLE NPM_VERSION)
