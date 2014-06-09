set (libname "nrf24l01")
add_library (nrf24l01 SHARED nrf24l01.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (nrf24l01 ${MAA_LIBRARIES})

install (TARGETS ${libname} DESTINATION lib/upm COMPONENT ${libname})
install (FILES nrf24l01.h DESTINATION include/upm COMPONENT ${libname})

if (IPK)
  cpack_add_component (${libname} DISPLAY_NAME ${libname} REQUIRED INSTALL_TYPES all)
  set(CPACK_COMPONENT_${libname}_DESCRIPTION "libupm NRF tx/rx")
endif()
