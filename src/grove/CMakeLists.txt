upm_mixed_module_init (NAME grove
    DESCRIPTION "upm grove module"
    CPP_HDR groveled.hpp
            grovetemp.hpp grovebase.hpp grove.hpp
    CPP_SRC groveled.cxx
            grovetemp.cxx
    REQUIRES mraa)
