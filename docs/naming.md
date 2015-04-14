Naming a module                         {#naming}
===============

UPM attempts to follow a clear naming pattern. Modules should be sensibly named
and then placed in ${libdir}/upm and headers in ${includedir}/upm, all modules
should be prefixed with libupm-<modulename>. The upm_module_init will
automatically name python UPM modules as pyupm_<modulename> and javascript
modules as jsupm_<modulename>. For example for src/grove/ the library built
will be libupm-grove.so, the python module pyupm_grove and the js module
jsupm_grove.

### Choosing a name for a new module

1. Pick a name, typically the chip name is the most sensible
2. Use it & stick to it

### Rules for name picking

1. Your lib must belong to the UPM namespace
2. Usually picking the name of the chip of your sensor/actuator makes sense.
Other times this does not. Try to pick a generic name so people with a similar
sensor can inherit your class if they only have minor changes.
3. Avoid brand names, often your module can be very generic with little effort
4. Use only lowercase characters in your file names and folder names.

### Doubt

If ever, give either of us a ping via email:
 mihai.tudor.panu@intel.com
 john.r.van.drasek@intel.com
 brendan.le.foll@intel.com
and we'll try suggest decent names for your module.

