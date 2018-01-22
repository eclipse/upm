if (OPENZWAVE_FOUND)
  set (libname "ozw")
  set (libdescription "Module for the OpenZWave Library Interface")
  set (module_src ${libname}.cxx zwNode.cxx ozwinterface.cxx ozwdump.cxx aeotecss6.cxx aeotecsdg2.cxx aeotecdw2e.cxx aeotecdsb09104.cxx tzemt400.cxx)
  set (module_hpp ${libname}.hpp ozwinterface.hpp ozwdump.hpp aeotecss6.hpp aeotecsdg2.hpp aeotecdw2e.hpp aeotecdsb09104.hpp tzemt400.hpp)

  set (reqlibname "libopenzwave")
  upm_module_init(${OPENZWAVE_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  target_include_directories(${libname} PUBLIC ${OPENZWAVE_INCLUDE_DIRS})

  # openzwave/aes/aes.h has unknown pragmas, disable warning -Wunknown-pragmas
  compiler_flag_supported(CXX is_supported -Wno-unknown-pragmas)
  if (is_supported)
      target_compile_options(${libname} PUBLIC -Wno-unknown-pragmas)
  endif (is_supported)
endif ()
