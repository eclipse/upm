set (libname "servo")
add_library (servo SHARED servo.cxx es08a.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (servo ${MAA_LIBRARIES})
