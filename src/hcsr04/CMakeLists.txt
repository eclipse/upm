set (libname "hcsr04")
add_library (hcsr04 SHARED hcsr04.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (hcsr04 ${MAA_LIBRARIES})
