find_program(UPM_CORDOVA_BINDING NAMES upm-cordova-binding
)

if(UPM_CORDOVA_BINDING)
    message(STATUS "The npm package to build cordova bindings found at ${UPM_CORDOVA_BINDING}")
else()
    message(FATAL_ERROR "Unable to find the npm package for building cordova bindings, please install by 'npm install -g upm-cordova-binding'")
endif()