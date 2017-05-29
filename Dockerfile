FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive

# Preapare apt-get
RUN apt-get update && apt-get -y --no-install-recommends install software-properties-common

# Add Mraa Repository
RUN add-apt-repository ppa:mraa/mraa && \
  # Update apt-get
  apt-get -y update && \
  # Install apt-utils
  apt-get -y --no-install-recommends install apt-utils && \
  # Main Build Dependencies
  apt-get -y --no-install-recommends install git build-essential cmake clang-3.8 g++-4.8 libpthread-stubs0-dev pkg-config wget libpcre3 libpcre3-dev \
  # Mraa Build Dependencies
  libmraa1 libmraa-dev mraa-tools python-mraa python3-mraa libmraa-java \
  # Docs Build Dependencies
  python-sphinx doxygen graphviz \
  # Python Build Dependencies
  python python-dev python3 python3-dev \
  # Java Build Dependencies
  default-jre default-jdk \
  # Sensor Specific Build Dependencies
  libmodbus-dev libjpeg-dev
  # TODO: install libbacnet

# Install openzwave
RUN apt-get update && apt-get -y --no-install-recommends install libudev-dev && \
    git clone https://github.com/OpenZWave/open-zwave.git && cd open-zwave && make && make install && ldconfig /usr/local/lib64

# Swig Build Dependencies
RUN wget http://iotdk.intel.com/misc/tr/swig-3.0.10.tar.gz && \
    tar xf swig-3.0.10.tar.gz && cd swig-3.0.10 && \
    ./configure --prefix=/usr/ && make && make install && cd ..

# Node.js Build Dependencies
RUN wget -q -O - https://raw.githubusercontent.com/creationix/nvm/v0.33.2/install.sh | bash
ARG NODE_VERSION
ENV NVM_DIR /root/.nvm
RUN . $NVM_DIR/nvm.sh && nvm install $NODE_VERSION && nvm use $NODE_VERSION && \
    npm install -g node-gyp && node-gyp install

# Set Workdir
WORKDIR /usr/src/app

# Configure Build Arguments
ARG BUILDDOC
ARG BUILDCPP
ARG BUILDFTI
ARG BUILDSWIGPYTHON
ARG BUILDSWIGNODE
ARG BUILDSWIGJAVA
ARG BUILDEXAMPLES
ARG IPK
ARG RPM
ARG NPM
ARG BUILDTESTS
ARG WERROR
ARG CC
ARG CXX

# Configure Build Environment
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64/
ENV CC $CC
ENV CXX $CXX

# Copy sources
COPY . .

# Change Workdir to build directory
WORKDIR /usr/src/app/build

# Run cmake
RUN . $NVM_DIR/nvm.sh && cmake \
    -DSWIG_EXECUTABLE=/usr/bin/swig \
    -DSWIG_DIR:PATH=/usr/share/swig/3.0.10/ \
    -DBUILDDOC=$BUILDDOC \
    -DBUILDSWIGPYTHON=$BUILDSWIGPYTHON \
    -DBUILDSWIGNODE=$BUILDSWIGNODE \
    -DBUILDSWIGJAVA=$BUILDSWIGJAVA \
    -DBUILDEXAMPLES=$BUILDEXAMPLES \
    -DIPK=$IPK \
    -DRPM=$RPM \
    -DNPM=$NPM \
    -DBUILDTESTS=$BUILDTESTS \
    -DWERROR=$WERROR \
    ..

CMD make
