set (libname "4digitdisplay")
add_library (4digitdisplay SHARED tm1637.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (4digitdisplay ${MAA_LIBRARIES})
