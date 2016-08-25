%module jsupm_zfm20
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"
%include "cpointer.i"

/* Send "int *" to JavaScript as intp */
%pointer_functions(int, intp);
%{
    #include "zfm20.hpp"
    speed_t int_B57600 = B57600;
%}

%include "zfm20.hpp"
speed_t int_B57600 = B57600;
