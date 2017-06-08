if (JPEG_FOUND)
    upm_mixed_module_init (NAME vcap
        DESCRIPTION "Video Frame Capture and Image Save Utility"
        CPP_HDR vcap.hpp
        CPP_SRC vcap.cxx
        REQUIRES jpeg)
endif()
