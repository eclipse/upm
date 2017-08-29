# This macro sets up the initial JSON test environment
# it copies over everything required by the test over to the
# build directory so that the tests can be run from there
macro(SETUP_JSON_COLLATERAL UBD NE)
    execute_process(COMMAND mkdir -p ${UBD}/build/node_test;
        COMMAND cp ${UBD}/tests/node/package.json ${UBD}/build/node_test/;
        COMMAND cp ${UBD}/tests/node/jsonlint.js ${UBD}/build/node_test/;
        COMMAND cp ${UBD}/tests/node/test.js ${UBD}/build/node_test/;
        COMMAND ${NE} install --prefix=${UBD}/build/node_test/;
        RESULT_VARIABLE ret_val
    )
    if(ret_val)
        message(FATAL_ERROR "value of result of first test: ${ret_val_1}")
    endif()
endmacro()

# This macro runs the json test and checks the json files for
# their validity and also checks for the required fields
macro(EXEC_JSON_TEST UBD NE)
    execute_process(COMMAND ${NE} test --prefix=${UBD}/build/node_test/
        RESULT_VARIABLE ret_val
    )
    execute_process(COMMAND rm -rf ${UBD}/build/node_test)
    if(ret_val)
        message(FATAL_ERROR "value of the result of the second test: ${ret_val_2}")
    endif()
endmacro()

# this macro removes all the JSON collateral from the build directory
#macro(REMOVE_JSON_COLLATERAL UBD)
#    execute_process(COMMAND rm -rf ${UBD}/build/node_test)
#endmacro()

setup_json_collateral(${UPMBASEDIR} ${NPMEXEC})
exec_json_test(${UPMBASEDIR} ${NPMEXEC})
#remove_json_collateral(${UPMBASEDIR})
