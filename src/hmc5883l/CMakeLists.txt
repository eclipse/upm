add_library (hmc5883l SHARED hmc5883l.cxx)

include_directories (${MAA_INCLUDE_DIR})

target_link_libraries (hmc5883l ${MAA_LIBRARIES})

find_package (PythonLibs)

include_directories (
  ${PYTHON_INCLUDE_PATH}
  ${PYTHON_INCLUDE_DIRS}
  ${MAA_INCLUDE_DIR}
  .
)

set_source_files_properties (pyupm_hmc5883l.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties (jsupm_hmc5883l.i PROPERTIES CPLUSPLUS ON)

swig_add_module (pyupm_hmc5883l python pyupm_hmc5883l.i hmc5883l.cxx)
swig_add_module (jsupm_hmc5883l python jsupm_hmc5883l.i hmc5883l.cxx)
swig_link_libraries (pyupm_hmc5883l ${PYTHON_LIBRARIES} ${MAA_LIBRARIES})
swig_link_libraries (jsupm_hmc5883l ${PYTHON_LIBRARIES} ${MAA_LIBRARIES})
