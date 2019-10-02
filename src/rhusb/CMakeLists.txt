set (libname "rhusb")
set (libdescription "Omega RH-USB Temperature and Humidity Sensor")
set (module_src ${libname}.cxx)
set (module_hpp ${libname}.hpp)
set (module_iface iHumidity.hpp iTemperature.hpp)
upm_module_init(mraa)

