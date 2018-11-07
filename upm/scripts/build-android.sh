#!/usr/bin/env bash
#
# The script is used to build .aar packages for Upm modules.
#
# Author: Nicolas Oliver <dario.n.oliver@intel.com>
#
# All environment variables used are passed from docker-compose.
# Those environment variables can also be declared in the host,
# and this script can be used to build .aar packages without using
# docker-compose.
#
# Environment variables:
#    - NDK_HOME - Path to Android NDK install folder. i.e. /opt/android-ndk-r14b/
#    - MRAA_INSTALL_DIR - Path to Mraa library install folder. i.e /opt/mraa/install/
#    - JAVA_HOME - Path to Java install folder. i.e. /usr/lib/jvm/java-8-openjdk-amd64/
#
# IMPORTANT: MRAA_INSTALL_DIR must contain an mraa version built for Android Things.
set -e

# Check required environment variables and exit if they are not set
function check_environment {
  VAR_NAME=$1
  VAR_VALUE=$2
  # Check required parameters
  VAR_NAME=${VAR_NAME:?value not provided}
  # Chek if variable is set
  if [ -z "${VAR_VALUE}" ]; then
    echo "Required environment variable ${VAR_NAME} is not defined. Exiting..."
    exit 1;
  else
    echo "Required environment variable ${VAR_NAME} is set."
  fi
}

# Check for required environment variables
check_environment "NDK_HOME" ${NDK_HOME}
check_environment "MRAA_INSTALL_DIR" ${MRAA_INSTALL_DIR}
check_environment "JAVA_HOME" ${JAVA_HOME}

PKG_CONFIG_LIBDIR="$NDK_HOME/platforms/android-24/arch-x86/usr/lib"
PKG_CONFIG_LIBDIR=$PKG_CONFIG_LIBDIR":$MRAA_INSTALL_DIR/lib/pkgconfig/"
export PKG_CONFIG_LIBDIR

cmake \
  -DANDROID_COMPILER_FLAGS_CXX='-std=c++11' \
  -DANDROID_PIE=1 \
  -DANDROID_PLATFORM=android-24 \
  -DANDROID_STL_FORCE_FEATURES=ON \
  -DANDROID_STL=c++_shared \
  -DANDROID_TOOLCHAIN_NAME=x86-i686 \
  -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake \
  -DCMAKE_FIND_ROOT_PATH=$MRAA_INSTALL_DIR \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DJAVA_AWT_INCLUDE_PATH=$JAVA_HOME/include \
  -DJAVA_AWT_LIBRARY=$JAVA_HOME/jre/lib/amd64/libjawt.so \
  -DJAVA_INCLUDE_PATH=$JAVA_HOME/include \
  -DJAVA_INCLUDE_PATH2=$JAVA_HOME/include/linux \
  -DJAVA_JVM_LIBRARY=$JAVA_HOME/jre/lib/amd64/server/libjvm.so \
  -DBUILDSWIGJAVA=$BUILDSWIGJAVA \
  -DBUILDSWIGNODE=$BUILDSWIGNODE \
  -DBUILDSWIGPYTHON=$BUILDSWIGPYTHON \
  -DWERROR=$WERROR \
  -H. \
  -Bbuild

make -j8 -Cbuild

# Anotate the .java src from doxygen
find src/ -name "*.i" > build/upm.i.list
#../doxy/doxyport build/upm.i.list \
#  -s src/interfaces/,src/bacnetmstp,src/bmg160,src/bma250e,src/bmm150 \
#  -m doxy/samples.mapping.txt \
#  -d build/src/ \
#  --convert-protected-to-private \
#  -o build/upm-java-files.txt \
#  -c build/compile_commands.json

gen_and_upload() {
    # Skip any directories without .java src files
    if [ $(ls *.java | wc -l) -eq "0" ]; then
        return
    fi

    # Get the MRAA version string from the .pom file
    VERSION=$(ls *.pom | perl -nle 'print $2 if /^(.*)-(\d+\.\d+\.\d+)\.pom/')
    # Get the MRAA name string from the .pom file
    LIB_NAME=$(ls *.pom | perl -nle 'print $1 if /^(.*)-(\d+\.\d+\.\d+)\.pom/')

    # Bundle upload directory
    PKG_BUNDLE_DIR=BUNDLE_DIR
    mkdir -p ${PKG_BUNDLE_DIR}

    # Create the aar directory structure
    AAR_DIRECTORY="$LIB_NAME-$VERSION"
    mkdir -p $AAR_DIRECTORY/jni/x86
    mkdir -p $AAR_DIRECTORY/res/values

    # Write out res/values/values.xml to the aar
    echo "${values_xml/XXX_LIBRARY/$LIB_NAME}" > $AAR_DIRECTORY/res/values/values.xml

    # Write out AndroidManifest.xml to the aar
    _tmp_manifest=${AndroidManifest_xml/XXX_PACKAGE/$JAVA_PKG_PREFIX_DOT}
    _tmp_manifest=${_tmp_manifest/XXX_LIBRARY/$LIB_NAME}
    echo "${_tmp_manifest/XXX_VERSION/$VERSION}" > $AAR_DIRECTORY/AndroidManifest.xml

    # Copy over the so's
    cp *.so $AAR_DIRECTORY/jni/x86

    # Create javadoc jar
    javadoc *.java -d $LIB_NAME-javadoc
    jar cf $PKG_BUNDLE_DIR/$LIB_NAME-$VERSION-javadoc.jar $LIB_NAME-javadoc/*

    # Create sources jar file for upload
    jar cf $PKG_BUNDLE_DIR/$LIB_NAME-$VERSION-sources.jar *.java

    # Copy pom file
    cp $LIB_NAME-$VERSION.pom $PKG_BUNDLE_DIR

    # Copy the java module jar to classes.jar
    cp $LIB_NAME.jar $AAR_DIRECTORY/classes.jar

    # Create the .aar
    pushd $AAR_DIRECTORY
        jar cf ../$PKG_BUNDLE_DIR/$LIB_NAME-$VERSION.aar *
    popd

    # Create the bundle file
    pushd $PKG_BUNDLE_DIR
        # Create the tar bundle
        jar cvf ../$LIB_NAME-$VERSION-bundle.jar *
    popd
}

# Template for res/values/values.xml in aar
values_xml='<?xml version="1.0" encoding="utf-8"?><resources><string name="app_name">XXX_LIBRARY</string></resources>'

# Template for AndroidManifest.xml in aar
AndroidManifest_xml='<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="XXX_PACKAGE.XXX_LIBRARY"
    android:versionCode="1"
    android:versionName="XXX_VERSION" >
    <uses-sdk android:minSdkVersion="23" android:targetSdkVersion="25"/>
    <application android:allowBackup="true" android:label="@string/app_name" android:supportsRtl="true" />
</manifest>'

# Package prefix
JAVA_PKG_PREFIX_DOT="io.mraa.at.upm"

# Work in the UPM build directory
cd build/src

# For each directory in build/src which contains a .pom file...
for lib_dir in `ls -d */`; do
    if ls $lib_dir/upm_*.pom 1> /dev/null 2>&1; then
        pushd $lib_dir
            # Allow for spawning multiple if necessary (&)
            gen_and_upload
        popd
    fi
done
