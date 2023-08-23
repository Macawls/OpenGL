import os
import subprocess

class ANSIColors:
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    GRAY = '\033[90m'
    ENDC = '\033[0m'

class Helpers:
    @staticmethod
    def clean_build_dir(build_dir):
        if os.path.exists(build_dir):
            for f in os.listdir(build_dir):
                os.remove(os.path.join(build_dir, f))
            os.rmdir(build_dir)
        else:
            print(f"{ANSIColors.RED}Directory does not exist: {ANSIColors.GRAY}{build_dir}{ANSIColors.ENDC}")

    @staticmethod
    def get_source_files(extensions: list, source_dir):
        source_files = []
        for root, _, files in os.walk(source_dir):
            for file in files:
                if any(file.endswith(ext) for ext in extensions):
                    source_files.append(os.path.join(root, file))
        return source_files

    @staticmethod
    def compile_source_files(source_files, build_dir, include_dir):
        object_files = []

        for file in source_files:
            object_file = os.path.join(build_dir, os.path.splitext(os.path.basename(file))[0] + ".o")
            compile_command = f"g++ -c {file} -I {include_dir} -o {object_file}"
            print(f"{ANSIColors.GRAY}{compile_command}{ANSIColors.ENDC}")
            subprocess.call(compile_command, shell=True)
            object_files.append(object_file)

        return object_files

    @staticmethod
    def link_object_files(object_files, build_dir, file_name, lib_dir, lib, additional_flags):
        link_command = f"g++ {' '.join(object_files)} -o {os.path.join(build_dir, file_name)} -L {lib_dir} {lib} {additional_flags}"
        print(f"{ANSIColors.GRAY}{link_command}{ANSIColors.ENDC}")
        subprocess.call(link_command, shell=True)

    @staticmethod
    def clean_output_files(build_dir):
        for f in os.listdir(build_dir):
            if f.endswith(".o"):
                os.remove(os.path.join(build_dir, f))

    @staticmethod
    def run_executable(build_dir, file_name):
        run_command = os.path.join(build_dir, file_name)
        print(f"{ANSIColors.GRAY}{run_command}{ANSIColors.ENDC}")
        subprocess.call(run_command, shell=True)