Installing UPM                         {#installing}
============

UPM packages are provided for some of the major supported distributions, making
it very easy to install UPM and its dependencies without having to go through a
full build.

### Poky Linux

Yocto Project based Poky Linux builds are provided for Intel Galileo, Intel
Edison and Minnowboard. These are the official images released with the Intel
IoT Developer Kit and can found [here](https://iotdk.intel.com/images/3.5/).

To update to the latest stable UPM version:

```bash
echo "src intel-iotdk https://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586/" > /etc/opkg/intel-iotdk.conf
opkg update
opkg upgrade mraa upm
```

If you would like to try the development version use `intelgalactic-dev`
instead.

### WindRiver Pulsar Linux

WR Pulsar uses SmartPM for software updates. Updating to the latest UPM version
requires:

```bash
smart channel -y --add upm_mraa_channel type=rpm-md name="MRAA/UPM" baseurl=http://iotdk.intel.com/repos/pulsar/8/x64
smart update
smart install -y mraa upm
```

For Pulsar 7 use the following baseurl instead: `baseurl=http://iotdk.intel.com/repos/pulsar/7/x64`

Wind River Linux 7 uses 01.org repositories and will have a default update
channel that includes MRAA & UPM. Thus you only have to update and install.

### Ubuntu Desktop/Server

MRAA and UPM are available through a Personal Package Archive (PPA) on Ubuntu
that can be found [here](https://launchpad.net/~mraa/+archive/ubuntu/mraa).

To install:

```bash
sudo add-apt-repository ppa:mraa/mraa
sudo apt-get update
sudo apt-get install libupm-dev libupm-java python-upm python3-upm node-upm upm-examples
```

Running UPM applications on Ubuntu systems requires elevated permissions
(e.g. run with `sudo`).

### Fedora Linux

MRAA and UPM are available through the default package repositories on Fedora and can be
installed in the standard method using dnf.

To install:

```bash
sudo dnf install upm
sudo dnf install paython3-upm nodejs-upm upm-examples
```

### Installing for Red Hat Enterprise Linux, CentOS and Other Linux Distributions

The UPM project does not currently distribute official binaries for RHEL
or CentOS so developers will have to compile the project from sources as
described [here](./building.md).

For testing and development purposes it may be possible to share and install
.deb and .rpm packages across similar Linux builds.

### Node.js bindings only (NPM)

This option allows compiling MRAA/UPM on any system with npm and node-gyp.

The UPM modules have a hard dependency on MRAA, thus you will have to install
it first. Also, make sure Node can find MRAA by updating the `NODE_PATH`
variable if necessary. A global installation is preferred:

`npm install mraa -g`

After this, to install a UPM module using npm simply type:

`npm install jsupm_<module-name> -g`

In order to initialize the platform, require MRAA *before* you load a UPM
module in your code:

```js
require('mraa');
var lib = require('jsupm_<module-name>');
```
