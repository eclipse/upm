set (libname "servo")
add_library (servo SHARED servo.cxx es08a.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (servo ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES servo.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm servo")
