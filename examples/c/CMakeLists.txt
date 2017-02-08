# Create an list of all C source files in this directory
file (GLOB example_src_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.c")

# - Handle special cases here --------------------------------------------------

#add_example(humidity-sensor.c TARGETS si7005-c bmp280-c)

# - Create an executable for all other src files in this directory -------------
foreach (_example_src ${example_src_list})
    add_example(${_example_src} TARGETS utilities-c SUFFIX "-c")
endforeach ()
