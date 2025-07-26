import sys
sys.dont_write_bytecode = True

import os
import subprocess
import argparse

import build as build_script

def count_lines():
    build_script.track_process(["cloc", "src_main"], "CountLines")


def count_progress():
    # To be implemented
    pass


def main():
    parser = argparse.ArgumentParser(
        description=f"Decomp utils for {build_script.PROJECT_NAME}."
    )
    parser.add_argument("--count", action="store_true", help="Count the number of lines in .h and .cpp files.")
    parser.add_argument("--progress", action="store_true", help="Count the number of implemented decomp functions.")

    args = parser.parse_args()

    if args.count:
        count_lines()

    if args.progress:
        count_progress()

if __name__ == "__main__":
    main()
