%include "../upm.i"

/* Make it easy to use the methods which take a uint& id and score */
%include "typemaps.i"
%apply uint16_t &OUTPUT {uint16_t &id};
%apply uint16_t &OUTPUT {uint16_t &score};

%{
    #include "zfm20.hpp"
%}
%include "zfm20.hpp"
