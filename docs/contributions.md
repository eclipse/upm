Contributing a module                         {#contributions}
=====================

Here are the rules of contribution:
- Your new module must have an example that builds against your UPM library.
- Each commit must have a sign-off line by everyone who authored or reviewed
  them.
- Commits must be named `<file/module>: Some decent description`.
- You must license your module under a FOSS license. The recommended license
  is MIT but any permissive license is fine. Please consider that people using
  UPM may want to write proprietary programs with your sensors so we like to
  avoid GPL. If your license is not MIT please include a LICENSE file in
  src/mymodule/.
- The top of each source file must contain a comment block containing the
  license information.
- Please test your module builds before contributing and make sure it works on
  the latest version of libmraa. If you tested on a specific board/platform
  please tell us what this was in your PR.
- Try not to break master. In any commit.
- Attempt to have some decent API documentation as described in the the @ref
  documentation [guide](documentation.md).

Including the MIT license
=========================
Choosing the [MIT license](http://opensource.org/licenses/MIT) is preferred for
the UPM repository.  Below is the comment block needed at the top each source
file:

        /*
         * The MIT License (MIT)
         *
         * Author: <your full name> 
         * Copyright (c) <year> <copyright holder>
         *
         * Author: <contributing author full name - if applicable> 
         * Copyright (c) <year> <copyright holder>
         *
         * Permission is hereby granted, free of charge, to any person obtaining a copy of
         * this software and associated documentation files (the "Software"), to deal in
         * the Software without restriction, including without limitation the rights to
         * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
         * the Software, and to permit persons to whom the Software is furnished to do so,
         * subject to the following conditions:
         *
         * The above copyright notice and this permission notice shall be included in all
         * copies or substantial portions of the Software.
         *
         * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
         * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
         * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
         * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
         * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
         * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
         */


Code signing
============

The sign-off is a simple line at the end of the explanation for the
patch, which certifies that you wrote it or otherwise have the right to pass it
on as an open-source patch.  The rules are pretty simple: if you can certify
the below:

        Developer's Certificate of Origin 1.1

        By making a contribution to this project, I certify that:

        (a) The contribution was created in whole or in part by me and I
            have the right to submit it under the open source license
            indicated in the file; or

        (b) The contribution is based upon previous work that, to the best
            of my knowledge, is covered under an appropriate open source
            license and I have the right under that license to submit that
            work with modifications, whether created in whole or in part
            by me, under the same open source license (unless I am
            permitted to submit under a different license), as indicated
            in the file; or

        (c) The contribution was provided directly to me by some other
            person who certified (a), (b) or (c) and I have not modified
            it.

        (d) I understand and agree that this project and the contribution
            are public and that a record of the contribution (including all
            personal information I submit with it, including my sign-off) is
            maintained indefinitely and may be redistributed consistent with
            this project or the open source license(s) involved.

then you just add a line to each of your commits with `--signoff` saying

        Signed-off-by: Random J Developer <random@developer.example.org>

using your real name (sorry, no pseudonyms or anonymous contributions.)
Unsigned commits will not be accepted.


Creating a new sensor library using the sensortemplate
=======================================

A stubbed-out sensor library is available which can be leveraged to get
up-and-running quickly when writing a new sensor library.  Use the shell
commands below to generate collateral files for your new sensor library.


```shell
#!/bin/bash

function make_new_sensor {
    export SensorName=$1
    # Get a lowercase version of the string
    export sensorname=${SensorName,,}

    # Make sure this is run from the root UPM directory
    if ! grep -q 'UPM ' README.md; then echo "Please run from the root UPM directory"; return -1; fi

    printf "Generating new sensor: ${SensorName}\n"
    # Copy sensortemplate files to ${sensorname}
    find docs/ examples/ src/ -name '*sensortemplate*' -exec bash -c 'cp -r $0 ${0/sensortemplate/${sensorname}}' {} \;
    # Copy SensorTemplate files to ${SensorName}
    find examples/ src/ -name '*SensorTemplate*' -exec bash -c 'cp -r $0 ${0/SensorTemplate/${SensorName}}' {} \;
    # Rename sernsortemplate src files
    rename "s/sensortemplate/${sensorname}/" src/${sensorname}/*
    # Search/replace the new files, replacing all instances of sensortemplate
    perl -p -i -e "s/SensorTemplate/${SensorName}/g" src/${sensorname}/* examples/*/*${sensorname}* examples/*/*${SensorName}*
    perl -p -i -e "s/sensortemplate/${sensorname}/g" src/${sensorname}/* examples/*/*${sensorname}* examples/*/*${SensorName}*
    # Remove objects starting with "//" from the new library descriptor .json file
    perl -p -i -e 'BEGIN{undef $/;} s/\s+"\/\/.*?},//smg' src/${sensorname}/${sensorname}.json
    # Add mynewmodule example target for java
    perl -p -i -e "s/^((.*)SensorTemplateSample sensortemplate(.*))/\1\n\2${SensorName}Sample ${sensorname}\3/g" examples/java/CMakeLists.txt
    # Add mynewmodule example mappings for doxygen
    perl -p -i -e "s/^(.*SensorTemplateSample.*)$/\1\n${sensorname}.cxx\t${SensorName}Sample.java\t${sensorname}.js\t${sensorname}.py/g" doxy/samples.mapping.txt
    # Display TODO's
    printf "Generation complete for sensor library: ${SensorName}\n"
    printf "TODO's:\n"
    printf "\t1. Update src/hdr files: src/${sensorname}/${sensorname}.hpp src/${sensorname}/${sensorname}.cxx\n"
    printf "\t\tChange the Author\n"
    printf "\t\tChange the Copyright\n"
    printf "\t\tUpdate all doxygen tags (follow directions for @tags)\n"
    printf "\t2. Update examples: examples/*/${sensorname}.* examples/java/*${SensorName}*.java\n"
    printf "\t3. Overwrite docs/images/${sensorname}.png with a valid image of your sensor\n"
}

# Call make_new_sensor with your new sensor name, example: 'MyNewSensor1234'
make_new_sensor MyNewSensor1234
```

Once all files have been created, they can be used as a starting-point for your
new library.  They will need additional customization (your name/email address,
documentation, sensor images, etc).
