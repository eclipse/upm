#!/usr/bin/env bash

# Folders Location
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="${DIR}/../.."

# Find test scripts
TEST_SCRIPTS="$( find ${DIR} | grep test- )"
TEST_EXEC_RESULT=0

for i in ${TEST_SCRIPTS}
do
  CURRENT_TEST_NAME="$( basename $i)"
  CURRENT_TEST_PATH="${i}"

  # Execute the test
  echo "Running ${CURRENT_TEST_NAME}"
  bash ${CURRENT_TEST_PATH}
  EXIT_STATUS=$?

  if [ "${EXIT_STATUS}" == "0" ]; then
    echo "Passed ${CURRENT_TEST_NAME}";
  else
    TEST_EXEC_RESULT=1
    echo "Fail ${CURRENT_TEST_NAME}";
  fi
done

if [ "${TEST_EXEC_RESULT}" == "0" ]; then
  echo "Success";
else
  echo "Fail";
fi

# Generate Consolidated Coverage Report
if [ ! -z "${COVERAGE}" ]; then
  ISTANBUL_CMD="${ROOT_DIR}/node_modules/.bin/istanbul"
  ${ISTANBUL_CMD} report text lcov;
fi

exit ${TEST_EXEC_RESULT}
