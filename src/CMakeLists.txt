macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

macro (upm_CREATE_INSTALL_PKGCONFIG generated_file install_location)
  configure_file (${PROJECT_SOURCE_DIR}/src/pkgconfig.in
    ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} @ONLY)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${generated_file} DESTINATION ${install_location})
endmacro (upm_CREATE_INSTALL_PKGCONFIG)

macro(upm_SWIG_PYTHON)
  if (BUILDSWIGPYTHON AND BUILDSWIG)
    set_source_files_properties (pyupm_${libname}.i PROPERTIES CPLUSPLUS ON)
    swig_add_module (pyupm_${libname} python pyupm_${libname}.i ${module_src})
    swig_link_libraries (pyupm_${libname} ${PYTHON_LIBRARIES} ${MRAA_LIBRARIES})
    target_include_directories ( ${SWIG_MODULE_pyupm_${libname}_REAL_NAME}
      PUBLIC
      "${PYTHON_INCLUDE_PATH}"
      "${PYTHON_INCLUDE_DIRS}"
     )
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/_pyupm_${libname}.so
         ${CMAKE_CURRENT_BINARY_DIR}/pyupm_${libname}.py
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/
         COMPONENT ${libname})
  endif()
endmacro()

macro(upm_SWIG_NODE)
  if (BUILDSWIGNODE AND BUILDSWIG)
    set_source_files_properties (jsupm_${libname}.i PROPERTIES CPLUSPLUS ON)
    set_source_files_properties (jsupm_${libname}.i PROPERTIES SWIG_FLAGS "-node")
    swig_add_module (jsupm_${libname} javascript jsupm_${libname}.i ${module_src})
    swig_link_libraries (jsupm_${libname} ${MRAA_LIBRARIES} ${NODE_LIBRARIES})
    target_include_directories ( ${SWIG_MODULE_jsupm_${libname}_REAL_NAME}
      PUBLIC
      "${NODE_INCLUDE_DIRS}"
     )
    set_target_properties (jsupm_${libname} PROPERTIES
      COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -DBUILDING_NODE_EXTENSION"
      PREFIX ""
      SUFFIX ".node"
    )
    createpackagejson(${libname})
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}.node
         DESTINATION lib/node_modules/jsupm_${libname} COMPONENT ${libname})
  endif()
endmacro()

macro(upm_doxygen)
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
    if (BUILDSWIG)
      add_dependencies (_pyupm_${libname} ${libname}doc_i)
    else ()
      add_dependencies (${libname} doc)
    endif ()
  endif ()
endmacro()

if (SWIG_FOUND)
  if(BUILDSWIGPYTHON)
    find_package (PythonLibs)
    string (REPLACE "." ";" PYTHON_VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
    list (GET PYTHON_VERSION_LIST 0 PYTHON_VERSION_MAJOR)
    list (GET PYTHON_VERSION_LIST 1 PYTHON_VERSION_MINOR)
  endif(BUILDSWIGPYTHON)
  if(BUILDSWIGNODE)
    find_path (NODE_ROOT_DIR "node/node.h")
    set (NODE_INCLUDE_DIRS
      ${NODE_ROOT_DIR}/src
      ${NODE_ROOT_DIR}/node
      ${NODE_ROOT_DIR}/deps/v8/include
      ${NODE_ROOT_DIR}/deps/uv/include
    )
    macro(createpackagejson)
        configure_file (${PROJECT_SOURCE_DIR}/src/package.json.in ${CMAKE_CURRENT_BINARY_DIR}/package.json @ONLY)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/package.json
            DESTINATION lib/node_modules/jsupm_${libname} COMPONENT ${libname})
    endmacro()

  endif(BUILDSWIGNODE)
endif()

macro(upm_module_init)
  add_library (${libname} SHARED ${module_src})
  include_directories (${MRAA_INCLUDE_DIR} .)
  target_link_libraries (${libname} ${MRAA_LIBRARIES})
  set_target_properties(
    ${libname}
    PROPERTIES PREFIX "libupm-"
    SOVERSION ${upm_VERSION_MAJOR}
    VERSION ${upm_VERSION_STRING}
  )
  upm_create_install_pkgconfig (upm-${libname}.pc lib${LIB_SUFFIX}/pkgconfig)
  if (SWIG_FOUND)
    upm_swig_python()
    upm_swig_node()
  endif()
  if (BUILDDOC)
    upm_doxygen()
  endif()
  install(TARGETS ${libname} DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install (FILES ${module_h} DESTINATION include/upm COMPONENT ${libname})

  if (IPK)
    cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
    set(CPACK_COMPONENT_${libname}_DESCRIPTION "${libdescription}")
  endif()
endmacro(upm_module_init)

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach()
