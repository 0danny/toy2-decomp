import sys
sys.dont_write_bytecode = True

import os
import subprocess
import argparse
import re
from pathlib import Path

import build as build_script

def count_lines():
    build_script.track_process(["cloc", "src_main"], "CountLines")


def parse_source_files():
    src_main_path = Path("src_main")
    implemented_functions = {}
    
    # Pattern to match function comments with address and status
    func_pattern = re.compile(r'//\s*\$FUNC\s+([0-9A-Fa-f]+)\s+\[(\w+)\]')
    
    # Find all .h and .cpp files
    source_files = list(src_main_path.glob("**/*.h")) + list(src_main_path.glob("**/*.cpp"))
    
    for file_path in source_files:
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                
                # Find all function markers
                matches = func_pattern.findall(content)
                for address, status in matches:
                    # Normalize address to uppercase
                    address = address.upper().zfill(8)
                    implemented_functions[address] = {
                        'status': status.upper(),
                        'file': str(file_path.relative_to(src_main_path))
                    }
        except Exception as e:
            print(f"Warning: Could not read file {file_path}: {e}")
    
    return implemented_functions


def count_progress():
    # Check if functions_map.txt exists
    functions_map_path = Path("decomp_utils/functions_map.txt")
    if not functions_map_path.exists():
        print("Error: decomp_utils/functions_map.txt not found!")
        return
    
    # Load function map from IDA
    ida_functions = set()
    try:
        with open(functions_map_path, "r") as f:
            for line in f:
                line = line.strip()
                if line:
                    # Handle format like "0x401000" or just "401000"
                    if line.startswith('0x') or line.startswith('0X'):
                        address = line[2:].upper().zfill(8)
                    else:
                        # Extract hexadecimal addresses if no 0x prefix
                        address_match = re.search(r'([0-9A-Fa-f]{6,8})', line)
                        if address_match:
                            address = address_match.group(1).upper().zfill(8)
                        else:
                            continue
                    ida_functions.add(address)
    except Exception as e:
        print(f"Error reading functions_map.txt: {e}")
        return
    
    if not ida_functions:
        print("No valid function addresses found in functions_map.txt")
        return
    
    # Parse source files for implemented functions
    print("Parsing source files...")
    implemented_functions = parse_source_files()
    
    # Count statistics
    total_ida_functions = len(ida_functions)
    implemented_count = 0
    unfinished_count = 0
    not_started_count = 0
    
    # Categorize functions
    implemented_addresses = set()
    unfinished_addresses = set()
    
    for address, info in implemented_functions.items():
        if info['status'] == 'IMPLEMENTED':
            implemented_count += 1
            implemented_addresses.add(address)
        elif info['status'] == 'UNFINISHED':
            unfinished_count += 1
            unfinished_addresses.add(address)
    
    # Functions that are in IDA but not found in source files
    not_started_addresses = ida_functions - implemented_addresses - unfinished_addresses
    not_started_count = len(not_started_addresses)
    
    # Print results
    print("\n" + "="*60)
    print("DECOMPILATION PROGRESS REPORT")
    print("="*60)
    print(f"Total functions in IDA:     {total_ida_functions}")
    print(f"Implemented functions:      {implemented_count}")
    print(f"Unfinished functions:       {unfinished_count}")
    print(f"Not started functions:      {not_started_count}")
    print("-"*60)
    
    if total_ida_functions > 0:
        implemented_percentage = (implemented_count / total_ida_functions) * 100
        started_percentage = ((implemented_count + unfinished_count) / total_ida_functions) * 100
        print(f"Implementation progress:    {implemented_percentage:.1f}%")
        print(f"Started progress:           {started_percentage:.1f}%")
        print(f"Implemented vs Overall:     {implemented_count}/{total_ida_functions}")
    
    print("="*60)
    
    # Check for functions in source but not in IDA (potential issues)
    extra_functions = set(implemented_functions.keys()) - ida_functions
    if extra_functions:
        print(f"\nWarning: Found {len(extra_functions)} functions in source files that are not in IDA map:")
        for address in sorted(extra_functions):
            info = implemented_functions[address]
            print(f"  {address} - {info['file']} [{info['status']}]")


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
