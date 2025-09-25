#!/usr/bin/env python3

import glob
import os
import os.path
import pathlib
import platform
import subprocess

library_path = pathlib.Path(__file__).parents[3]
unit_test_root = library_path.joinpath("Tests", "unit-tests")

gcc_include_statements = []
gcc_include_statements += [ "-I", str(library_path.joinpath("Core", "inc-c")) ]
gcc_include_statements += [ "-I", str(library_path.joinpath("Reader", "inc-c")) ]
gcc_include_statements += [ "-I", str(library_path.joinpath("Utility", "inc-c")) ]

def build_single_unit_test(test_name: str):

    # List sources

    source_path = unit_test_root.joinpath(test_name, "src-c")

    source_list = list[str]()
    for folder_path, folders, files in os.walk(source_path):
        base_name = folder_path[len(str(source_path)) + 1:]

        for file_name in files:
            source_list.append(str(pathlib.Path(base_name).joinpath(file_name)))

    # If the test is empty: Ignore it

    if source_list == [ ]:
        return

    # Delete old objects

    test_build_root = library_path.joinpath(".build", "UnitTests", test_name)

    for object_file in glob.glob("*.o", root_dir=test_build_root):
        object_file_path = str(pathlib.Path(test_build_root).joinpath(object_file))
        try:
            # print(f"info(build-system) :: Removed residue file: '{object_file_path}'")
            os.remove(object_file_path)
        except:
            print(f"\033[95mwarn(build-system) :: Failed removing object file at: '{object_file_path}'.\033[0m")
            pass

    # Create folder if it doesn't exist yet.

    try:
        os.makedirs(test_build_root)
        print(f"\033[92mnote(build-system) :: Created new object folder at: '{test_build_root}'.\033[0m")
    except:
        pass

    # Build Sources

    object_paths = [ ]
    for source_name in source_list:

        object_name = str(source_name).replace("-", "_").replace("/", "-") + ".o"
        object_path = str(test_build_root.joinpath(object_name))
        object_paths += [ object_path ]

        gcc_command = [ "gcc", "-c" ]
        gcc_command += [ "-o", object_path ]
        gcc_command += [ str(source_path.joinpath(source_name)) ]
        gcc_command += gcc_include_statements

        subprocess.call(gcc_command)

    executable_suffix = ".elf"
    if platform.system() == "Windows":
        executable_suffix = ".exe"

    # @todo: Apple systems aren't tested at the moment
    if platform.system() == "Darwin":
        executable_suffix = ".macho" # Mach-O is the executable file format on modern mainstream Apple systems.

    output_path = str(test_build_root.joinpath(test_name)) + executable_suffix

    try:
        os.remove(output_path)
    except:
        pass

    core_library_path = library_path.joinpath(".build", "reliconf-ini-core.a")
    reader_library_path = library_path.joinpath(".build", "reliconf-ini-reader.a")

    if not os.path.isfile(core_library_path):
        print("\033[95mwarn(build-system) :: Library wasn't built before the tests. Building library...\033[0m")
        subprocess.call([ library_path.joinpath("Tools", "src-py3", "builder", "build_library.py") ])

    final_output_command = [ "gcc", "-o", output_path, core_library_path, reader_library_path ] + object_paths
    subprocess.call(final_output_command)

def build_all_unit_tests():
    test_root_folder_path = library_path.joinpath("Tests")

    for unit_test_item in os.listdir(test_root_folder_path.joinpath("unit-tests")):
        build_single_unit_test(unit_test_item)

build_all_unit_tests()
