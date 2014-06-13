Naming a module                         {#naming}
===============

UPM attemps to follow a clear naming pattern. Modules should be sensibly named
and then placed in /usr/lib/upm and headers in /usr/include/upm.

### Choosing a name

1. Pick a name
2. Use it

### Rules for name picking

1. Your lib must belong to namespace UPM
2. Usually picking the name of the chip of your sensor/actuator might make
sense. Other times this does not. Try to pick a generic name so people with a
similar sensor can inherit your class if they only have minor changes.
3. Avoid brand names

### Doubt

If ever, give me a ping via email: brendan.le.foll@intel.com and I'll try
suggest decent names for your module.

