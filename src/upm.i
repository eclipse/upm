%include "std_string.i"
%include "stdint.i"

%typemap(out) mraa_result_t = int;
