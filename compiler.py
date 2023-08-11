import argparse
import os

from helpers import (
    Helpers,
    ANSIColors as Colors,
)

file_name = "opengl"
build_dir = "build"
source_dir = "src"
dependencies_dir = "dependencies"
include_dir = f"{dependencies_dir}/include"
lib_dir = f"{dependencies_dir}/lib"

libs = "-lglfw3 -lopengl32 -lgdi32"

def setupParser():
    parser = argparse.ArgumentParser(description="Compile and optionally run the source")
    parser.add_argument("--run", action="store_true", help="Run the executable")
    parser.add_argument("--clean", action="store_true", help="Clean the build directory")
    parser.add_argument("--build", action="store_true", help="Build the source")

    return parser

def main():
    args = setupParser().parse_args()

    if args.clean:
        print(f"{Colors.BLUE}Cleaning directory: {Colors.GRAY}{build_dir}{Colors.ENDC}{Colors.ENDC}")
        Helpers.clean_build_dir(build_dir)

    if args.build:
        if not os.path.exists(build_dir):
            print(f"{Colors.BLUE}Creating directory: {Colors.GRAY}{build_dir}{Colors.ENDC}{Colors.ENDC}")
            os.makedirs(build_dir)

        print(f"{Colors.BLUE}Compiling...\n{Colors.RED}{source_dir}/{Colors.ENDC}{Colors.ENDC}")
        
        source_files = Helpers.get_source_files([".cpp", ".c"], source_dir)
        object_files = Helpers.compile_source_files(source_files, build_dir, include_dir)

        print(f"{Colors.BLUE}\nLinking...\n{Colors.YELLOW}{libs}{Colors.ENDC}{Colors.ENDC}")
        Helpers.link_object_files(object_files, build_dir, file_name, lib_dir, libs)

    if args.run:
        print(f"{Colors.BLUE}\nRunning...\n{Colors.GREEN}{file_name}{Colors.ENDC}{Colors.ENDC}")
        name = file_name + ".exe"

        if not os.path.exists(os.path.join(build_dir, name)):
            print(f"{Colors.RED}Executable {name} does not exist.{Colors.ENDC}")
            return

        Helpers.run_executable(build_dir, name)

if __name__ == "__main__":
    main()
