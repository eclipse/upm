find_program (YUIDOC_EXECUTABLE NAMES yuidoc
    HINTS $ENV{YUIDOC_DIR}
    PATH_SUFFIXES bin
    DOC "Yuidoc documentation generator"
)

include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (Yuidoc DEFAULT_MSG
    YUIDOC_EXECUTABLE
)

# Get Yuidoc version
if (YUIDOC_EXECUTABLE)
    execute_process(COMMAND ${YUIDOC_EXECUTABLE} --version
                    OUTPUT_VARIABLE YUIDOC_VERSION)
    string(REGEX MATCH "([0-9]\\.[0-9]\\.[0-9])" YUIDOC_VERSION_STR ${YUIDOC_VERSION})
    message ("INFO - Yuidoc version is " ${YUIDOC_VERSION_STR})
endif ()

mark_as_advanced (YUIDOC_EXECUTABLE)
