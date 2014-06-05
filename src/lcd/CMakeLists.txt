set (libname "i2clcd")
add_library (i2clcd SHARED iiclcd.cxx lcm1602.cxx jhd1313m1.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (i2clcd ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES iiclcd.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm lcd")
