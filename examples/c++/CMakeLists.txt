# Create an list of all C++ source files in this directory
file (GLOB example_src_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cxx")

# - Handle special cases here --------------------------------------------------

# Test temperature interface for 2 sensor libraries
add_example(iTemperature-sample.cxx TARGETS lm35 abp)
# Test light interface for 3 sensor libraries
add_example(iLight-sample.cxx TARGETS apds9002 bh1750 max44009)

# - Create an executable for all other src files in this directory -------------
foreach (_example_src ${example_src_list})
    add_example(${_example_src})
endforeach ()
