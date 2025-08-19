#!/usr/bin/env bash
# SPDX-License-Identifier: MPL-2.0

INVOCATION=$(pwd)
cd $(dirname $0)
LIBRARY_HOME=$(pwd)
cd $INVOCATION

case $1 in
    "b" | "build")
        $LIBRARY_HOME/src-bash/build.sh "${@:2}"
        ;;
esac
