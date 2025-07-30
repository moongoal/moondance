#!/bin/bash

if [[ $# -eq 0 ]]; then
  BUILD_TYPE=Debug
else
  BUILD_TYPE=$1
fi

pipenv run -- \
    conan build \
        -d full_deploy \
        -pr:a default \
        -s:a build_type=$BUILD_TYPE \
        --build missing \
        .
