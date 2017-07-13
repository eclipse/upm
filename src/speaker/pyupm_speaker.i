// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_speaker
%include "../upm.i"

%include "speaker.hpp"
%{
    #include "speaker.hpp"
%}
