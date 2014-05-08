set (libname "lcm1602")
add_library (lcm1602 SHARED lcm1602.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (lcm1602 ${MAA_LIBRARIES})

if (SWIG_FOUND)
  find_package (PythonLibs)

  include_directories (
    ${PYTHON_INCLUDE_PATH}
    ${PYTHON_INCLUDE_DIRS}
    ${MAA_INCLUDE_DIR}
    .
  )

  set_source_files_properties (pyupm_lcm1602.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_lcm1602.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_lcm1602.i PROPERTIES SWIG_FLAGS "-node")

  swig_add_module (pyupm_lcm1602 python pyupm_lcm1602.i lcm1602.cxx)
#  swig_add_module (jsupm_lcm1602 javascript jsupm_lcm1602.i lcm1602.cxx)

  swig_link_libraries (pyupm_lcm1602 ${PYTHON_LIBRARIES} ${MAA_LIBRARIES})
#  swig_link_libraries (jsupm_lcm1602 ${MAA_LIBRARIES})

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
    add_dependencies (${SWIG_MODULE_pyupm_lcm1602_REAL_NAME} ${libname}doc_i)
  endif ()

endif ()
