find_program (UPM_CORDOVA_BINDING NAMES upm-cordova-binding
    DOC "UPM Cordova binding generator")

include (FindPackageHandleStandardArgs)

# Get version
if (UPM_CORDOVA_BINDING)
    execute_process(COMMAND ${NPM_EXECUTABLE} info upm-cordova-binding version
        OUTPUT_VARIABLE UPM_CORDOVA_BINDING_VERSION
        ERROR_VARIABLE UPM_CORDOVA_BINDING_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE)
endif()

find_package_handle_standard_args(UpmCordovaGenerator
    REQUIRED_VARS UPM_CORDOVA_BINDING
    VERSION_VAR UPM_CORDOVA_BINDING_VERSION
    FAIL_MESSAGE "Unable to find the npm package for generating UPM Cordova bindings.  The generator can be installed via: 'npm install -g upm-cordova-binding'")

mark_as_advanced (UPM_CORDOVA_BINDING UPM_CORDOVA_BINDING_VERSION)
