add_test (NAME check_consistency COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/check_consistency.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILDSWIGJAVA)
    add_test (NAME check_autoloadlibrary COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_autoloadlibrary.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_examplenames_java COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py java
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_clean COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_clean.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

if (BUILDSWIGNODE)
    add_test (NAME check_examplenames_js COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py js
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif (BUILDSWIGNODE)

if (BUILDSWIGPYTHON)
    add_test (NAME check_examplenames_python COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py python
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    if (PYTHON2INTERP_FOUND)
        add_test (NAME check_load_modules_python2 COMMAND ${PYTHON2_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
            ${CMAKE_BINARY_DIR}/src/*/python${PYTHON2_VERSION_MAJOR}.${PYTHON2_VERSION_MINOR}/*.py
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)

        add_test (NAME check_load_examples_python2 COMMAND ${PYTHON2_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
            ${CMAKE_SOURCE_DIR}/examples/python/*.py
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)
    endif (PYTHON2INTERP_FOUND)

    if (PYTHON3INTERP_FOUND)
        add_test (NAME check_load_modules_python3 COMMAND ${PYTHON3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
            ${CMAKE_BINARY_DIR}/src/*/python${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}/*.py
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)
    endif (PYTHON3INTERP_FOUND)
endif (BUILDSWIGPYTHON)
