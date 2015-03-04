Porting a module from Arduino                         {#porting}
=============================

Porting arduino libraries to libmraa as UPM libraries is usually fairly easy.
The issues typically come from misunderstanding of how a non real time OS deals
with interrupts and timers. It also highly depends on the sensor. A concrete
example is explained in detail on @ref max31855

### Adding a new module to UPM

1. Choose a name for your module (see @ref naming)
2. Make a new folder in src/modulename
3. Create a CMakeLists.txt file inside src/modulename

### CmakeLists.txt

By default you need a header called modulename.h and a C++ file called
modulename.cxx. You can have multiple headers and source files. Only public
headers need to be added to module_h and all source files need to be in
module_src.

~~~~~~~~~~~
set (libname "modulename")
set (libdescription "Module Description")
set (module_src ${libname}.cxx)
set (module_h ${libname}.h)
upm_module_init()
~~~~~~~~~~~

### Making your API

The easiest way to do this is to have a look at a similar sensor to yours.
Typically create a class for your sensor with a constructor that defines the
pins it is on. This constructor will create the mraa_*_context structs that are
required to talk to the board's IO. An I2c sensor will create a
mraa_i2c_context, keep it as a private member and require a bus number and slave
address in it's constructor.

Typically in sensors a simple object->read() function is preferred, depending on
your sensor/actuator this may or may not be easy or not even make sense. Most
UPM APIs have a simple set of functions.

### Mapping arduino API to libmraa

Your constructor is similar to the setup() function in arduino, you should
initialise your IO the way you want it. This means initialising contexts
(private members) and setting the correct modes for them.

See the mraa API documentation for exact API.

### Building

To build your module just follow @ref building. By creating a folder and the
CMakelists.txt file you have done all that is required to add your sensor to
the UPM build system.

### Sending your module to us for inclusion in UPM

The last step is when you're happy with your module and it works send us a pull
request! We'd love to include your sensor in our repository.

If you don't like github you can also send mihai.tudor.panu@intel.com a git
formatted patch of your sensor. More details are on @ref contributions and on
https://help.github.com/articles/creating-a-pull-request

