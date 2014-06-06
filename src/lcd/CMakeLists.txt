set (libname "i2clcd")
add_library (i2clcd SHARED iiclcd.cxx lcm1602.cxx jhd1313m1.cxx ssd1308.cxx ssd1327.cxx)
include_directories (${MAA_INCLUDE_DIR})
target_link_libraries (i2clcd ${MAA_LIBRARIES})
