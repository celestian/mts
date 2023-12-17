#!/usr/bin/env bash

# Obtain the path to the work directory
RELATIVE_SOURCE_PATH=`dirname ${BASH_SOURCE[@]}`
SOURCE_PATH=`readlink -f ${RELATIVE_SOURCE_PATH}`

MCU0='/dev/ttyUSB0'
MCU1='/dev/ttyUSB1'
MCU2='/dev/ttyUSB2'

if [ "$1" == "" ]
then

    echo "./tool_box.sh (pretty | clean | fclean)"
    echo "./tool_box.sh kts (build | flash)"
    echo "./tool_box.sh cts (build | flash)"

elif [ "$1" == "pretty" ]
then

    find . -name "*.md" | xargs sed -i 's/[ \t]*$//'

    clang-format -i -style=file \
        `find ${SOURCE_PATH}/kts/main -type f -name '*.c'` \
        `find ${SOURCE_PATH}/kts/main -type f -name '*.h'` \
        `find ${SOURCE_PATH}/kts/main/include -type f -name '*.h'`

    # cppcheck -I /home/celestian/Projects/esp/esp-idf/components --enable=all --cppcheck-build-dir=build --std=c11 main

elif [ "$1" == "clean" ]
then

    idf.py -C ${SOURCE_PATH}/kts clean

elif [ "$1" == "fclean" ]
then

    idf.py -C ${SOURCE_PATH}/kts fullclean

elif [ "$1" == "kts" ] && [ "$2" == "build" ]
then

    idf.py -C ${SOURCE_PATH}/kts build

elif [ "$1" == "kts" ] && [ "$2" == "flash" ]
then

    idf.py -C ${SOURCE_PATH}/kts -p ${MC0} flash monitor

elif [ "$1" == "cts" ] && [ "$2" == "build" ]
then

    idf.py -C ${SOURCE_PATH}/cts build

elif [ "$1" == "cts" ] && [ "$2" == "flash" ]
then

    idf.py -C ${SOURCE_PATH}/kts -p ${MC1} flash monitor

fi
