set (libname "buzzer")
add_library (buzzer SHARED buzzer.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (buzzer ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES buzzer.h DESTINATION include/upm COMPONENT ${libname})

cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm buzzer")
