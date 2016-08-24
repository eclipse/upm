# Extract module name from non-standard example name
macro(get_module_name example_name module_name)
  string(LENGTH ${example_name} length)
  string(FIND ${example_name} "-" index)
  if (${index} GREATER 1)
    string(SUBSTRING ${example_name} 0 ${index} substr)
    set(${module_name} ${substr})
  elseif (${example_name} MATCHES "^grove")
    set (${module_name} "grove")
  elseif ((${example_name} MATCHES "^mq" AND ${length} EQUAL 3) OR ${example_name} STREQUAL "tp401")
    set (${module_name} "gas")
  else()
    set(${module_name} ${example_name})
  endif()
endmacro()

# Set source file, include and linker settings for an example
# If example cannot be built, example_bin is cleared
macro(add_custom_example example_bin example_src example_module_list)
  set(found_all_modules TRUE)
  foreach (module ${example_module_list})
    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/src/${module}")
      set(found_all_modules FALSE)
    endif()
    if (MODULE_LIST)
      list(FIND MODULE_LIST ${module} index)
      if (${index} EQUAL -1)
        set(found_all_modules FALSE)
      endif()
    endif()
  endforeach()
  if (found_all_modules)
    add_executable (${example_bin} ${example_src})
    target_link_libraries (${example_bin} ${CMAKE_THREAD_LIBS_INIT})
    foreach (module ${example_module_list})
      set(module_dir "${PROJECT_SOURCE_DIR}/src/${module}")
      include_directories (${module_dir})
      if (${module} STREQUAL "lcd")
        set(module "i2clcd")
      endif()
      target_link_libraries (${example_bin} ${module})
    endforeach()
  else()
   MESSAGE(INFO " Ignored ${example_bin}")
   set (example_bin "")
  endif()
endmacro()


# Add specified example by name
# Note special case for grove based examples
macro(add_example example_name)
  set(example_src "${example_name}.c")
  set(example_bin "${example_name}-example-c")
  get_module_name(${example_name} module_name)
  set(module_dir "${PROJECT_SOURCE_DIR}/src/${module_name}")
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${example_src}"
     AND EXISTS ${module_dir}
     AND IS_DIRECTORY ${module_dir})
    add_custom_example(${example_bin} ${example_src} ${module_name})
    if ((NOT ${example_bin} STREQUAL "") AND (${module_name} STREQUAL "grove"))
      set(grove_module_path "${PROJECT_SOURCE_DIR}/src/${example_name}")
      if (EXISTS ${grove_module_path})
        include_directories(${grove_module_path})
        target_link_libraries (${example_bin} ${example_name})
      endif()
    endif()
  else()
   MESSAGE(INFO " Ignored ${example_bin}")
  endif()
endmacro()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

# UPM c include directories
include_directories (${PROJECT_SOURCE_DIR}/include
                     ${CMAKE_SOURCE_DIR}/src/utilities)

# Set the mraa include and link directories prior to adding examples
include_directories (${MRAA_INCLUDE_DIRS})
link_directories (${MRAA_LIBDIR})

# If your sample source file matches the name of the module it tests, add it here
# Exceptions are as follows:
#  string after first '-' is ignored (e.g. nrf24l01-transmitter maps to nrf24l01)
#  mq? will use module gas
#  grove* will use module grove
add_example (dfrph)
add_example (vk2828u7)
