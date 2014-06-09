set (libname "max44000")
add_library (max44000 SHARED max44000.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (max44000 ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES max44000.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm MAX44000")

