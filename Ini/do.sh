#!/usr/bin/env bash

INVOCATION_PATH="$(pwd)"
cd "$(dirname $0)"
LIBRARY_PATH="$(pwd)"
cd "$INVOCATION_PATH"


case "$1" in
    "b" | "build")
        "$LIBRARY_PATH/Tools/src-py3/builder/build_library.py" "${@:2}"
        "$LIBRARY_PATH/Tools/src-py3/tester/build_tests.py" "${@:2}"
        ;;

    "bl" | "build-library")
        "$LIBRARY_PATH/Tools/src-py3/builder/build_library.py" "${@:2}"
        ;;

    "bt" | "build-tests")
        "$LIBRARY_PATH/Tools/src-py3/tester/build_tests.py" "${@:2}"
        ;;

    "ut" | "unit-test")
        "$LIBRARY_PATH/Tools/src-py3/tester/run_unit_tests.py" "${@:2}"
        ;;
esac
