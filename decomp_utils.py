import sys
sys.dont_write_bytecode = True

import argparse
import re
from pathlib import Path
from colorama import Fore, Style, init

init(autoreset=True)

import build as build_script


# Templates
#
# Functions: $FUNC 004CE810 [IMPLEMENTED] || $FUNC 004CE810 [UNFINISHED]
# Global Vars: $GLOBAL 004CE810
# Bugs: $BUG
# TODO: $TODO

def count_lines():
    build_script.track_process(["cloc", "src_main"], "CountLines")

def parse_source_files():
    src_main_path = Path("src")
    implemented_functions = {}

    func_pattern = re.compile(r'//\s*\$FUNC\s+([0-9A-Fa-f]+)\s+\[(\w+)\]')

    source_files = list(src_main_path.glob("**/*.h")) + list(src_main_path.glob("**/*.cpp"))

    for file_path in source_files:
        try:
            with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                content = f.read()

            matches = func_pattern.findall(content)

            for address, status in matches:
                address = address.upper().zfill(8)
                implemented_functions[address] = {
                    "status": status.upper(),
                    "file": str(file_path.relative_to(src_main_path)),
                }

        except Exception as e:
            print(f"Warning: Could not read file {file_path}: {e}")

    return implemented_functions


def parse_functions_map(functions_map_path):
    ida_functions = {}

    with open(functions_map_path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            match = re.match(r'^(?:0x)?([0-9A-Fa-f]{6,8})(?:\s+(.+))?$', line)
            if not match:
                continue

            address = match.group(1).upper().zfill(8)
            func_name = match.group(2).strip() if match.group(2) else ""

            ida_functions[address] = func_name

    return ida_functions


def count_progress(namespace_filter=None):
    functions_map_path = Path("tools/Resources/functions_map.txt")
    if not functions_map_path.exists():
        print("Error: tools/Resources/functions_map.txt not found!")
        return

    try:
        ida_functions = parse_functions_map(functions_map_path)
    except Exception as e:
        print(f"Error reading functions_map.txt: {e}")
        return

    if not ida_functions:
        print("No valid function addresses found in functions_map.txt")
        return

    print("Parsing source files...")
    source_functions = parse_source_files()

    ida_addresses = set(ida_functions.keys())

    implemented_addresses = set()
    unfinished_addresses = set()

    for address, info in source_functions.items():
        if info["status"] == "IMPLEMENTED":
            implemented_addresses.add(address)
        elif info["status"] == "UNFINISHED":
            unfinished_addresses.add(address)
        else:
            print(f"Found an unknown $FUNC type for {address}")

    if namespace_filter:
        print_namespace_progress(
            namespace_filter,
            ida_functions,
            source_functions,
        )
        return

    total_ida_functions = len(ida_addresses)
    implemented_count = len(implemented_addresses)
    unfinished_count = len(unfinished_addresses)

    not_started_addresses = ida_addresses - implemented_addresses - unfinished_addresses
    not_started_count = len(not_started_addresses)

    print("\n" + "=" * 60)
    print("DECOMPILATION PROGRESS REPORT")
    print("=" * 60)
    print(f"Total functions in IDA:     {total_ida_functions}")
    print(f"Implemented functions:      {implemented_count}")
    print(f"Unfinished functions:       {unfinished_count}")
    print(f"Not started functions:      {not_started_count}")
    print("-" * 60)

    implemented_percentage = (implemented_count / total_ida_functions) * 100
    started_percentage = ((implemented_count + unfinished_count) / total_ida_functions) * 100

    print(f"Implementation progress:    {implemented_percentage:.1f}%")
    print(f"Started progress:           {started_percentage:.1f}%")
    print(f"Implemented vs Overall:     {implemented_count}/{total_ida_functions}")
    print("=" * 60)

    extra_functions = set(source_functions.keys()) - ida_addresses
    if extra_functions:
        print(f"\nWarning: Found {len(extra_functions)} functions in source files that are not in IDA map:")
        for address in sorted(extra_functions):
            info = source_functions[address]
            print(f"  {address} - {info['file']} [{info['status']}]")


def print_namespace_progress(namespace, ida_functions, source_functions):
    namespace_prefix = namespace + "::"

    matching_functions = [
        (address, name)
        for address, name in ida_functions.items()
        if name.startswith(namespace_prefix)
    ]

    matching_functions.sort(key=lambda item: item[0])

    print("\n" + "=" * 60)
    print(f"FUNCTIONS FOR {namespace}")
    print("=" * 60)

    if not matching_functions:
        print(f"No functions found in functions_map.txt for namespace: {namespace}")
        print("=" * 60)
        return

    implemented_count = 0
    unfinished_count = 0
    not_started_count = 0

    max_name_len = max(len(func_name) for _, func_name in matching_functions)

    for address, func_name in matching_functions:
        info = source_functions.get(address)

        if info is None:
            status = "NOT_STARTED"
            file_name = ""
            not_started_count += 1
        elif info["status"] == "IMPLEMENTED":
            status = "IMPLEMENTED"
            file_name = info["file"]
            implemented_count += 1
        elif info["status"] == "UNFINISHED":
            status = "UNFINISHED"
            file_name = info["file"]
            unfinished_count += 1
        else:
            status = info["status"]
            file_name = info["file"]

        raw_status = f"[{status}]"

        if status == "IMPLEMENTED":
            coloured_status = f"{Fore.GREEN}{raw_status}{Style.RESET_ALL}"
        elif status == "UNFINISHED":
            coloured_status = f"{Fore.YELLOW}{raw_status}{Style.RESET_ALL}"
        else:
            coloured_status = f"{Fore.RED}{raw_status}{Style.RESET_ALL}"

        status_padding = " " * (14 - len(raw_status))

        print(
            f"0x{address} "
            f"{func_name:<{max_name_len}} "
            f"{coloured_status}{status_padding} "
            f"{file_name}"
        )

    print("-" * 60)
    print(f"Total in namespace:         {len(matching_functions)}")
    print(f"{Fore.GREEN}Implemented:{Style.RESET_ALL}                {implemented_count}")
    print(f"{Fore.YELLOW}Unfinished:{Style.RESET_ALL}                 {unfinished_count}")
    print(f"{Fore.RED}Not started:{Style.RESET_ALL}                {not_started_count}")
    print("=" * 60)

def main():
    parser = argparse.ArgumentParser(
        description=f"Decomp utils for {build_script.PROJECT_NAME}."
    )

    parser.add_argument(
        "--count",
        action="store_true",
        help="Count the number of lines in .h and .cpp files.",
    )

    parser.add_argument(
        "--progress",
        nargs="?",
        const=True,
        metavar="NAMESPACE",
        help="Count implemented decomp functions. Optionally filter by namespace.",
    )

    args = parser.parse_args()

    if args.count:
        count_lines()

    if args.progress:
        namespace_filter = None if args.progress is True else args.progress
        count_progress(namespace_filter)

    if not args.progress and not args.count:
        print("Enter an option, --count or --progress.")


if __name__ == "__main__":
    main()