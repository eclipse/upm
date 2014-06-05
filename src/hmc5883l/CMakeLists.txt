set (libname "hmc5883l")
add_library (hmc5883l SHARED hmc5883l.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (hmc5883l ${MAA_LIBRARIES})

if (SWIG_FOUND)
  find_package (PythonLibs)

  include_directories (
    ${PYTHON_INCLUDE_PATH}
    ${PYTHON_INCLUDE_DIRS}
    ${MAA_INCLUDE_DIR}
    .
  )

  set_source_files_properties (pyupm_hmc5883l.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_hmc5883l.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_hmc5883l.i PROPERTIES SWIG_FLAGS "-node")

  swig_add_module (pyupm_hmc5883l python pyupm_hmc5883l.i hmc5883l.cxx)
#  swig_add_module (jsupm_hmc5883l javascript jsupm_hmc5883l.i hmc5883l.cxx)

  swig_link_libraries (pyupm_hmc5883l ${PYTHON_LIBRARIES} ${MAA_LIBRARIES})
#  swig_link_libraries (jsupm_hmc5883l ${MAA_LIBRARIES})

  if (DOXYGEN_FOUND)
    set (CMAKE_SWIG_FLAGS -DDOXYGEN=${DOXYGEN_FOUND})
    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doxy2swig.py -n
        ${CMAKE_BINARY_DIR}/xml/${libname}_8h.xml
        ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i
        DEPENDS ${CMAKE_BINARY_DIR}/xml/${libname}_8h.xml
    )
    add_custom_target (${libname}doc_i DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i)
    add_dependencies (${libname}doc_i doc)
    add_dependencies (${SWIG_MODULE_pyupm_hmc5883l_REAL_NAME} ${libname}doc_i)
  endif ()

endif ()

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES hmc5883l.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm Digital Compass")
