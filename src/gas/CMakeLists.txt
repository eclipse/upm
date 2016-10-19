upm_mixed_module_init (NAME gas
    DESCRIPTION "Generic gas detection sensor library"
    C_HDR mqx.h
    C_SRC mqx.c
    CPP_HDR gas.hpp mq2.hpp mq3.hpp mq4.hpp mq5.hpp mq6.hpp mq7.hpp mq8.hpp mq9.hpp tp401.hpp
    CPP_SRC gas.cxx mq2.cxx mq3.cxx mq4.cxx mq5.cxx mq6.cxx mq7.cxx mq8.cxx mq9.cxx tp401.cxx
    FTI_SRC mqx_fti.c
    REQUIRES mraa)
