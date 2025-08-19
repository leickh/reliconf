#!/usr/bin/env bash
# SPDX-License-Identifier: MPL-2.0

INVOCATION=$(pwd)
cd $(dirname $0)/../..
LIBRARY_ROOT=$(pwd)
cd ..
CONFIGURY_ROOT=$(pwd)
cd $INVOCATION


COMPILER_OPTIONS="$@"

search_c_sources() {
    local SOURCE_PATH="$1"
    local FUNCTION_INVOCATION=$(pwd)

    cd $SOURCE_PATH
    find . -type f | cut -c 3-
    cd $FUNCTION_INVOCATION
}

replace_path_elements() {
    local INCLUDE_SEARCH_PATH="$1"

    INCLUDE_SEARCH_PATH=${INCLUDE_SEARCH_PATH//'${INI}'/"$CONFIGURY_ROOT/Ini"}
    INCLUDE_SEARCH_PATH=${INCLUDE_SEARCH_PATH//'${CONFIGURY}'/"$CONFIGURY_ROOT"}
    echo "$INCLUDE_SEARCH_PATH"
}

make_include_search_path_arguments() {
    local INCLUDE_LIST_PATH="$1"

    while read -e LINE;
    do
        # Skip empty lines
        if [[ "$LINE" == "" ]];
        then
            continue
        fi

        # If this line is a comment, skip it
        if [[ "$LINE" == "#"* ]];
        then
            continue
        fi

        echo "-I $(replace_path_elements "$LINE")"
    done <$INCLUDE_LIST_PATH
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
    local SOURCE_NAME="$1"
    local INCLUDE_STATEMENTS="$2"

    local OBJECT_NAME=$(make_object_name $SOURCE_NAME)

    gcc -c \
        $COMPILER_OPTIONS \
        -o $LIBRARY_ROOT/.build/objects/reader/$OBJECT_NAME \
        $LIBRARY_ROOT/Reader/src-c/$SOURCE_NAME \
        $INCLUDE_STATEMENTS
}

build_library() {
    local SOURCE_LIST=$(search_c_sources $LIBRARY_ROOT/Reader/src-c)

    rm -f $LIBRARY_ROOT/.build/objects/reader/*.o
    mkdir -p $LIBRARY_ROOT/.build/objects/reader

    local INCLUDE_STATEMENTS=$(make_include_search_path_arguments $LIBRARY_ROOT/Reader/includes.txt)

    for SOURCE_ITEM in $SOURCE_LIST;
    do
        compile_single_source "$SOURCE_ITEM" "$INCLUDE_STATEMENTS"
    done

    rm -f $LIBRARY_ROOT/.build/libconfigury-ini.a
    ar -rvs $LIBRARY_ROOT/.build/libconfigury-ini.a \
        $LIBRARY_ROOT/.build/objects/reader/*.o
}

build_library
