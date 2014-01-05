#!/bin/bash
DIR=$(dirname $0) &> /dev/null
pushd "${DIR}/bin" &> /dev/null
./jewelminer
popd &> /dev/null
