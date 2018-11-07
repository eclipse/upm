#!/usr/bin/env bash

set -e

# Folders Location
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="${DIR}/../../.."
WORKSPACE_DIR="${DIR}/workspace"
XML_DIR="${WORKSPACE_DIR}/xml"
UPM_VERSION="v1.3.0"

# Create workspace directory if missing
if [ ! -d "${WORKSPACE_DIR}" ]; then mkdir ${WORKSPACE_DIR}; fi
# Download xml.tar.gz file to test if missing
if [ ! -d "${XML_DIR}" ]; then
    mkdir ${XML_DIR} && cd ${XML_DIR}
    curl https://iotdk.intel.com/docs/upm/${UPM_VERSION}/xml.tar.gz | tar xz
fi
# Go Back to workspace directory to execute docgen
cd ${WORKSPACE_DIR}
# Run docgen.js command with coverage report
ISTANBUL_CMD="${ROOT_DIR}/node_modules/.bin/istanbul"
ISTANBUL_OPTS="--root ${ROOT_DIR} --report json --print none"
DOCGEN_OPTS="-m upm -i xml -f yuidoc,ternjs,jsdoc --strict -o output"
DOCGEN_CMD="${ROOT_DIR}/docgen.js"

if [ -z "${COVERAGE}" ]; then
  ${DOCGEN_CMD} ${DOCGEN_OPTS};
else
  ${ISTANBUL_CMD} cover ${DOCGEN_CMD} ${ISTANBUL_OPTS} -- ${DOCGEN_OPTS};
fi
