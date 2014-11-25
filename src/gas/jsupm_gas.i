%module jsupm_gas
%include "../upm.i"
%include "../carrays_uint16_t.i"

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
