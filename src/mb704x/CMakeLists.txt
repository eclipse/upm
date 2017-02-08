upm_mixed_module_init (NAME mb704x
    DESCRIPTION "MB7040/7047 I2C MaxSonar WR Ultrasonic Ranger"
    C_HDR mb704x.h
    C_SRC mb704x.c
    CPP_HDR mb704x.hpp
    CPP_SRC mb704x.cxx
    FTI_SRC mb704x_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
