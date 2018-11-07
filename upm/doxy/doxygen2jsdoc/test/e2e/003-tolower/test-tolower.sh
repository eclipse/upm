#!/usr/bin/env bash

set -e

# Folders Location
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="${DIR}/../../../"
WORKSPACE_DIR="${DIR}/workspace"
ASSETS_DIR="${DIR}/assets"

# Create workspace directory if missing
if [ ! -d "${WORKSPACE_DIR}" ]; then
  mkdir ${WORKSPACE_DIR};
fi
# Copy assets directory to workspace directory
rm -fr ${WORKSPACE_DIR}/assets
cp -r ${ASSETS_DIR} ${WORKSPACE_DIR}/assets
# Go to workspace directory to execute docgen
cd ${WORKSPACE_DIR}
# Run docgen.js help command with coverage report
ISTANBUL_CMD="${ROOT_DIR}/node_modules/.bin/istanbul"
ISTANBUL_OPTS="--root ${ROOT_DIR} --report json --print none"
TOLOWER_OPTS="-i ${WORKSPACE_DIR}/assets"
TOLOWER_CMD="${ROOT_DIR}/tolower.js"

if [ -z "${COVERAGE}" ]; then
  ${TOLOWER_CMD} ${TOLOWER_OPTS};
else
  ${ISTANBUL_CMD} cover ${TOLOWER_CMD} ${ISTANBUL_OPTS} -- ${TOLOWER_OPTS};
fi
