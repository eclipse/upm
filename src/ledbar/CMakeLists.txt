set (libname "ledbar")
add_library (ledbar SHARED my9221.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (ledbar ${MAA_LIBRARIES})
