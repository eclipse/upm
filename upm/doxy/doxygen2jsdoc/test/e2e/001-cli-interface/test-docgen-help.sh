#!/usr/bin/env bash

set -e

# Folders Location
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="${DIR}/../../../"
WORKSPACE_DIR="${DIR}/workspace"

# Create workspace directory if missing
if [ ! -d "${WORKSPACE_DIR}" ]; then mkdir ${WORKSPACE_DIR}; fi
# Go to workspace directory to execute docgen
cd ${WORKSPACE_DIR}
# Run docgen.js help command with coverage report
ISTANBUL_CMD="${ROOT_DIR}/node_modules/.bin/istanbul"
ISTANBUL_OPTS="--root ${ROOT_DIR} --report json --print none"
DOCGEN_OPTS="--help"
DOCGEN_CMD="${ROOT_DIR}/docgen.js"

if [ -z "${COVERAGE}" ]; then
  ${DOCGEN_CMD} ${DOCGEN_OPTS};
else
  ${ISTANBUL_CMD} cover ${DOCGEN_CMD} ${ISTANBUL_OPTS} -- ${DOCGEN_OPTS};
fi
