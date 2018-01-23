%module javaupm_relay
%include "../upm.i"

%{
    #include "relay.hpp"
%}

%include "relay.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_relay)
