add_library (hmc5883l SHARED hmc5883l.cxx)

include_directories (${MAA_INCLUDE_DIR})

target_link_libraries (hmc5883l ${MAA_LIBRARIES})
