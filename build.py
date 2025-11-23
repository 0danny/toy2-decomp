import sys
sys.dont_write_bytecode = True

import os
import subprocess
import argparse

# Constants
PROJECT_NAME = "toy2decomp"
BUILD_FOLDER = "build"

def track_process(command, custom_name=""):
    proc = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    name = command[0] if custom_name == "" else custom_name

    for line in proc.stdout:
        print(f"[{name}]: {line.strip()}")

    proc.wait()
    if proc.returncode != 0:
        raise subprocess.CalledProcessError(proc.returncode, command)


def set_environment():
    vs6_bin = os.environ.get('VS6_BIN_PATH')
    if not vs6_bin:
        print("[set_environment]: VS6_BIN_PATH is not set", file=sys.stderr)
        sys.exit(1)

    vcvars_path = f"{vs6_bin}\\VCVARS32.bat"

    # Run the batch file and capture the environment
    result = subprocess.run(
        f'"{vcvars_path}" && set',
        shell=True,
        capture_output=True,
        text=True
    )
    
    # Parse the environment variables from the output
    env_vars = {}
    for line in result.stdout.split('\n'):
        if '=' in line:
            key, value = line.split('=', 1)
            env_vars[key] = value
    
    # Update os.environ
    os.environ.update(env_vars)

def copy_assets():
    # Placeholder for asset copy logic
    print("[build]: Assets have been copied to build folder.")


def get_executable_name():
    return f"{PROJECT_NAME}.exe" if os.name == 'nt' else f"./{PROJECT_NAME}"


def run_build(launch_app, build_type):
    print(f"[build]: Running build for {PROJECT_NAME} with type {build_type}")

    # Set up VS6 environment
    set_environment()

    # Prepare build directory
    os.makedirs(BUILD_FOLDER, exist_ok=True)
    os.chdir(BUILD_FOLDER)

    # Configure with CMake
    track_process([
        'cmake',
        '..',
        '-G', 'NMake Makefiles',
        f'-DCMAKE_BUILD_TYPE={build_type}'
    ])

    # Build with NMake
    track_process(['nmake'])

    # Copy any assets
    copy_assets()

    # Launch application if requested
    if launch_app:
        exe_path = os.path.join(os.getcwd(), get_executable_name())
        print(f"----------------- Starting {PROJECT_NAME} -----------------")
        subprocess.run([exe_path])


def check_executable_size(build_test_type):
    exe_name = get_executable_name()
    exe_path = os.path.join(BUILD_FOLDER, exe_name)

    # Remove old executable
    if os.path.exists(exe_path):
        os.remove(exe_path)

    # Save current directory to restore later
    old_cwd = os.getcwd()

    # Run a fresh build
    run_build(launch_app=False, build_type=build_test_type)

    # Restore working directory
    os.chdir(old_cwd)

    # Measure size
    size_bytes = os.path.getsize(exe_path)
    size_kb = size_bytes / 1024
    size_mb = size_bytes / (1024 * 1024)
    print(f"[size]: Executable is {size_kb:.2f} KB ({size_mb:.2f} MB)")


def build_tools(app):
    # we want to use clang for toy2 tools
    os.environ['CC'] = 'clang'
    os.environ['CXX'] = 'clang++'

    TOOLS_BUILD_FOLDER = f"{BUILD_FOLDER}//build_tools"

    os.makedirs(TOOLS_BUILD_FOLDER, exist_ok=True)
    os.chdir(TOOLS_BUILD_FOLDER)

    build_arguments = [
        'cmake',
        '-G', 'Ninja',
        f'-DCMAKE_BUILD_TYPE=RelWithDebInfo',
        '-Wno-dev',
        '../../tools'
    ]

    track_process(build_arguments)
    track_process(['ninja'])

    if app != "nl":
        exe_path = os.path.join(os.getcwd(), f"{app}.exe")
        print(f"----------------- Starting {app} -----------------")
        subprocess.run([exe_path])

def main():
    parser = argparse.ArgumentParser(
        description=f"Build script for {PROJECT_NAME} using VS6 NMake."
    )
    parser.add_argument("--nl", action="store_true", help="Don't launch the app after build")
    parser.add_argument("--cs", action="store_true", help="Check executable size after build")

    parser.add_argument(
        "--tools", 
        choices=["SaveViewer"],
        help="Build and launch the Toy2 tools",
    )

    parser.add_argument(
        "--bt",
        choices=["Debug", "Release", "RelWithDebInfo", "MinSizeRel"],
        default="RelWithDebInfo",
        help="CMake build type (default: RelWithDebInfo)"
    )

    args = parser.parse_args()

    if args.tools:
        build_tools(args.tools)
        return

    if args.cs:
        check_executable_size(build_test_type=args.bt)
    else:
        run_build(launch_app=not args.nl, build_type=args.bt)


if __name__ == "__main__":
    main()
