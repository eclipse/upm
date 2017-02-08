upm_mixed_module_init (NAME lsm303agr
    DESCRIPTION "3-Axis eCompass Module"
    C_HDR lsm303agr.h lsm303agr_defs.h
    C_SRC lsm303agr.c
    CPP_HDR lsm303agr.hpp
    CPP_SRC lsm303agr.cxx
    FTI_SRC lsm303agr_fti.c
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
