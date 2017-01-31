if (BACNET_FOUND)
  set (libname "bacnetmstp")
  set (libdescription "Driver module for BACnet MS/TP devices")
  set (module_src ${libname}.cxx device-client.c bacnetutil.cxx)
  set (module_hpp ${libname}.hpp bacnetutil.hpp device.h)

  set (reqlibname "libbacnet")
  upm_module_init(${BACNET_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  # Add the bacnet include dirs to this target
  target_include_directories(${libname} PUBLIC ${BACNET_INCLUDE_DIRS})
endif ()
