upm_mixed_module_init (NAME mcp2515
    DESCRIPTION "CAN Bus Controller"
    C_HDR mcp2515.h mcp2515_regs.h
    C_SRC mcp2515.c
    CPP_HDR mcp2515.hpp
    CPP_SRC mcp2515.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
