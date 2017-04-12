// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_gas
%include "../upm.i"
%include "../carrays_uint16_t.i"

%feature("autodoc", "3");

%include "gas.hpp"
%{
    #include "gas.hpp"
%}

%include "mq2.hpp"
%{
    #include "mq2.hpp"
%}

%include "mq3.hpp"
%{
    #include "mq3.hpp"
%}

%include "mq4.hpp"
%{
    #include "mq4.hpp"
%}

%include "mq5.hpp"
%{
    #include "mq5.hpp"
%}

%include "mq6.hpp"
%{
    #include "mq6.hpp"
%}

%include "mq7.hpp"
%{
    #include "mq7.hpp"
%}

%include "mq8.hpp"
%{
    #include "mq8.hpp"
%}

%include "mq9.hpp"
%{
    #include "mq9.hpp"
%}

%include "tp401.hpp"
%{
    #include "tp401.hpp"
%}
