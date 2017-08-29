# Finding and pointing a variable to the npm executable if found
# Only works on Linux systems as of now

find_program(NPM_EXECUTABLE NAMES npm
    HINTS
    /usr
)

if(NPM_EXECUTABLE)
    message(STATUS "NPM Executable found at ${NPM_EXECUTABLE}")
else()
    message(ERROR "Unable to find NPM installation, please install NPM")
endif()
