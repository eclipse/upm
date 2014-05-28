set (libname "buzzer")
add_library (buzzer SHARED buzzer.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (buzzer ${MAA_LIBRARIES})
