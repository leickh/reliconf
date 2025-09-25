#!/usr/bin/env python3

import glob
import os
import pathlib
import subprocess

library_path = pathlib.Path(__file__).parents[3]

include_paths = [ library_path.joinpath("Core", "inc-c"), library_path.joinpath("Reader", "inc-c") ]

def build_library_module(module_name: str, output_name: str):
    core_path = library_path.joinpath(module_name)

    # List sources

    core_source_path = core_path.joinpath("src-c")

    source_list = list[str]()
    for folder_path, folders, files in os.walk(core_source_path):
        base_name = folder_path[len(str(core_source_path)) + 1:]

        for file_name in files:
            source_list.append(str(pathlib.Path(base_name).joinpath(file_name)))

    # Build Sources

    object_root_folder_path = library_path.joinpath(".build", module_name)

    # Delete old objects

    for object_file in glob.glob("*.o", root_dir=object_root_folder_path):
        object_file_path = str(pathlib.Path(object_root_folder_path).joinpath(object_file))
        try:
            # print(f"info(build-system) :: Removed residue file: '{object_file_path}'")
            os.remove(object_file_path)
        except:
            print(f"\033[95mwarn(build-system) :: Failed removing object file at: '{object_file_path}'.\033[0m")
            pass

    # Create folder if it doesn't exist yet.

    try:
        os.makedirs(object_root_folder_path)
        print(f"\033[92mnote(build-system) :: Created new object folder at: '{object_root_folder_path}'.\033[0m")
    except:
        pass

    object_paths = [ ]
    for source_name in source_list:

        object_name = str(source_name).replace("-", "_").replace("/", "-") + ".o"
        object_path = str(object_root_folder_path.joinpath(object_name))
        object_paths += [ object_path ]

        gcc_command = [ "gcc", "-c" ]
        gcc_command += [ "-o", object_path ]
        gcc_command += [ str(core_source_path.joinpath(source_name)) ]
        for include_path in include_paths:
            gcc_command += [ "-I", include_path ]

        subprocess.call(gcc_command)

    output_path = str(library_path.joinpath(".build", output_name))

    try:
        os.remove(output_path)
    except:
        pass

    link_archiver_command = [ "ar", "-rs", output_path ] + object_paths
    subprocess.call(link_archiver_command)



def build_library():
    build_library_module("Core", "reliconf-ini-core.a")
    build_library_module("Reader", "reliconf-ini-reader.a")

build_library()
