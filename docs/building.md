Building UPM                         {#building}
============

UPM uses cmake in order to make compilation relatively painless. Cmake runs
build out of tree so the recommended way is to clone from git and make a build/
directory. Failure to do an out-of-source build may cause issues when
rebuilding later on.

This project depends on libmraa, so that needs to be installed first. Use the
following environment variables to configure the paths:

    PKG_CONFIG_PATH=$PKG_CONFIG_PATH:.../mraa/build/lib/pkgconfig
    CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:.../mraa/build/include
    LIBRARY_PATH=$LIBRARY_PATH:.../mraa/build/lib

UPM will attempt to build all directories inside src/ and they must contain
individual CMakeLists.txt files.

~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake ..
make
~~~~~~~~~~~~~

Our cmake configure has a number of options, cmake-gui or ccmake can show you
all the options. The interesting ones are detailed below:

Changing install path from /usr/local to /usr
~~~~~~~~~~~~~
-DCMAKE_INSTALL_PREFIX:PATH=/usr
~~~~~~~~~~~~~
Building debug build:
~~~~~~~~~~~~~
-DCMAKE_BUILD_TYPE=DEBUG
~~~~~~~~~~~~~
Using clang instead of gcc:
~~~~~~~~~~~~~
-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
~~~~~~~~~~~~~
Cross-compiling on a different system:
~~~~~~~~~~~~~
-DCMAKE_CXX_FLAGS:STRING=-m32 -march=i586
-DCMAKE_C_FLAGS:STRING=-m32 -march=i586
~~~~~~~~~~~~~
Building with an older version of swig (swig 2.0+) requires the disabling of javascript:
~~~~~~~~~~~~~
-DBUILDSWIGNODE=OFF
~~~~~~~~~~~~~
Disabling python module building
~~~~~~~~~~~~~
-DBUILDSWIGPYTHON=OFF
~~~~~~~~~~~~~
Setting the python library to use:
~~~~~~~~~~~~~
-DPYTHON_LIBRARY:FILEPATH=/usr/lib/libpython2.7.so.1.0
~~~~~~~~~~~~~
Building doxygen doc
~~~~~~~~~~~~~
-BUILDDOC=ON
~~~~~~~~~~~~~
You can also generate the include and lib directories containing all the sensor
headers and library files respectively with *make install*. Further, you may
choose to generate these only for a specific sensor you modified, and this can
be achieved by building from the individual makefile of the sensor. Assuming
you're in the build directory, to make the lcd module you would:

~~~~~~~~~~~~~
cd src/lcd
make install
~~~~~~~~~~~~~

Often developers are only interested in building one module or even just the
python/node module to do some quick testing using scripting. In order to do
this you need to use the target name for the python or node module you want to
rebuild. For example the lcd module target name is i2clcd. Therefore the python
module target name will be prefixed by _pyupm_. Just do the following to build
only that module. Modules not using the UPM cmake macros may have different
naming.

~~~~~~~~~~~~~
make _pyupm_i2clcd
~~~~~~~~~~~~~

Sometimes you want to build a small C++ example against an installed library.
This is fairly easy if installed system-wide. Just link against the correct
library (in this case libupm-tm1637) and then add /usr/include/upm to the
loader path:

~~~~~~~~~~~~
g++ test.cxx -lupm_tm1637 -I/usr/include/upm
~~~~~~~~~~~~

You can also use pkg-config to return the information to you, which is
considered the correct way if including UPM in a build system like cmake or
autotools on linux.

~~~~~~~~~~~
pkg-config --cflags --libs upm-tm1637
~~~~~~~~~~~
