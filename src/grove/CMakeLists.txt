set (libname "grove")
add_library (grove SHARED grove.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (grove ${MAA_LIBRARIES})

if (SWIG_FOUND)
  find_package (PythonLibs)

  include_directories (
    ${PYTHON_INCLUDE_PATH}
    ${PYTHON_INCLUDE_DIRS}
    ${MAA_INCLUDE_DIR}
    .
  )

  set_source_files_properties (pyupm_grove.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_grove.i PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (jsupm_grove.i PROPERTIES SWIG_FLAGS "-node")

  swig_add_module (pyupm_grove python pyupm_grove.i grove.cxx)
#  swig_add_module (jsupm_grove javascript jsupm_grove.i grove.cxx)

  swig_link_libraries (pyupm_grove ${PYTHON_LIBRARIES} ${MAA_LIBRARIES})
#  swig_link_libraries (jsupm_grove ${MAA_LIBRARIES})

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
    add_dependencies (${SWIG_MODULE_pyupm_grove_REAL_NAME} ${libname}doc_i)
  endif ()

endif ()
