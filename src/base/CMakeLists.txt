add_library(upm_base STATIC ainBase.cxx)
add_definitions(-fPIC)

link_directories(${MRAA_LIBDIR})
include_directories (${MRAA_INCLUDE_DIRS})
include_directories (${UPM_H_DIR})
target_link_libraries (upm_base ${MRAA_LIBRARIES})

target_include_directories(upm_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install (DIRECTORY . DESTINATION include/upm/base FILES_MATCHING PATTERN "*.hpp")
