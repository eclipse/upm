%include ".iUpmObject.i"

#if (SWIGJAVA)
#if SWIG_VERSION >= 0x030009
%include "swiginterface.i"
%interface_impl(upm::iMraa);
#endif
#endif

%{
#include "iMraa.hpp"
%}
%include "iMraa.hpp"

typedef std::vector<mraa::Aio*> vec_aio;
%template(vec_aio) std::vector<mraa::Aio*>;

typedef std::vector<mraa::Gpio*> vec_gpio;
%template(vec_gpio) std::vector<mraa::Gpio*>;

typedef std::vector<mraa::I2c*> vec_i2c;
%template(vec_i2c) std::vector<mraa::I2c*>;

typedef std::vector<mraa::Iio*> vec_iio;
%template(vec_iio) std::vector<mraa::Iio*>;

typedef std::vector<mraa::Pwm*> vec_pwm;
%template(vec_pwm) std::vector<mraa::Pwm*>;

typedef std::vector<mraa::Spi*> vec_spi;
%template(vec_spi) std::vector<mraa::Spi*>;

typedef std::vector<mraa::Uart*> vec_uart;
%template(vec_uart) std::vector<mraa::Uart*>;

typedef std::vector<mraa::UartOW*> vec_uartow;
%template(vec_uartow) std::vector<mraa::UartOW*>;
