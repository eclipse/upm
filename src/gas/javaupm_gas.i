%module javaupm_gas
%include "../upm.i"
%include "typemaps.i"

%apply uint16_t *OUTPUT {uint16_t *buffer };

%include "gas.h"
%{
    #include "gas.h"
%}

%include "mq2.h"
%{
    #include "mq2.h"
%}

%include "mq3.h"
%{
    #include "mq3.h"
%}

%include "mq5.h"
%{
    #include "mq5.h"
%}

%include "mq9.h"
%{
    #include "mq9.h"
%}

%include "tp401.h"
%{
    #include "tp401.h"
%}
