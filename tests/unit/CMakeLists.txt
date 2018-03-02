# For now, Google Test is NOT required */
find_package(GTest)

# If not found, print a status message and return
if(NOT GTEST_FOUND)
    message(STATUS "Install Google Test to enable additional unit testing")
    return ()
endif()

add_executable(utilities_tests utilities/utilities_tests.cxx)
target_link_libraries(utilities_tests utilities GTest::GTest GTest::Main)
gtest_add_tests(utilities_tests "" AUTO)
