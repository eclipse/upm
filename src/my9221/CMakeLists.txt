upm_mixed_module_init (NAME my9221
    DESCRIPTION "12-channel Constant Current LED Driver with Grayscale"
    C_HDR my9221.h
    C_SRC my9221.c
    CPP_HDR my9221.hpp groveledbar.hpp grovecircularled.hpp
    CPP_SRC my9221.cxx groveledbar.cxx grovecircularled.cxx
    CPP_WRAPS_C
    REQUIRES mraa utilities-c)
