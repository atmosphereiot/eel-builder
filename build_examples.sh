#!/bin/bash

# Create build directory to store built EELs
mkdir -p build
rm -rf build/*

# Get the directory the script lives in
# Some bash gore from stack overflow
# This is just so the script can be called from anywhere
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

pushd ${DIR}

# Destination for built EELS
eelDir=${DIR}/build

for i in $(find examples -name "metadata.json"); do
    echo "Updating EEL from ${i}"

    # EEL source folder
    eelSrc=${DIR}/${i%/*}

    # Generate the actual EEL
    node ${DIR}/eelbuilder.js --generate --dir=${eelSrc} --dest=${eelDir}
done

popd