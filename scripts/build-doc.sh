#!/usr/bin/env bash

set -x
set -e

# Folders Location
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="${DIR}/.."
BUILD_DIR="${ROOT_DIR}/build"
BUILD_LOGS_DIR="${BUILD_DIR}/logs"

# Install doxygen2jsdoc dependencies
cd ${ROOT_DIR}/doxy/doxygen2jsdoc && npm install

# Install doxyport dependencies
cd ${ROOT_DIR}/doxy/doxyport && make setup

# Create logs folder if not exists
mkdir -p ${BUILD_LOGS_DIR}

# Make Documentation
cd ${ROOT_DIR} && make -j8 -Cbuild 2> ${BUILD_LOGS_DIR}/build-doc.log

# Make Java Documentation
cd ${BUILD_DIR} && find ../src/ -name "*.i" > upm.i.list && \
../doxy/doxyport/doxyport upm.i.list \
    --cmake ./compile_commands.json \
    --source ../src/interfaces/,../src/bacnetmstp,src \
    --destination src/ \
    --convert-protected-to-private \
    --output upm-java-files.txt \
    --mapping ../doxy/samples.mapping.txt 1> ${BUILD_LOGS_DIR}/doxyport.log && \
rm -fr java && mkdir java && cd java && \
doxygen ../Doxyfile-java 2> ${BUILD_LOGS_DIR}/doxygen-java.log

