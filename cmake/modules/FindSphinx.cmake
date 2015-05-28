find_program (SPHINX_EXECUTABLE NAMES sphinx-build
    HINTS $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx documentation html generator"
)

find_program (SPHINX_API_EXECUTABLE NAMES sphinx-apidoc
    HINTS $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx documentation rst generator"
)

include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (Sphinx DEFAULT_MSG
    SPHINX_EXECUTABLE
    SPHINX_API_EXECUTABLE
)

# Get Sphinx version
if (SPHINX_EXECUTABLE)
    execute_process(COMMAND ${SPHINX_EXECUTABLE} --version
                    OUTPUT_VARIABLE SPHINX_VERSION)
    if(SPHINX_VERSION)
      string(REGEX MATCH "([0-9]\\.[0-9]\\.[0-9])" SPHINX_VERSION_STR ${SPHINX_VERSION})
      message ("INFO - Sphinx version is " ${SPHINX_VERSION_STR})
    endif()
endif ()

mark_as_advanced (SPHINX_EXECUTABLE)
mark_as_advanced (SPHINX_API_EXECUTABLE)
