add_executable (compass compass.cxx)
add_executable (groveled groveled.cxx)
add_executable (grovetemp grovetemp.cxx)

include_directories (${PROJECT_SOURCE_DIR}/src/hmc5883l)
include_directories (${PROJECT_SOURCE_DIR}/src/grove)

target_link_libraries (compass hmc5883l ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (groveled grove ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (grovetemp grove ${CMAKE_THREAD_LIBS_INIT})
