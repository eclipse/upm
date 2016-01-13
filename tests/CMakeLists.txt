add_test (NAME check_consistency COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/check_consistency.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILDSWIGJAVA)
    add_test (NAME check_autoloadlibrary COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_autoloadlibrary.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_samplenames COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_samplenames.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_test (NAME check_clean COMMAND ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/check_clean.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif ()
