add_executable (compass compass.cxx)
add_executable (groveled groveled.cxx)
add_executable (grovetemp grovetemp.cxx)
add_executable (lcm-lcd lcm-lcd.cxx)

include_directories (${PROJECT_SOURCE_DIR}/src/hmc5883l)
include_directories (${PROJECT_SOURCE_DIR}/src/grove)
include_directories (${PROJECT_SOURCE_DIR}/src/lcm1602)

target_link_libraries (compass hmc5883l ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (groveled grove ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (grovetemp grove ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (lcm-lcd lcm1602 ${CMAKE_THREAD_LIBS_INIT})
