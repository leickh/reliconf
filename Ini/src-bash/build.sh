#!/usr/bin/env bash
# SPDX-License-Identifier: MPL-2.0

INVOCATION=$(pwd)
cd $(dirname $0)/..
LIBRARY_HOME=$(pwd)
cd $INVOCATION

COMPILER_OPTIONS="$@"

search_c_sources() {
    local SOURCE_PATH=$1
    local FUNCTION_INVOCATION=$(pwd)

    cd $SOURCE_PATH
    find . -type f | cut -c 3-
    cd $FUNCTION_INVOCATION
}

make_object_name() {
    local SOURCE_NAME=$1
    # Remove folder hierarchies from source name
    local FLAT_NAME=$(echo $SOURCE_NAME | tr "-" "_" | tr "/" "-")
    # Remove suffix and add new suffix '.o'
    ((LEN_NAME_WITHOUT_SUFFIX=${#FLAT_NAME}-2))
    local NAME_WITHOUT_SUFFIX=$(echo $FLAT_NAME | cut -c -$LEN_NAME_WITHOUT_SUFFIX)
    # Print out object name
    echo $NAME_WITHOUT_SUFFIX.o
}

compile_single_source() {
    local SOURCE_NAME=$1

    local OBJECT_NAME=$(make_object_name $SOURCE_NAME)

    gcc $COMPILER_OPTIONS -c -o $LIBRARY_HOME/.build/objects/$OBJECT_NAME $LIBRARY_HOME/src-c/$SOURCE_NAME -I $LIBRARY_HOME/inc-c
}

build_library() {
    SOURCE_LIST=$(search_c_sources $LIBRARY_HOME/src-c)

    rm -f $LIBRARY_HOME/.build/objects/*.o
    mkdir -p $LIBRARY_HOME/.build/objects

    for SOURCE_ITEM in $SOURCE_LIST;
    do
        compile_single_source $SOURCE_ITEM
    done

    ar -rvs .build/configury-ini.a \
        $LIBRARY_HOME/.build/objects/*.o
}

build_library
