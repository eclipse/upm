upm_mixed_module_init (NAME hmc5883l
    DESCRIPTION "3-axis Digital Compass"
    C_HDR hmc5883l.h
    C_SRC hmc5883l.c
    CPP_HDR hmc5883l.hpp
    CPP_SRC hmc5883l.cxx
    CPP_WRAPS_C
    FTI_SRC hmc5883l_fti.c
    REQUIRES mraa)

target_link_libraries(${libnamec} m)
