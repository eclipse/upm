set (libname "4digitdisplay")
add_library (4digitdisplay SHARED tm1637.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (4digitdisplay ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES tm1637.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm 4 Digit Buzzer")
