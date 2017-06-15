FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive

# Prepare apt-get
RUN apt-get update && apt-get -y --no-install-recommends install software-properties-common

# Add Mraa Repository
RUN add-apt-repository ppa:mraa/mraa && \
  # Update apt-get
  apt-get -y update && \
  # Install apt-utils
  apt-get -y --no-install-recommends install apt-utils && \
  # Main Build Dependencies
  apt-get -y --no-install-recommends install git build-essential cmake swig clang-3.8 gcc-4.8 g++-4.8 libpthread-stubs0-dev pkg-config wget unzip \
  # Mraa Build Dependencies
  libmraa1 libmraa-dev mraa-tools python-mraa python3-mraa libmraa-java \
  # Docs Build Dependencies
  python-sphinx doxygen graphviz \
  # Python Build Dependencies
  python python-dev python3 python3-dev python-pip python3-pip \
  # Java Build Dependencies
  default-jre default-jdk \
  # Sensor Specific Build Dependencies
  libjpeg-dev

# Configure Compiler Environment
ARG CC
ARG CXX
ENV CC $CC
ENV CXX $CXX

# Install GCC6 too
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get -y --no-install-recommends install gcc-6 g++-6

# Configure Java Home
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64/

# Install libbacnet 0.3.12
RUN wget https://downloads.sourceforge.net/project/bacnet/bacnet-stack/bacnet-stack-0.8.3/bacnet-stack-0.8.3.zip && \
    unzip -qq bacnet-stack-0.8.3.zip && cd bacnet-stack-0.8.3 && \
    BACDL_DEFINE=-DBACDL_MSTP=1 MAKE_DEFINE=-fPIC make clean all

# Install libmodbus 3.1.4
RUN wget http://libmodbus.org/releases/libmodbus-3.1.4.tar.gz && \
    tar xf libmodbus-3.1.4.tar.gz && cd libmodbus-3.1.4 && \
    ./configure && make -j8 && make install

# Install openzwave
RUN apt-get update && apt-get -y --no-install-recommends install libudev-dev && \
    git clone --depth 1 https://github.com/OpenZWave/open-zwave.git && cd open-zwave && make -j8 install && \
    echo "/usr/local/lib64" >> /etc/ld.so.conf.d/openzwave.conf

# Using a custom SWIG version
# RUN wget https://downloads.sourceforge.net/project/swig/swig/swig-3.0.10/swig-3.0.10.tar.gz && \
##    tar xf swig-3.0.10.tar.gz && cd swig-3.0.10 && \
##    apt-get update && apt-get -y --no-install-recommends install libpcre3 libpcre3-dev && \
##    ./configure --prefix=/usr/ && make && make install && cd ..

# Node.js Build Dependencies
RUN wget -q -O - https://raw.githubusercontent.com/creationix/nvm/v0.33.2/install.sh | bash
ARG NODE_VERSION
ENV NVM_DIR /root/.nvm
RUN . $NVM_DIR/nvm.sh && nvm install $NODE_VERSION && nvm use $NODE_VERSION && \
    npm install -g node-gyp && node-gyp install

# Install Test Dependencies
RUN pip install chardet && pip3 install chardet

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

# Copy sources
COPY . .

# Temp solution for libbacnet install
COPY bacnet/libbacnet.pc /usr/lib/pkgconfig/libbacnet.pc
RUN cp /bacnet-stack-0.8.3/lib/libbacnet.a /usr/lib/libbacnet.a
RUN mkdir /usr/include/bacnet && cp /bacnet-stack-0.8.3/include/* /usr/include/bacnet/

# Change Workdir to build directory
WORKDIR /usr/src/app/build

# Run cmake
RUN . $NVM_DIR/nvm.sh && cmake \
    -DSWIG_EXECUTABLE=/usr/bin/swig \
#   -DSWIG_DIR:PATH=/usr/share/swig/3.0.10/ \
    -DBUILDDOC=$BUILDDOC \
    -DBUILDCPP=$BUILDCPP \
    -DBUILDFTI=$BUILDFTI \
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
