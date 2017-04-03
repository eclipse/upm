# Create an list of all C++ source files in this directory
file (GLOB example_src_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cxx")

# - Handle special cases here --------------------------------------------------

# Test humidity interface for 2 sensor libraries
add_example(humidity-sensor.cxx TARGETS si7005 bmp280)
# Test pressure interface for 2 sensor libraries
add_example(pressure-sensor.cxx TARGETS bmp280 bmpx8x)
# Test temperature interface for 3 sensor libraries
add_example(temperature-sensor.cxx TARGETS bmp280 bmpx8x si7005)
# Test light interface for 2 sensor libraries
add_example(light-sensor.cxx TARGETS si1132 max44009)
# Test light controller interface for 3 sensor libraries
add_example(light-controller.cxx TARGETS lp8860 ds1808lc hlg150h)

# - Create an executable for all other src files in this directory -------------
foreach (_example_src ${example_src_list})
    add_example(${_example_src})
endforeach ()
