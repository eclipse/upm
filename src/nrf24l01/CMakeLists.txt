set (libname "nrf24l01")
add_library (nrf24l01 SHARED nrf24l01.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (nrf24l01 ${MAA_LIBRARIES})
