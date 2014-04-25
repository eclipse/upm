add_executable (compass compass.cxx)

include_directories (${PROJECT_SOURCE_DIR}/src/hmc5883l)

target_link_libraries (compass hmc5883l)
