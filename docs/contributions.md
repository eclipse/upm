Contributing a module                         {#contributions}
=====================

In order to contribute to the project:
- The top of each source file must contain a comment block containing the
  license information.
- Commits must be named `<file/module>: Some decent description`.
- Each commit must have a sign-off line by everyone who authored or reviewed
  them.
- Your new module must have an example that builds against your UPM library.
- Attempt to have some decent API documentation as described in the the @ref
  documentation [guide](documentation.md).

Including the MIT license
=========================
Choosing the [MIT license](http://opensource.org/licenses/MIT) is preferred for
the UPM repository. Below is the comment block needed at the top each source
file:

        /*
         * Author: <your full name>
         * Copyright (c) <year> <copyright holder>
         *
         * Author: <contributing author full name - if applicable>
         * Copyright (c) <year> <copyright holder>
         *
         * This program and the accompanying materials are made available under the
         * terms of the MIT License which is available at https://opensource.org/licenses/MIT.
         *
         * SPDX-License-Identifier: MIT
         */

Eclipse Contributor Agreement
============

Your contribution cannot be accepted unless you have a signed [ECA - Eclipse Foundation Contributor Agreement](http://www.eclipse.org/legal/ECA.php) in place.

Here is the checklist for contributions to be considered _acceptable_:

1. [Create an account at Eclipse](https://dev.eclipse.org/site_login/createaccount.php).
2. Add your GitHub user name in your account settings.
3. [Log into the project's portal](https://projects.eclipse.org/) and sign the ["Eclipse ECA"](https://projects.eclipse.org/user/sign/cla).
4. Ensure that you [_sign-off_](https://wiki.eclipse.org/Development_Resources/Contributing_via_Git#Signing_off_on_a_commit) your Git commits.
5. Ensure that you use the _same_ email address as your Eclipse account in commits.
6. Include the appropriate copyright notice and license at the top of each file.

Your signing of the ECA will be verified by a webservice called 'ip-validation'
that checks the email address that signed-off on your commits has signed the
ECA. **Note**: This service is case-sensitive, so ensure the email that signed
the ECA and that signed-off on your commits is the same, down to the case.

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
