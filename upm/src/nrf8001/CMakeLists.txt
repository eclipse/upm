upm_mixed_module_init (NAME nrf8001
    DESCRIPTION "Bluetooth Low Energy (BLE) Module"
    CPP_HDR aci.h aci_cmds.h aci_evts.h acilib_defs.h acilib.h acilib_if.h
            acilib_types.h aci_protocol_defines.h aci_queue.h aci_setup.h
            boards.h dtm.h hal_aci_tl.h hal_platform.h lib_aci.h uart_over_ble.h
    CPP_SRC acilib.cpp aci_queue.cpp aci_setup.cpp hal_aci_tl.cpp lib_aci.cpp
            nrf8001.cxx
    REQUIRES mraa)
