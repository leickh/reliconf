#!/usr/bin/env python3

import glob
import os
import pathlib
import subprocess

library_path = pathlib.Path(__file__).parents[3]
unit_test_binary_path = library_path.joinpath(".build", "UnitTests")

for unit_test_name in os.listdir(unit_test_binary_path):
    unit_test_path = unit_test_binary_path.joinpath(unit_test_name)

    print(f"\033[92mnote(build-system) :: Executing unit test: '{unit_test_name}'.\033[0m")

    elf_files = glob.glob("*.elf", root_dir=unit_test_path)
    if elf_files != [ ]:
        for elf_file in elf_files:
            subprocess.call([str(unit_test_path.joinpath(elf_file))])

    exe_files = glob.glob("*.exe", root_dir=unit_test_path)
    if exe_files != [ ]:
        for exe_file in exe_files:
            subprocess.call([str(unit_test_path.joinpath(exe_file))])

    macho_files = glob.glob("*.macho", root_dir=unit_test_path)
    if macho_files != [ ]:
        for macho_file in macho_files:
            subprocess.call([str(unit_test_path.joinpath(macho_file))])


