# Add some general tests (which use python2)...
if (PYTHON_DEFAULT_AVAILABLE)
    # Consistency check
    add_test (NAME check_consistency COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_consistency.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # Check file encodings in src tree
    add_test (NAME check_file_encoding COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_file_encoding.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src)
endif (PYTHON_DEFAULT_AVAILABLE)

# Add some java tests
if (BUILDSWIGJAVA AND PYTHON_DEFAULT_AVAILABLE)
    add_test (NAME check_autoloadlibrary COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_autoloadlibrary.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_examplenames_java COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py java
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_clean COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_clean.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif (BUILDSWIGJAVA AND PYTHON_DEFAULT_AVAILABLE)

# Add a node test
if (BUILDSWIGNODE AND PYTHON_DEFAULT_AVAILABLE)
    add_test (NAME check_examplenames_js COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py js
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif (BUILDSWIGNODE AND PYTHON_DEFAULT_AVAILABLE)

# Add a python test
if (BUILDSWIGPYTHON AND PYTHON_DEFAULT_AVAILABLE)
    # Check python examples against cxx examples
    add_test (NAME check_examplenames_python COMMAND ${PYTHON_DEFAULT_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_examplenames.py python
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif (BUILDSWIGPYTHON AND PYTHON_DEFAULT_AVAILABLE)

# Add some python2 tests
if (BUILDSWIGPYTHON AND PYTHON2INTERP_FOUND)
    # Test load python2 modules
    add_test (NAME check_load_modules_python2 COMMAND ${PYTHON2_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
        ${CMAKE_BINARY_DIR}/src/*/python${PYTHON2_VERSION_MAJOR}.${PYTHON2_VERSION_MINOR}/*.py
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)

    # Test load examples w/python2
    add_test (NAME check_load_examples_python2 COMMAND ${PYTHON2_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
        ${CMAKE_SOURCE_DIR}/examples/python/*.py
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)
endif (BUILDSWIGPYTHON AND PYTHON2INTERP_FOUND)

# Add some python3 tests
if (BUILDSWIGPYTHON AND PYTHON3INTERP_FOUND)
    add_test (NAME check_load_modules_python3 COMMAND ${PYTHON3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
        ${CMAKE_BINARY_DIR}/src/*/python${PYTHON3_VERSION_MAJOR}.${PYTHON3_VERSION_MINOR}/*.py
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)

    # Test load examples w/python3
    add_test (NAME check_load_examples_python3 COMMAND ${PYTHON3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_pythonload.py
        ${CMAKE_SOURCE_DIR}/examples/python/*.py
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/)
endif (BUILDSWIGPYTHON AND PYTHON3INTERP_FOUND)
