set (libname "ledbar")
add_library (ledbar SHARED my9221.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (ledbar ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES my9221.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm LEDBAR")

