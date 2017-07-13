// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_buzzer
%include "../upm.i"

%include "buzzer_tones.h"
%include "buzzer.hpp"
%{
    #include "buzzer.hpp"
%}
