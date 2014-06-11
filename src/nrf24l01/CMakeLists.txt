set (libname "nrf24l01")
set (libdescription "libupm NRF tx/rx")
add_library (nrf24l01 SHARED nrf24l01.cxx)
upm_module_init()
