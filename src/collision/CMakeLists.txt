upm_mixed_module_init (NAME collision
    DESCRIPTION "Collision sensor"
    C_HDR collision.h
    C_SRC collision.c
    CPP_HDR collision.hpp
    CPP_SRC collision.cxx
    FTI_SRC collision_fti.c
    CPP_WRAPS_C
    REQUIRES mraa)
