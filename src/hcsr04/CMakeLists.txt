set (libname "hcsr04")
add_library (hcsr04 SHARED hcsr04.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (hcsr04 ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES hcsr04.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm proximity sensor")
