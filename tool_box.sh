#!/usr/bin/env bash

# Obtain the path to the work directory
RELATIVE_SOURCE_PATH=`dirname ${BASH_SOURCE[@]}`
SOURCE_PATH=`readlink -f ${RELATIVE_SOURCE_PATH}`

find . -name "*.md" | xargs sed -i 's/[ \t]*$//'

clang-format -i -style=file \
    `find ${SOURCE_PATH}/kts/main -type f -name '*.c'` \
    `find ${SOURCE_PATH}/kts/main -type f -name '*.h'`

