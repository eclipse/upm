if (TINYB_FOUND)
  set (libname "2jciebu01_ble")
  set (libdescription "Omron BLE Environment Sensor")
  set (module_src "${libname}.cxx")
  set (module_hpp "${libname}.hpp")

  upm_module_init(tinyb 2jciebu01_usb)
endif()
