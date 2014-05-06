add_executable (compass compass.cxx)
add_executable (groveled groveled.cxx)

include_directories (${PROJECT_SOURCE_DIR}/src/hmc5883l)
include_directories (${PROJECT_SOURCE_DIR}/src/grove)

target_link_libraries (compass hmc5883l)
target_link_libraries (groveled grove)
