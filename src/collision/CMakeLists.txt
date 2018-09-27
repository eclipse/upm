upm_mixed_module_init (NAME collision
    DESCRIPTION "Collision Sensor"
    C_HDR collision.h
    C_SRC collision.c
    CPP_HDR collision.hpp
    CPP_SRC collision.cxx
    FTI_SRC collision_fti.c
    IFACE_HDR iCollision.hpp
    CPP_WRAPS_C
    REQUIRES mraa)
