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
      COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=0x0${V8_DEFINE_STRING}"
      PREFIX ""
      SUFFIX ".node"
    )
    createpackagejson(${libname})
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/jsupm_${libname}.node
         DESTINATION lib/node_modules/jsupm_${libname} COMPONENT ${libname})
  endif()
endmacro()

macro(upm_SWIG_JAVA)
  if (BUILDSWIGJAVA AND BUILDSWIG)

    FIND_PACKAGE (JNI REQUIRED)

    include_directories (
      ${JAVA_INCLUDE_PATH}
      ${JAVA_INCLUDE_PATH2}
      ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    set_source_files_properties (javaupm_${libname}.i PROPERTIES CPLUSPLUS ON)
    set_source_files_properties (javaupm_${libname}.i PROPERTIES SWIG_FLAGS ";-package;upm_${libname};-I${CMAKE_BINARY_DIR}/src")
    swig_add_module (javaupm_${libname} java javaupm_${libname}.i ${module_src})
    swig_link_libraries (javaupm_${libname} ${MRAA_LIBRARIES} ${JAVA_LIBRARIES})
    target_include_directories ( ${SWIG_MODULE_javaupm_${libname}_REAL_NAME}
      PUBLIC
      "${JAVA_INCLUDE_DIRS}"
      "${JAVA_INCLUDE_PATH}"
     )
    set_target_properties (javaupm_${libname} PROPERTIES
      COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -DJAVACALLBACK"
      PREFIX "lib"
      SUFFIX ".so"
    )
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libjavaupm_${libname}.so
	DESTINATION lib/java
    )
    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}.jar
	DESTINATION lib/java
    )

    if (NOT DEFINED $ENV{JAVA_HOME_NATIVE})
	set (JAVAC $ENV{JAVA_HOME}/bin/javac)
	set (JAR $ENV{JAVA_HOME}/bin/jar)
    else ()
	set (JAVAC $ENV{JAVA_HOME_NATIVE}/bin/javac)
	set (JAR $ENV{JAVA_HOME_NATIVE}/bin/jar)
    endif ()

    add_custom_command (TARGET javaupm_${libname}
        POST_BUILD
        COMMAND cmake -E echo "Compiling java.."
        COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/upm_${libname}
        COMMAND ${JAVAC} *.java -d ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND cmake -E echo "Creating jar"
        COMMAND ${JAR} cvf upm_${libname}.jar upm_${libname}
    )

  endif()
endmacro()

macro(upm_doxygen)
  if (DOXYGEN_FOUND)
    if(NOT DEFINED classname)
      set (classname ${libname})
    endif()
    set (CMAKE_SWIG_FLAGS -DDOXYGEN=${DOXYGEN_FOUND})
    add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doxy2swig.py -n
        ${CMAKE_BINARY_DIR}/xml/${classname}_8h.xml
        ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i
        DEPENDS ${CMAKE_BINARY_DIR}/xml/${classname}_8h.xml
    )
    add_custom_target (${libname}doc_i DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${libname}_doc.i)
    add_dependencies (${libname}doc_i doc)
    if (BUILDSWIG)
      add_dependencies (_pyupm_${libname} ${libname}doc_i)
      add_dependencies (pydoc _pyupm_${libname})
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
    if(NOT NODE_FOUND)
      find_package(Node)
    endif()
    if(SWIG_VERSION VERSION_LESS 3.0.5 AND NODE_VERSION_STRING VERSION_GREATER 0.12)
      message("WARNING - SWIG 3.0.5+ required for building with nodejs 0.12. Current version is ${SWIG_VERSION}")
    endif()
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
#  if(BUILDSWIGJAVA)
#    add_subdirectory (java)
#  endif(BUILDSWIGJAVA)
    
endif()

macro(upm_module_init)
  add_library (${libname} SHARED ${module_src})
  foreach (linkflag ${ARGN})
    target_link_libraries (${libname} ${linkflag})
  endforeach ()
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
    upm_swig_java()
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
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory(${subdir})
    endif()
endforeach()
