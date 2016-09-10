upm_mixed_module_init (NAME grove
    DESCRIPTION "upm grove module"
    CPP_HDR grovebutton.hpp groveled.hpp groverelay.hpp groverotary.hpp
            grovetemp.hpp grovebase.hpp grove.hpp
    CPP_SRC grovebutton.cxx groveled.cxx groverelay.cxx groverotary.cxx
            grovetemp.cxx
    REQUIRES mraa)
